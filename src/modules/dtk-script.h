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
 * \file	dtk-script.h
 * \brief	declares class DtkScript
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the main class for the Dtk-Script Compatability.
 *
 */

#ifndef __DTK_SCRIPT_H_
#define __DTK_SCRIPT_H_ 1

// C++ Headers

// Project Headers
#include "defs.h"
#include "socket.h"
#include "module.h"
#include "moduleoptions.h"
#include "modulefactory.tcc"

// Module Headers
#include "dtk-scriptdefs.h"


DECEPTION_NAMESPACE_USE;

DTKSCRIPT_NAMESPACE_BEGIN 

//{{{1 DXG DOC
/**
 * \class	DtkScript
 * \brief	provides compatability to run Dtk response files.
 *
 * The Deception Toolkit uses so called response files to simulate 
 * network vulnerabilities.  These simple text files may be edited at
 * runtime, providing some sort of scripting mechanism.
 * This class enables the framework to use these response files
 * (scripts).
 */
//}}}1 DXG DOC
class DtkScript : public Module
{
	public:
		void modMain(Socket *mySocket, std::string myOptionString);

		~DtkScript(void);
		DtkScript(void);

	private:
		DtkScript(const DtkScript &rCopy);
};

DTKSCRIPT_NAMESPACE_END

DTKSCRIPT_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * \brief	this module's specific factory.
 *
 * the declaration is needed outside of the module's own namespace to
 * overcome the following icc warning:
 * \code
 * remark #1418: external definition with no prior declaration
 * ModuleFactory<TestModule> *testModule
 * \endcode
 *
 * the parametrized instance of ModuleFactory will enable the
 * deceptiond-framework to work with this module and call it when an
 * appropriate connection request arrives.
 */
//}}}1 DXG DOC
extern ModuleFactory<DtkScript> *dtkScript;

#endif // __DTK_SCRIPT_H_
