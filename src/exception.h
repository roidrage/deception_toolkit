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

#ifndef _EXCEPTION_H
#define _EXCEPTION_H
/**
 * \file exception.h
 * \author	Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains basic Exception class. Go derive it!
 * This class should be derived from special exceptions like IOException,
 * SocketException, you name it
 *
 */

#include <errno.h>
#include <string.h>

#include <string>

#include "defs.h"


DECEPTION_NAMESPACE_BEGIN

// {{{1 DXG DOC
/**
 * \class Exception
 * 
 * This is the standard exception class. It contains two basic constructors for
 * use with a message or an errno(3) and a function to fetch the error message
 *
 * \note This class can be used as a general exception or can be derived for
 * use with special exceptions.
 */
// }}}1 DXG DOC
class Exception
{
	private:
		int errNo;				///< keeps current errno for this exception
		std::string message;	///< store a message
	public:
		// {{{2 DXG DOC
		/**
		 * Default constructor. Just initializes errNo and message
		 */
		// }}}2 DXG DOC
		Exception() : errNo(0), message("")
		{ // CONSTRUCTOR
		}
		// {{{2 DXG DOC
		/**
		 * Constructor for error message in form of a string
		 *
		 * \param _message Error message
		 */
		// }}}2 DXG DOC
		Exception(std::string _message) : errNo(0), message(_message) { }
		// {{{2 DXG DOC
		/**
		 * Constructor to be used with an error number from errno
		 *
		 * \param _errno Number of error condition
		 */
		// }}}2 DXG DOC
		Exception(int _errno) : errNo(_errno), message("")
		{ // CONSTRUCTOR
		}
		// {{{2 DXG DOC
		/**
		 * Destructor explicitly resets errno to zero to make sure, we treat errors
		 * as we should. Has been declared virtual, see eff. c++, 1st ed., item 14
		 */
		// }}}2 DXG DOC
		virtual ~Exception()
		{ // DESTRUCTOR
			// reset external errno
			errno = 0;
		}

		// {{{2 DXG DOC
		/**
		 * Return error message of current exception
		 *
		 * \return Error message
		 */
		// }}}2 DXG DOC
		virtual std::string toString() const
		{
			if (this->errNo != 0) {
				return ::strerror(this->errNo);
			} else {
				return this->message;
			}
		}
		
		// {{{2 DXG DOC
		/**
		 * Return name of current exception.
		 *
		 * \return Name of exception
		 **/
		// }}}2 DXG DOC
		virtual std::string getType() const
		{
			return "Exception";
		}
};

DECEPTION_NAMESPACE_END

#endif // _EXCEPTION_H
