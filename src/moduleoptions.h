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
 * \file	moduleoptions.h
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the main class for Module Options handling.
 *
 */

#ifndef __MODULEOPTIONS_H_
#define __MODULEOPTIONS_H_ 1

// C++ Headers
#include <map>
#include <string>

// Project Headers
#include "defs.h"

DECEPTION_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	ModuleOptions
 *
 * \todo	Allow for a multiple key-value pairs in the optionstring
 * \todo	Allow for a custom delimiter on the optionstring
 *
 * This class parses the optionstring from the configuration file and
 * stores its key value pairs in a private data member.
 */
//}}}1 DXG DOC
class ModuleOptions
{
	public:
		std::string getOption(std::string key);
		
		ModuleOptions(std::string optString = "");
		~ModuleOptions(void);

	private:
		//{{{ 2 DXG DOC
		/**
		 * Convenience type defintion for map
		 *
		 * The private data member has quite a lengthy declaration this
		 * is just a short name for it.
		 */
		//}}} 2 DXG DOC
		typedef std::map<std::string, std::string> ModuleOptionsMap;
		ModuleOptionsMap data;	///< key-value pairs from the optionstring

		// hidden
		ModuleOptions(const ModuleOptions &rCopy);
};

DECEPTION_NAMESPACE_END

#endif // __MODULEOPTIONS_H_
