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
 * \file	moduleconfig.cpp
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file implements the easy interface provided by Config
 */

#include <iostream>		// currently only for debugging purposes needed.
#include <exception>

#include "moduleconfig.h"
#include "nullpointerexception.h"
#include "ioexception.h"
#include "logging.h"

extern Deception::Logging globLog;

//{{{1 DXG DOC
/**
 * Creates necessary data structures for Xerces and starts
 * reading of configuration file
 * 
 * \param fileName Filename of configuration file 
 */
//}}}1 DXG DOC
void Deception::Config::read(std::string fileName)
{
    // get Xerces running
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException &e) {
        std::cerr << " Error during XML Init: "
            << XMLString::transcode(e.getMessage()) << std::endl;
        ::exit(EXIT_FAILURE);
    }

   // set up the xml parser
    SAX2XMLReader *parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreValidation, true);

	Deception::ConfigHandler *configHandler = new Deception::ConfigHandler;

    parser->setContentHandler(configHandler);
    parser->setErrorHandler(configHandler);
    // parse the config file
    // this fills the ModuleRegistry with emtpy Data.
    try {
        parser->parse(fileName.c_str());
    }
    catch (XMLException &e) {
        std::cerr << "xerces error: "
            << XMLString::transcode(e.getMessage()) << std::endl;
        ::exit(EXIT_FAILURE);
    }
    catch (const SAXParseException &e) {
        std::cerr << "xerces parsing error: "
            << XMLString::transcode(e.getMessage()) << std::endl;
        ::exit(EXIT_FAILURE);
    }
	catch (Deception::NullPointerException &e) {
		std::cerr << "nullpointer error: " << e.toString() << std::endl;
		::exit(EXIT_FAILURE);
	}
	catch (Deception::IOException &e) {
		std::cerr << "critical i/o error: " << e.toString() << std::endl;
		::exit(EXIT_FAILURE);
	} catch (std::exception &e) {
		std::cerr << "error in standard library: " << e.what() << std::endl;
		::exit(EXIT_FAILURE);
	}
    delete(parser);
    delete(configHandler);
	return;
}

//{{{1 DXG DOC
/**
 * The default constructor creates a Config object,
 * and allocates a new \c ModuleRegistry
 * for the private member \c this->store.
 */
//}}}1 DXG DOC
Deception::Config::Config(void)
{ // DEFAULT CONSTRUCTOR
	return;
}

//{{{1 DXG DOC
/**
 * Destroys a Config object and deallocates the memory used by \c
 * this->store.
 */
//}}}1 DXG DOC
Deception::Config::~Config(void)
{ // DESTRUCTOR
	return;
}
