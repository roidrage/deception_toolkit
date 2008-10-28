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
 * \file	moduleconfighandler.h
 * \author	alexis hildebrandt <afh@2drop.net>
 * \author	mathias meyer <pom@beatsteaks.de
 *
 * Class declaration for ConfigHandler
 */

#ifndef __MODULECONFIGURATION_HANDLER_H_
#define __MODULECONFIGURATION_HANDLER_H_ 1

// C++ Headers
#include <map>
#include <string>
#include <iostream>

// Xerces Headers
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// Project Headers
#include "defs.h"

// failsafe fallback for non gcc compiler
#ifndef __GNUC__
#define __GNUC__ 0
#endif

#ifdef XERCES_HAS_CPP_NAMESPACE
#if __GNUC__ == 2
	/*
	 * gcc 2.95.x does not need to use the xerces namespace
	 * since it uses the older 2.1.0 version of libxerces-c
	 */
#else
XERCES_CPP_NAMESPACE_USE
#endif
#endif


DECEPTION_NAMESPACE_BEGIN


//{{{1 DXG DOC
/**
 * \class ConfigHandler
 *
 * This class is our default configuration handling facility. It is derived 
 * from DefaultHandler, a class included in Xerces C++. Functions of
 * interest can just be overloaded this way and all is well. 
 */
//}}}1
class ConfigHandler : public DefaultHandler
{
	public:
		void fatalError(const SAXParseException&);
		void startElement(const XMLCh* const uri,
				const XMLCh* const localName,
				const XMLCh* const qName,
				const Attributes& attribs);
		void endElement(const XMLCh* const uri,
				const XMLCh* const localName,
				const XMLCh* const qName);
		void characters(const XMLCh *const chars, const unsigned int len);
		bool isWhitespace(const std::string& str) const;
		ConfigHandler() :
			inModule(false),
			inLogFile(false),
			inModuleDir(false),
			inHostList(false),
			inUser(false),
			inGroup(false),
			inCapture(false),
			mDirRead(false),
			logFileRead(false),
			userRead(false),
			groupRead(false),
			captureRead(false),
			socketCount(0)
		   	{ }
		std::string getLogFile() const
		{
			return this->logFile;
		}
	private:
		static std::string className;	///< name for logging
		std::string curModDirName;		///< current module directory path
		std::string curModFileName;		///< current module filename
		std::string curModName;			///< current module name
		std::string curModOption;		///< current module option
		std::string ipaddr;				///< ip address of current virtual host
		std::string user;				///< user name, child processes shall belong to
		std::string group;				///< group name, child processes shall belong to
		std::string logFile;			///< file to log to
		bool inModule;					///< flag, if ports can be read
		bool inLogFile;					///< flag, if logfile name can be read
		bool inModuleDir;				///< flag, if directory name of moduledir can be read
		bool inHostList;				///< flag, if config reached a list of port for a specific virtual host
		bool inUser;					///< flag, if user name can now be read
		bool inGroup;					///< flag, if group name can now be read
		bool inCapture;					///< flag, if capture device can be read
		bool mDirRead;					///< flag, if module directory has already been read
		bool logFileRead;				///< flag, if log file has been read
		bool userRead;					///< flag, if user name has been read
		bool groupRead;					///< flag, if group name has been read
		bool captureRead;				///< flag, if capture device has been read
		int socketCount;				///< counter to check, if we don't have more sockets than OPEN_MAX
};

DECEPTION_NAMESPACE_END
#endif // __MODULECONFIGURATION_HANDLER_H_
