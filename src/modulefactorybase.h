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
 * \file	modulefactorybase.h
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the abstract interface, that all module factories need
 * to provide. It also serves as the base class for ModuleFactory.
 *
 */

#ifndef __MODULEFACTORYBASE_H_
#define __MODULEFACTORYBASE_H_ 1

// C++ Headers
#include <string>

// Project Headers
#include "defs.h"
#include "logging.h"
#include "module.h"
#include "moduleregistry.h"


DECEPTION_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	ModuleFactoryBase
 *
 * This class defines the interface that all modulefactories need to
 * provide in order to work with the deception framework.
 * It mainly takes care of auto-registration with the ModuleRegistry.
 */
//}}}1 DXG DOC
class ModuleFactoryBase
{
	public:
		virtual Module* createModObject(void) const = 0;

		virtual ~ModuleFactoryBase(void);
		ModuleFactoryBase(std::string modName);

	private:
		static std::string className;	///< name for logging
		// hidden
		ModuleFactoryBase(void);
		ModuleFactoryBase(const ModuleFactoryBase &rCopy);
};

DECEPTION_NAMESPACE_END

/* causes gcc-2.9x to errout complaining about multiply defined
 *NullPointerException. even though the one we use is defined in a
 *private namespace Deception::
 */
//DECEPTION_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * This variable enables every module to log through the frameworks
 * logging facility.
 */
//}}}1 DXG DOC
extern Deception::Logging globLog;

#endif // __MODULEFACTORYBASE_H_
