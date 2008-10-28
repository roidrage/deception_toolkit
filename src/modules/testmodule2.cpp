/**
 * \file	testmodule2.cpp
 * \brief	implementation of the main class TestModule1 for testmodule1
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * this file implements the main class for a testmodule implementation.
 *
 *
 * Copyright (c) 2003, Alexis Hildebrandt, Mathias Meyer
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *  - Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// Project Headers
#include "testmodule2.h"

DECEPTION_NAMESPACE_USE
using namespace Testmodule2;

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
const std::string moduleName = 
	"testModuleB";
/// \brief definition and initalisation of testModule
// see testmodule1.h for details on testModule
ModuleFactory<TestModule2> *testModuleB = 
	new ModuleFactory<TestModule2>(moduleName);


//{{{1 DXG DOC
/**
 * \brief	dummy method.
 *
 * this is merely a dummy function for this testing module.
 *
 * \return	void
 * \retval	none
 *
 * \param	*mySocket	the socket that the client has connected form
 * \param	myOption	the modules specific options string form the config
 */
//}}}1 DXG DOC
void TestModule2::modMain(Socket *mySocket, std::string myOption)
{
	// dummy code to overcome CC warns
	(void) mySocket->getFd();
	std::cout << "DUMMY method TestModule2::modMain()\n";

	return;
}

//{{{1 DXG DOC
/**
 * \brief	default constructor
 *
 * creates a TestModule1 object
 */
//}}}1 DXG DOC
TestModule2::TestModule2(void)
{
	return;
}

//{{{1 DXG DOC
/**
 * \brief	default destructor
 *
 * destroys a TestModule1 object
 */
//}}}1 DXG DOC
TestModule2::~TestModule2(void)
{
	return;
}
