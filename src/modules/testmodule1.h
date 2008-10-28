/**
 * \file	testmodule1.h
 * \brief	declares the main class TestModule1 for testmodule1
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * this file declares the main class for an example testmodule implementation.
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

#ifndef __TESTMODULE_A_H_
#define __TESTMODULE_A_H_ 1

// C++ Headers
#include <iostream>

// Project Headers
#include "defs.h"
#include "module.h"
#include "socket.h"
#include "modulefactory.tcc"

DECEPTION_NAMESPACE_USE;

namespace Testmodule1 {

//{{{1 DXG DOC
/**
 * \class	TestModule1
 * \brief	declaration for a test class
 *
 * this class defines the interface that all modules need to provide
 * in order to work with the deceptiond-framework.
 */
//}}}1 DXG DOC
class TestModule1 : public Module
{
	public:
		void modMain(Socket *mySocket, std::string myOption);

		TestModule1(void);
		~TestModule1(void);

	private:
		TestModule1(const TestModule1 &rCopy);
};

} // end namespace Testmodule1

//{{{1 DXG DOC
/**
 * \brief	this module's specific factory.
 *
 * the declaration is needed outside of the Deception namespace to
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
extern ModuleFactory<Testmodule1::TestModule1> *testModuleA;

#endif // __TESTMODULE_A_H_
