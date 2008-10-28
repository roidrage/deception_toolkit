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

#ifndef _OUTPUTSTREAM_H
#define _OUTPUTSTREAM_H
/**
 * \file outputstream.h
 *
 * Contains classes to make up an output stream. OutputBuffer therefore
 * is only a class to be derived by OutputStream. It's not really intended to be
 * used standalone.
 */

// non-gcc failsafe fallback
#ifndef __GNUC__
#define __GNUC__ 0
#endif

// C++ Headers
#include <iostream>

// C Headers
#include <cstdio>
#include <unistd.h>
#include <errno.h>

#if __GNUC__ == 2
//#include <streambuf.h>
#include <sstream>
#else
#include <streambuf>
#endif

// Project Headers
#include "defs.h"


DECEPTION_NAMESPACE_BEGIN

// {{{1 BEGIN DXG DOC
/**
 * \class OutputBuffer
 *
 * Base class for use with OutputStream.
 */
// }}}1
class OutputBuffer : public std::streambuf
{ // {{{1 SOURCE
	private:
		int fd;					///< file descriptor for use in overflow()
		bool initialized;		///< flag, if buffer is initialized
	public:
		OutputBuffer() : fd(-1), initialized(false)
		{ // CONSTRUCTOR
		}
		OutputBuffer(int _fd) : fd(_fd), initialized(false)
		{ // CONSTRUCTOR
		}
		// {{{2 DXG DOC
		/**
		 * Initialize buffer with a file descriptor
		 *
		 * \param _fd File descriptor
		 */
		// }}}2 DXG DOC
		void doInit(int _fd) {
			this->fd = _fd;
			this->initialized = true;
		}
		// {{{2 DXG DOC
		/**
		 * Disable buffer
		 */
		// }}}2 DXG DOC
		void disable()
		{
			this->initialized = false;
		}
		int getFd()
		{
			return this->fd;
		}
	protected:
		// {{{2 DXG DOC
		/**
		 * Overflow function. Derived from std::streambuf. Is called internally. Used
		 * to write one character at a time.
		 *
		 * \param c The character to be printed
		 *
		 * \return Next character
		 */
		// }}}2 DXG DOC
#if __GNUC__ == 2
		virtual std::stringbuf::int_type overflow(std::stringbuf::int_type c)
#else
		virtual std::streambuf::int_type overflow(std::streambuf::int_type c)
#endif
		{
			if (!this->initialized) {
				return EOF;
			}
			if (c != EOF) {
				char z = c;
				if (::write(this->fd, &z, 1) != 1) {
					return EOF;
				}
			}
			return c;
		}
		// {{{2 DXG DOC
		/**
		 * Derived from std::streambuf. Internally called to write multiple characters at a time
		 *
		 * \param *s	String to write to output channel
		 * \param size	Size of string
		 *
		 * \return Number of written characters
		 */
		// }}}2 DXG DOC
		virtual std::streamsize xsputn(const char *s, std::streamsize size)
		{
			if (!this->initialized) {
				return EOF;
			}
			int written, n;
			char *ptr = const_cast<char*>(s);
			written = size;
			while (written > 0) {
				n = ::write(this->fd, ptr, written);
				if (n < 0) {
					// handle errors
					if (errno == EINTR) {
						n = 0;
						continue;
					} else {
						return EOF;
					}
				}
				written -= n;
				ptr += n;
			}
			return size;
		}
	private:
		OutputBuffer(const OutputBuffer &buf);
		OutputBuffer &operator=(const OutputBuffer &buf);
}; // }}}1

// {{{1 DXG DOC
/**
 * \class OutputStream
 *
 * Class for use to output to file descriptors. Has to be initialized either in constructor
 * or with member function init(). Useable for any type of file descriptors, beit sockets or files.
 */
// }}}1 DXG DOC
class OutputStream : public std::ostream
{ // {{{1 SOURCE
	private:
		OutputBuffer outbuf;
	public:
		OutputStream() : std::ostream(&outbuf) {}
		OutputStream(int _fd) : std::ostream(&outbuf), outbuf(_fd) {}
		
		// {{{2 DXG DOC
		/**
		 * Initialize buffer with file descriptor
		 * 
		 * \param _fd File descriptor
		 */
		// }}}2 DXG DOC
		void doInit(int _fd)
		{
			this->outbuf.doInit(_fd);
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
				} catch (std::ios_base::failure &) {}
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
			this->outbuf.disable();
		}
		int getFd()
		{
			return this->outbuf.getFd();
		}
	private:
		// hide copy constructor and assignment operator
		OutputStream &operator=(const OutputStream &rhs);
		OutputStream(const OutputStream &rhs);
}; // }}}1

DECEPTION_NAMESPACE_END
#endif // _OUTPUTSTREAM_H
