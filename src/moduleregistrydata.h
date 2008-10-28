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
 * \file	moduleregistrydata.h
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares ModuleRegistryData, the class that holds all
 * important data to do the module plug-in handling.
 *
 */

#ifndef __MODULEREGISTRYDATA_H_
#define __MODULEREGISTRYDATA_H_ 1

// C++ Headers
#include <map>
#include <string>

// Project Headers
#include "defs.h"
#include "module.h"
#include "socket.h"


DECEPTION_NAMESPACE_BEGIN

//forward declaration
class ModuleFactoryBase;

//{{{1 DXG DOC
/**
 * \class	ModuleRegistryData
 *
 * This class specifies which data is needed so that a module is
 * correctly registered.
 */
//}}}1 DXG DOC
class ModuleRegistryData
{
	public:
		void setFileName(std::string modFileName);
		std::string getFileName(void) const;
		void setFactory(ModuleFactoryBase *factory);
		ModuleFactoryBase* getFactory(void) const;
		void setHandle(void *handle);
		void* getHandle(void) const;
		void setModName(std::string modName);
		std::string getModName(void) const;
		void setSocket(Socket *socket);
		Socket* getSocket(void) const;
		void setOption(std::string option);
		std::string getOption(void) const;

		virtual ~ModuleRegistryData(void);
		ModuleRegistryData(void);
		ModuleRegistryData(std::string modFileName, std::string modName);

	private:
		std::string fileName;			///< a module's filename
		ModuleFactoryBase *pFactory;	///< pointer to module's factory
		void *pHandle;					///< pointer to module handle
		std::string name;				///< module name
		std::string option;				///< the module's option string
		Socket *pSocket;				///< the module's socket

		// hidden
		ModuleRegistryData(const ModuleRegistryData &rCopy);
		ModuleRegistryData& operator=(const ModuleRegistryData &rhs);
};

DECEPTION_NAMESPACE_END

#endif // __MODULEREGISTRYDATA_H_
