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
 * \file	moduleregistry.h
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the class ModuleRegistry.
 *
 */

#ifndef __MODULEREGISTRY_H_
#define __MODULEREGISTRY_H_ 1

// C++ Headers
#include <map>
#include <string>
#include <iostream>

// Project Headers
#include "defs.h"
#include "socket.h"
#include "module.h"
#include "logging.h"
#include "nullpointerexception.h"
#include "moduleregistrydata.h"


DECEPTION_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	ModuleRegistry
 *
 * This class provides a central point where most necessary data for
 * the module handling (loading, unloading, instantiation) is kept.
 */
//}}}1 DXG DOC
class ModuleRegistry
{
	public:
		// types
		//{{{ 2 DXG DOC
		/**
		 * Convenience encapsulation.
		 *
		 * For details see Item 2 p.18 in "Effective STL" by S. Meyers.
		 */
		//}}} 2 DXG DOC
		typedef std::map<std::string, ModuleRegistryData*> ModuleRegistryMap;
		//{{{ 2 DXG DOC
		/**
		 * Convenience encapsulation.
		 *
		 * For details see Item 2 p.18 in "Effective STL" by S. Meyers.
		 */
		//}}} 2 DXG DOC
		typedef ModuleRegistryMap::iterator ModuleRegistryMapIterator;


		// methods
		ModuleFactoryBase* find(std::string idx);
		void addModule(std::string ipAddr, unsigned int portNum,
				std::string modFileName,
				std::string modName,
				std::string option);
		void insert(std::string modName, ModuleFactoryBase *pFactory);
		void insert(std::string modFileName, void *pHandle);
		void insert(std::string ipAddr, unsigned int portNum, Socket *socket);
		void* getHandle(std::string modFileName);
		ModuleRegistryMapIterator begin(void);
		ModuleRegistryMapIterator end(void);
		unsigned int getPort(std::string ipport);
		std::string getIpAddr(std::string ipport);
		std::string getOption(std::string idx);
		void remove(std::string fileName);

		virtual ~ModuleRegistry(void);
		ModuleRegistry(void);

	private:
		static std::string className;	///< name for logging
		static ModuleRegistryMap data;	///< the actual registration data

		//{{{ 2 DXG DOC
		/**
		 * Convenience encapsulation.
		 *
		 * For details see Item 2 p.18 in "Effective STL" by S. Meyers.
		 */
		//}}} 2 DXG DOC
		typedef enum nameType { MODNAME, FILENAME } NameType;
		std::string findPort(std::string &name, NameType type);

		// hidden
		ModuleRegistry(const ModuleRegistry &rCopy);
};

DECEPTION_NAMESPACE_END

//DECEPTION_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * The global logging facility
 *
 * This variable enables every module to log through the frameworks
 * logging facility.
 */
//}}}1 DXG DOC
extern Deception::Logging globLog;


#endif // __MODULEREGISTRY_H_
