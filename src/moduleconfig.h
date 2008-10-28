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
 * \file	moduleconfig.h
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares an easy interface to deceptiond configuration
 * loading
 */

#ifndef __MODULECONFIGURATION_H_
#define __MODULECONFIGURATION_H_ 1

// C++ Headers
#include <string>

// Project Headers
#include "moduleconfighandler.h"
#include "defs.h"

DECEPTION_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	Config
 *
 * This class defines an easy interface to moduleloading.
 * It takes care of the necessary steps needed additionally to auto-/
 * self-registration.
 */
//}}}1 DXG DOC
class Config
{
	public:
		void read(std::string fileName);
		
		Config(void);
		~Config(void);

	private:
		Config(const Config &rCopy);
};

DECEPTION_NAMESPACE_END
#endif // __MODULECONFIGURATION_H_
