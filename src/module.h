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
 * \file	module.h
 * Declares abstract class Module
 *
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 */

#ifndef __MODULE_H_
#define __MODULE_H_ 1

// C++ Headers
#include <string>

// Project Headers
#include "defs.h"
#include "socket.h"

DECEPTION_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class	Module
 *
 * This class defines the interface that all modules need to provide
 * in order to work with the deceptiond-framework.
 *
 * The virtual destructor is also defined, since pure virtual
 * destructors are not allowed!
 *
 * Class \c Module is needed by the main program, since it would
 * otherwise have no way of referring to the modules.
 */
//}}}1 DXG DOC
class Module
{
	public:
		//{{{1 DXG DOC
		/**
		 * \todo	a return type to indicate the status of the module
		 * 			would be appropriate.
		 *
		 * This method is the default entry point for a module.
		 * Every module needs to implement this method in its class that
		 * is registers with the ModuleFactory.
		 * From here other objectinstances can be created and further
		 * code be executed,to handle the connection and simulate the
		 * module's vulnerability.
		 *
		 * \param	*mySocket	Pointer to socket object
		 * \param	myOption	Option string
		 */
		//}}}1 DXG DOC
		virtual void modMain(Socket *mySocket, std::string myOption = "") = 0;
		//{{{1 DXG DOC
		/**
		 * Abstract classes need to define a virtual destructor
		 * otherwise several compilers will moan about it.
		 * This destructor does nothing and serves as a sort of dummy
		 * destructor for this class.
		 */
		//}}}1 DXG DOC
		virtual ~Module(void)
		{ // DESTRUCTOR
		}
};

DECEPTION_NAMESPACE_END

#endif // __MODULE_H_
