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
 * \file	dtk-port.h
 * \brief	declares class DtkPort
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the main class for the Dtk-Port Compatability.
 *
 */

#ifndef __DTK_PORT_H_
#define __DTK_PORT_H_ 1

// Project Headers
#include "defs.h"
#include "socket.h"
#include "module.h"
#include "logging.h"
#include "moduleoptions.h"
#include "modulefactory.tcc"

//{{{1 DXG DOC
/**
 * \namespace	Dtkport
 * \brief		Namespace for the dtk port module.
 *
 * The \c Dtkport namespace groups the symbols needed for the Dtk
 * Port Compatability.  This is just a small safety hook to avoid 
 * two symbols of the same name colliding at runtime.  Depending on the
 * compiler multiply defined symbols may still be possible.
 */
//}}}1 DXG DOC
/// namespace for the dtk script module
#define DTKPORT_NAMESPACE_NAME Dtkport
/// begin namespace definition
#define DTKPORT_NAMESPACE_BEGIN namespace DTKPORT_NAMESPACE_NAME {
/// end namespace definition
#define DTKPORT_NAMESPACE_END }
/// use namespace definition
#define DTKPORT_NAMESPACE_USE using namespace DTKPORT_NAMESPACE_NAME

DECEPTION_NAMESPACE_USE;

DTKPORT_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	DtkPort
 * \brief	simulate the functionality of the dtk port (365)
 *
 * The Deception Toolkit may be set up to bind to port 365 and issue
 * some sort of "Yes, we are decepting." message.
 * This is the Module providing the same functionality.
 */
//}}}1 DXG DOC
class DtkPort : public Module
{
	public:
		void modMain(Socket *mySocket, std::string myOptionString);

		~DtkPort(void);
		DtkPort(void);

	private:
		DtkPort(const DtkPort &rCopy);
};

DTKPORT_NAMESPACE_END 

DTKPORT_NAMESPACE_USE;

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
extern ModuleFactory<DtkPort> *dtkPort;

//{{{1 DXG DOC
/**
 * \brief	the global logging facility
 *
 * this variable enables every module to log through the frameworks
 * logging facility.
 */
//}}}1 DXG DOC
extern Logging globLog;

#endif // __DTK_PORT_H_
