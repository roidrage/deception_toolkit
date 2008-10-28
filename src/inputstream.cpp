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
 * \file inputstream.cpp
 * 
 * \author Mathias Meyer <pom@beatsteaks.de> with help from Nicolai M. Josuttis
 *
 * Contains class implementations for inputstream
 */
#include <unistd.h>
#include <errno.h>

#include <iostream>

#include "inputstream.h"
#include "socketexception.h"
#include "timeoutexception.h"
#include "nullpointerexception.h"

DECEPTION_NAMESPACE_USE

// {{{1 DXG DOC
/**
 * Is called internally in class std::streambuf. Has to be
 * reimplemented to use file descriptors.
 * 
 * \return Number of read characters
 */
// }}}1 DXG DOC
#if __GNUC__ == 2
std::stringbuf::int_type InputBuffer::underflow()
#else
std::streambuf::int_type InputBuffer::underflow()
#endif
{ // {{{1
	if (!this->isInitialized) {
		return EOF;
	}
	
	if (gptr() < egptr()) {
		return *gptr();
	}

	int numPutback;
	int num = 0;
	// set size of putback area
	// at most 4 characters
	numPutback = gptr() - eback();
	if (numPutback > 4) {
		numPutback = 4;
	}
	
	std::memcpy(buffer + (4 - numPutback), gptr() - numPutback, numPutback);
	try {
		if (!this->doSelect()) {
			throw TimeoutException("Client connection timed out");
		} else {
			bool readSuccess = false;
			while (!readSuccess) {
				num = ::read(this->fd, this->buffer + 4, bufferSize - 4);
				if (num <= 0) {
					if (errno == EINTR) {
						continue;
					}
					// error, reset internal buffer
					setg(0, 0, 0);
					return EOF;
				} else {
					readSuccess = true;
				}
			}
			setg(this->buffer + (4 - numPutback), this->buffer + 4, this->buffer + 4 + num);
			return *gptr();
		}
	} catch(TimeoutException &e) {
		// reset internal buffer
		setg(0, 0, 0);
		return EOF;
	}
} // }}}1

// {{{1 DXG DOC
/**
 * Check file descriptor for input. Implements input timeout.
 *
 * \retval true  if input is waiting
 * \retval false  if timeout exceeded or in case of other errors
 */
// }}}1 DXG DOC
bool InputBuffer::doSelect()
{
	fd_set ds;
	FD_ZERO(&ds);
	FD_SET(this->fd, &ds);
	int maxFd = this->fd + 1;
	struct timeval tempT = this->timeOut;
	int rFd = ::select(maxFd, &ds, 0, 0, &tempT);
	if (rFd < 0) {
		throw SocketException(errno);
	} else {
		return (rFd > 0);
	}
}
