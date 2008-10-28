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

#ifndef _NULLPOINTEREXCEPTION_H
#define _NULLPOINTEREXCEPTION_H
/**
 * \file nullpointerexception.h
 * 
 * \author Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains declaration of generic NullPointer class.
 */

#include "defs.h"
#include "exception.h"

DECEPTION_NAMESPACE_BEGIN

// {{{1 BEGIN DXG DOC
/**
 * Generic class for use to catch errors with undefined pointers.
 */
// }}}1
class NullPointerException : public Exception
{ // {{{1 SOURCE
	public:
		NullPointerException() {}
		NullPointerException(std::string _message) : Exception(_message) {}
		NullPointerException(int _errno) : Exception(_errno) {}
		// {{{2 DXG DOC
		/**
		 * Return name of current exception.
		 *
		 * \return Name of exception
		 **/
		// }}}2 DXG DOC
		virtual std::string getType() const
		{
			return "NullPointerException";
		}
}; // }}}1

DECEPTION_NAMESPACE_END

#endif // _NULLPOINTEREXCEPTION_H
