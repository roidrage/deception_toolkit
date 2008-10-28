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
 * \file	moduleloader.h
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares an easy interface to moduleloading.
 *
 */

#ifndef __MODULELOADER_H_
#define __MODULELOADER_H_ 1

// C++ Headers
#include <string>

// C Headers
#include <dlfcn.h>

// Project Headers
#include "defs.h"
#include "logging.h"
#include "moduleregistry.h"
#include "dlcloseexception.h"
#include "nullpointerexception.h"

#ifndef RTLD_NOW
/// needed for BSD compatability.
#define RTLD_NOW RTLD_LAZY
#endif


DECEPTION_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	ModuleLoader
 *
 * \todo	The static \c moduleDir variable does not really
 * 			go well with the intended vhosts, that can have their own
 * 			dtkscripts directory.
 *
 * This class defines an easy interface to moduleloading.
 * It takes care of the necessary steps and additionally
 * automatically initiates the self-registration of the objects.
 */
//}}}1 DXG DOC
class ModuleLoader
{
	public:
		void open(std::string modFileName);
		void close(std::string modFileName);
		void setModDir(std::string modDir);
		void loadAllModules(void);
		
		ModuleLoader(void);
		~ModuleLoader(void);

	private:
		static std::string className; ///< name for logging
		static std::string moduleDir; ///< directory path where modules reside
		ModuleRegistry *store; 	///< private pointer to the loaders registry

		//  hidden
		ModuleLoader(const ModuleLoader &rCopy);
		ModuleLoader* operator=(const ModuleLoader &rhs);
};

DECEPTION_NAMESPACE_END

//DECEPTION_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * This variable enables every module to log through the frameworks
 * logging facility.
 */
//}}}1 DXG DOC
extern Deception::Logging globLog;

#endif // __MODULELOADER_H_
