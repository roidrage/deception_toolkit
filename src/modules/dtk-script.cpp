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
 * \file	dtk-script.cpp
 * \brief	implements methods in class DtkScript
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file implements the methods for class for DtkScript.
 *
 */

// Module Headers
#include "dtk-script.h"
#include "dtk-scriptfsm.h"

DECEPTION_NAMESPACE_USE;
DTKSCRIPT_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * \brief	defines the module name for this module
 *
 * every module needs a unique module name.
 * it could also be a const char* parameter in the ModuleFactory constructor,
 * but a seperate const variable is preferred, since it seems more clear
 * that way.
 */
//}}}1 DXG DOC
std::string moduleName = 
	"dtkScript";
/// \brief instance of this modules factory see header file for details
ModuleFactory<DtkScript> *dtkScript = 
	new ModuleFactory<DtkScript>(moduleName);


//{{{1 DXG DOC
/**
 * \brief	the modules main entry point
 *
 * the main daemon program will call this method to pass execution onto 
 * the module.
 *
 * \todo	FIXME: char[6] with snprintf(5) will that work ?
 *
 * \return	void
 * \retval	none
 *
 * \param	*mySocket	the socket that the client has connected from
 * \param	myOption	the modules specific option string
 * 			form the configuration file
 */
//}}}1 DXG DOC
void DtkScript::modMain(Socket *mySocket, std::string myOption)
{
	std::string scriptDir;
	std::string dtkScript;
	std::string dtkScriptExt = ".response";

	// retrive module options
	ModuleOptions myOptions(myOption);

	// get dtk script name
	char port[6];
	snprintf(port, 6, "%d", mySocket->getPort());
	dtkScript = port + dtkScriptExt;
	scriptDir = myOptions.getOption("dtkscriptdir");
	if (scriptDir.empty() == true) {
		globLog.toLog(moduleName, ModuleError, "dtkscriptdir-option not configured correctly.");
		// XXX: return really okay ?
		return;
	}

	// set up the finite state machine
	DtkScriptFSM fsm(mySocket->getInputStream(), mySocket->getOutputStream());
	fsm.init(scriptDir, dtkScript);

	// pass execution to the fsm
	fsm.start();

	// write notification to the logging mechanism
	globLog.toLog(moduleName, ModuleInfo, "fsm terminated");

	return;
}

//{{{1 DXG DOC
/**
 * \brief	default constructor
 */
//}}}1 DXG DOC
DtkScript::DtkScript(void)
{
	return;
}

//{{{1 DXG DOC
/**
 * \brief	default destructor
 */
//}}}1 DXG DOC
DtkScript::~DtkScript(void)
{
	return;
}
