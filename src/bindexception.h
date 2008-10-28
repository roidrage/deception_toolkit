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

#ifndef _BINDEXCEPTION_H
#define _BINDEXCEPTION_H
/**
 * \file bindexception.h
 * \author	Mathias Meyer <pom@beatsteaks.de>
 * 
 * Contains declaration of specific BindException class.
 */

#include <string>

#include "defs.h"
#include "ioexception.h"

DECEPTION_NAMESPACE_BEGIN

// {{{1
/**
 * \class BindException
 *
 * This class represents a specific ::bind(2) error handler. It is derived from
 * IOException.
 *
 * \note It's specific to system call bind() and should only be thrown
 * for functions like Socket::bind()
 */
// }}}1
class BindException : public IOException
{
	public:
		BindException() {}
		BindException(std::string _message) : IOException(_message) {}
		BindException(int _errno) : IOException(_errno) {}
		// {{{1 DXG DOC
		/**
		 * Return name of current exception.
		 *
		 * \return name of exception
		 **/
		// }}}1 DXG DOC
		virtual std::string getType() const
		{
			return "BindException";
		}
};

DECEPTION_NAMESPACE_END

#endif // _BINDEXCEPTION_H
