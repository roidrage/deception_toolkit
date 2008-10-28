// Copyright (c) 2003, Alexis Hildebrandt, Mathias Meyer
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the authors nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * \file	moduleconfighandler.cpp
 * \author	alexis hildebrandt <afh@2drop.net>
 * \author 	mathias meyer <pom@beatsteaks.de>
 *
 * Contains the handler class implementation to deal with all configuration stuff
 */

#if defined(__linux__)
#include <linux/limits.h>
#else
#include <limits.h>
#endif

// project Headers
#include "moduleconfighandler.h"
#include "moduleregistry.h"
#include "moduleloader.h"
#include "logging.h"
#include "fw_pcap.h"

#include <xercesc/util/NumberFormatException.hpp>


const char* OPTION_MODULE		= "module";
const char* OPTION_MODULEDIR	= "moduledir";
const char* OPTION_FILENAME		= "filename";
const char* OPTION_NAME			= "name";
const char* OPTION_LOGFILE		= "logfile";
const char* OPTION_PORT			= "port";
const char* OPTION_PORTNO		= "no";
const char* OPTION_CHROOT		= "chroot";
const char* OPTION_USER			= "user";
const char* OPTION_GROUP		= "group";
const char* OPTION_HOST			= "host";
const char* OPTION_IP			= "ipaddr";
const char* OPTION_OPTION		= "option";
const char* OPTION_CAPTURE		= "capture";
const char* OPTION_CAP_ENABLE	= "enable";

extern Deception::Logging globLog;
extern std::string runUser;
extern std::string runGroup;
extern std::string capDevice;
extern bool enableCapture;

// define statics
std::string Deception::ConfigHandler::className = "ConfigHandler";

// not using the following statment we need to add Deception:: to all
// method headers!
// but screw it for now, since our own exceptions cause ambiguity with that
// of xerces
//DECEPTION_NAMESPACE_USE

/**
 * Is called from SAX2XMLReader in case a chunk of characters occurs in
 * an XML file. We use it to get the name of our logfile and the
 * directory that our modules reside in.
 *
 * \note Although there is a function DefaultHandler::ignorableWhitespace(),
 * this function is also called with strings containing whitespaces.
 *
 * \see isWhitespace
 */
void Deception::ConfigHandler::characters(const XMLCh *const chars,
		const unsigned int len)
{
	// check if given string is empty, because empty strings
	// are non of our business
	if (this->isWhitespace(std::string(XMLString::transcode(chars)))) {
		return;
	} else {
		// is it our module directory?
		if (this->inModuleDir && (this->curModDirName.length() == 0) && !this->mDirRead) {
			this->curModDirName.append(XMLString::transcode(chars));
		// or just our logfile?
		} else if (this->inLogFile && !this->logFileRead) {
			this->logFile.append(XMLString::transcode(chars));
		} else if (this->inUser && !this->userRead) {
			runUser.append(XMLString::transcode(chars));
		} else if (this->inGroup && !this->groupRead) {
			runGroup.append(XMLString::transcode(chars));
		} else if (this->inCapture && !this->captureRead) {
			capDevice.append(XMLString::transcode(chars));
		}
	}
}

// {{{1 DXG DOC
/**
 * Use this function to see, if a string only contains of \t or ' '.
 *
 * \param &str string to search in
 *
 * \retval true If string contains of whitespaces
 * \retval false If it doesn't
 */
// }}}1 DXG DOC
bool Deception::ConfigHandler::isWhitespace(const std::string &str) const
{
	for (unsigned int i = 0; i < str.length(); i++) {
		if ((str.at(i) == '\t') || (str.at(i) == ' ')) {
			continue;
		} else {
			return false;
		}
	}
	return true;
}

//{{{1 DXG DOC
/**
 * This function is executed by SAX2XMLReader every time a new element
 * is found in the config file
 *
 * \param uri The URI of the associated Namespace
 * \param localName The local part of the element name
 * \param qName The QName of this element
 * \param attribs A list of defined attributes in the current element
 */ 
//}}}1 DXG DOC
void Deception::ConfigHandler::startElement(const XMLCh* const uri,
									const XMLCh* const localName,
									const XMLCh* const qName,
									const Attributes& attribs)
{
	Deception::ModuleRegistry store;
	int port;
	std::string logMsg;
	// xml element <modulelist>?
	if (std::string(XMLString::transcode(qName)).compare(OPTION_MODULE) == 0) {
		// then say so and get the hell outta here
		this->inModule = true;
	} else if ((std::string(XMLString::transcode(qName)).compare(OPTION_LOGFILE) == 0)
			&& (this->logFile.length() == 0)) {
		this->inLogFile = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_MODULEDIR) == 0) {
		this->inModuleDir = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_HOST) == 0) {
		this->inHostList = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_USER) == 0) {
		this->inUser = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_GROUP) == 0) {
		this->inGroup = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_CAPTURE) == 0) {
		this->inCapture = true;
	}
	// fetch attributes
	for (unsigned int i = 0; i < attribs.getLength(); i++) {

		if ((std::string(XMLString::transcode(qName)).compare(OPTION_HOST) == 0)) {
			if (std::string(XMLString::transcode(attribs.getQName(i))).compare(OPTION_IP) == 0) {
				// get ipaddr
				this->ipaddr = XMLString::transcode(attribs.getValue(i));
			}
		// are we in element <module>?
		} else if ((std::string(XMLString::transcode(qName)).compare(OPTION_MODULE) == 0) && this->inHostList) {

			// if yes then get modulefilename,
			if (std::string(XMLString::transcode(attribs.getQName(i))).compare(OPTION_FILENAME) == 0) {
				// save modulefilename in class
				this->curModFileName = XMLString::transcode(attribs.getValue(i));
			} else // the modulename
			if (std::string(XMLString::transcode(attribs.getQName(i))).compare(OPTION_NAME) == 0) {
				// save modulefilename in class
				this->curModName = XMLString::transcode(attribs.getValue(i));
			} else // and the moduleoption
			if (std::string(XMLString::transcode(attribs.getQName(i))).compare(OPTION_OPTION) == 0) {
				// save modulefilename in class
				this->curModOption = XMLString::transcode(attribs.getValue(i));
			}
		// is it element <port>?
		} else if ((std::string(XMLString::transcode(qName)).compare(OPTION_PORT) == 0)
				&& this->inHostList && this->inModule) {

			// then let's check if there is an attribute called 'no'
			// specifying or nice port number
			if (std::string(XMLString::transcode(attribs.getQName(i))).compare(OPTION_PORTNO) == 0) {
				// is there currently a module defined?
				if (this->curModFileName.empty())
					continue;

				// get port
				try {
					port = XMLString::parseInt(attribs.getValue(i));
				} catch (NumberFormatException &e) {
					continue;
				}

				if (this->socketCount >= OPEN_MAX) {
					logMsg = "maximum number of open file descriptors reached.";
					logMsg.append(" dropping configuration for " + ipaddr + ":").append(intToString(port));
					globLog.toLog(this->className, Deception::Error, logMsg);
					logMsg.erase();
					continue;
				}				
				// is this port in any way legal?
				if ((port < 0) || (port > 65536)) {
					// otherwise we'll just ignore the crap
					logMsg.erase();
					logMsg.append("ignoring configuration port ").append(intToString(port))
						.append(" from module ").append(this->curModFileName)
						.append(": out of range");
					globLog.toLog(this->className, Deception::Debug, logMsg);
					continue;
				}

#ifdef DEBUG
				logMsg = "adding data for " + this->ipaddr + ":" + intToString(port)
					+ " with module " + this->curModName;
				globLog.toLog(this->className, Deception::Debug, logMsg);
#endif
				// add module data to store
				store.addModule(this->ipaddr, port, this->curModFileName, this->curModName, this->curModOption);
				this->socketCount++;
			}
		} else if (this->inCapture && (std::string(XMLString::transcode(attribs.getQName(i))).compare(OPTION_CAP_ENABLE) == 0)) {
			if (std::string(XMLString::transcode(attribs.getValue(i))).compare("yes") == 0) {
				enableCapture = true;
			}
		}
	} // end for

	return;
}

//{{{1 DXG DOC
/**
 * This function is called at the end of an element. Its purpose is to
 * reset this->currentModule to make configuration cooler and maybe a bit safer.
 */
//}}}1 DXG DOC
void Deception::ConfigHandler::endElement(const XMLCh* const uri,
		const XMLCh* const localName,
		const XMLCh* const qName)
{
	if (std::string(XMLString::transcode(qName)).compare(OPTION_MODULEDIR) == 0) {
		Deception::ModuleLoader ml;
		this->mDirRead = true;
		if (!this->curModDirName.empty())
			ml.setModDir(this->curModDirName);
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_MODULE) == 0) {
		this->curModFileName.erase();
		this->curModName.erase();
		this->inModule = false;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_LOGFILE) == 0) {
		// logfile info has ended
		this->inLogFile = false;
		this->logFileRead = true;
		globLog.setLogFile(this->logFile);
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_USER) == 0) {
		this->inUser = false;
		this->userRead = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_GROUP) == 0) {
		this->inGroup =  false;
		this->groupRead = true;
	} else if (std::string(XMLString::transcode(qName)).compare(OPTION_CAPTURE) == 0) {
		this->inCapture = false;
		this->captureRead = true;
	}

	return;
}

//{{{1 DXG DOC
/**
 * Error function to be used for parsing exceptions which could occure
 * in SAX2XMLReader.
 *
 * \param &e The occured exception
 */
//}}}1 DXG DOC
void Deception::ConfigHandler::fatalError(const SAXParseException& e)
{
	std::string logMsg = "error in xerces: ";
	logMsg.append(XMLString::transcode(e.getMessage()));
	globLog.toLog(this->className, Deception::Error, logMsg);
	return;
}
