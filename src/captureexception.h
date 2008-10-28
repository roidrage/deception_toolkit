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


#ifndef _CAPTURE_EXCEPTION_H
#define _CAPTURE_EXCEPTION_H
/**
 * \file captureexception.h
 * \author	Mathias Meyer <pom@beatsteaks.de>
 *
 * Declares error class for packet capturing
 * engine
 */

#include "defs.h"
#include "exception.h"


DECEPTION_NAMESPACE_BEGIN

// {{{1
/**
 * \class CaptureException
 * 
 * This class is to be used as a generic exception class for all kinds of i/o errors.
 * Use the default implementation or derive your own version from it, like
 * SocketException
 * 
 */
// }}}1
class CaptureException : public Exception
{
	public:
		CaptureException() {}
		CaptureException(std::string _message) : Exception(_message) {}
		CaptureException(int _errno) : Exception(_errno) {}
		// {{{1 DXG DOC
		/**
		 * Return name of current exception.
		 *
		 * \return Name of exception
		 **/
		// }}}1 DXG DOC
		virtual std::string getType() const
		{
			return "CaptureException";
		}
};

DECEPTION_NAMESPACE_END

#endif // _CAPTURE_EXCEPTION_H
