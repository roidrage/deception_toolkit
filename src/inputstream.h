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

#ifndef _INPUTSTREAM_H
#define _INPUTSTREAM_H

/**
 * \file inputstream.h
 *
 * \author Mathias Meyer <pom@beatsteaks.de>
 * 
 * Contains buffer and stream classes for input.
 */

// non-gcc failsafe fallback
#ifndef __GNUC__
#define __GNUC__ 0
#endif

// C++ Headers
#define _IO_THROW 1
#if __GNUC__ == 2
#include <sstream>
#include <istream.h>
#else
#include <streambuf>
#include <istream>
#include <iostream>
#endif

// C Headers
#include <cstdio>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

// Project Headers
#include "defs.h"

DECEPTION_NAMESPACE_BEGIN

class Socket;

// {{{1 DXG DOC
/**
 * \class InputBuffer
 *
 * Base class for input buffering.
 */
// }}}1 DXG DOC
class InputBuffer : public std::streambuf
{ // {{{1 SOURCE
	public:
		InputBuffer() : isInitialized(false), fd(-1)
		{ // CONSTRUCTOR
			setg(this->buffer + 4, this->buffer + 4, this->buffer + 4);
		}
		// {{{2 DXG DOC
		/**
		 * Initialize buffer object with a file descriptor
		 *
		 * \param _fd File descriptor
		 */
		// }}}2 DXG DOC
		void doInit(int _fd)
		{
			if (!isInitialized) {
				this->fd = _fd;
				this->isInitialized = true;
			}
		}
		// {{{2 DXG DOC
		/**
		 * Set timeout values
		 *
		 * \param sec Timeout in seconds
		 * \param msec Timeout in milliseconds, is added to with sec
		 */
		// }}}2 DXG DOC
		void setTimeout(long sec, long msec)
		{
			this->timeOut.tv_sec = sec;
			this->timeOut.tv_usec = msec;
		}
		// {{{2 DXG DOC
		/**
		 * Disable buffer
		 */
		// }}}2 DXG DOC
		void disable()
		{
			setg(0, 0, 0);
		}
	protected:
		bool doSelect();
#if __GNUC__ == 2
		virtual std::stringbuf::int_type underflow();
#else
		virtual std::streambuf::int_type underflow();
#endif
	private:
		bool isInitialized;					///< flag, if buffer is initialized with a file descriptor
		static const int bufferSize = 10;	///< defines buffer size
		char buffer[bufferSize];			///< buffer for use with underflow()
		int fd;								///< file descriptor to use in underflow()
		struct timeval timeOut;				///< timeout values
		InputBuffer(const InputBuffer &buf);
		InputBuffer &operator=(const InputBuffer &buf);
}; // }}}1

// {{{1 BEGIN DXG DOC
/**
 * \class InputStream
 *
 * Class to use for input streams. Use this class an not InputBuffer!
 *
 * \note This class can be used not only for sockets, but for any type of file
 * descriptors. Since sockets happen to use them, this class fits just fine.
 */
// }}}1
class InputStream : public std::istream
{
	private:
		InputBuffer inbuf;					///< input stream buffer
	public:
		InputStream() : std::istream(&inbuf)
		{ // CONSTRUCTOR
		}
		InputStream(int _fd) : std::istream(&inbuf)
		{ // CONSTRUCTOR
		}
		// {{{2 DXG DOC
		/**
		 * Initialize buffer with file descriptor
		 * 
		 * \param _fd File descriptor
		 */
		// }}}2 DXG DOC
		void doInit(int _fd)
		{
			this->inbuf.doInit(_fd);
		}
		// {{{2 DXG DOC
		/**
		 * Set buffer timeout
		 *
		 * \param sec Timeout in seconds
		 * \param msec Timeout in milliseconds, is added to with sec
		 */
		// }}}2 DXG DOC
		void setTimeout(long sec, long msec)
		{
			this->inbuf.setTimeout(sec, msec);
		}
		// {{{2 DXG DOC
		/**
		 * Disable the stream and set the corresponding flags
		 */
		// }}}2 DXG DOC
		void disable()
		{
#if __GNUC__ > 2 || __GNUC__ == 0
			if(this->exceptions() & std::ios_base::badbit) {
				try {
					this->setstate(std::ios_base::badbit | this->rdstate());
				} catch (ios_base::failure &e) {}
				throw;
			} else {
				this->setstate(std::ios_base::badbit | this->rdstate());
			}
#else
			// darn gcc < 3 does not support ios_base::failure,
			// so we only set ios_base::badbit and get the hell
			// outta here
			this->setstate(ios::badbit | this->rdstate());
#endif
			this->inbuf.disable();
		}
	private:
		/**
		 * The copy constructor is hidden in private to make it unusable.
		 */
		InputStream(const InputStream &rhs);
		/**
		 * operator= is hidden in private to make assignment impossible.
		 */
		InputStream &operator=(const InputStream &rhs);
};
DECEPTION_NAMESPACE_END
#endif // _INPUTSTREAM_H
