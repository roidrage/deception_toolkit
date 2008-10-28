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
 * \file noclientexception.h
 *
 * \author Mathias Meyer <pom@beatsteaks.de>
 * 
 * Defines exception class for ::accept()
 */
#ifndef _NOCLIENTEXCEPTION_H
#define _NOCLIENTEXCEPTION_H

#include "defs.h"
#include "ioexception.h"


DECEPTION_NAMESPACE_BEGIN

// {{{1 BEGIN DXG DOC
/**
 * \class NoClientException
 *
 * Signalizes, that a call to ::accept did not return a valid client descriptor.
 */
// }}}1
class NoClientException : public IOException
{
	public:
		NoClientException() {}
		NoClientException(std::string _message) : IOException(_message) {}
		NoClientException(int _errno) : IOException(_errno) {}
		// {{{1 DXG DOC
		/**
		 * Return name of current exception.
		 *
		 * \return Name of exception
		 **/
		// }}}1 DXG DOC
		virtual std::string getType() const
		{
			return "NoClientException";
		}
};

DECEPTION_NAMESPACE_END

#endif // _NOCLIENTEXCEPTION_H
