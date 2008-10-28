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

#ifndef _SOCKET_H
#define _SOCKET_H
/**
 * \file socket.h
 * 
 * \author Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains class declaration for class Socket
 */

// include c++ header files
#include <string>

#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>

// include custom headers
#include "defs.h"
#include "inputstream.h"
#include "outputstream.h"

#ifdef Darwin
typedef int socklen_t;
#endif

DECEPTION_NAMESPACE_BEGIN

// {{{1 BEGIN DXG DOC
/**
 *	\class Socket
 *
 *	Since C++ doesn't provide basic networking functionality, this is the class
 *	to wrap up all necessary stuff to have an object oriented networking
 *	interface to be used by deceptiond and all plugins
 *
 *	\note
 *	When compiling for solaris we link against libnsl, libsocket and libresolv,
 *	these three define themselves symbols named socket, accept, etc.
 *	We may be able to use the linker option -z muldefs which takes the
 *	first symbol definition and uses that one. But I think it is better
 *	to avoid this symbol clashing and define our own names.
 *
 *	\note This class does _not_ provide any client functions. That has to be
 *	done as part of the module framework, if it is necessary. Obviously missing is a method
 *	to use ::connect(2)
 *
 *	\code
 *	// general usage
 *	try {
 *		Socket sock(80);
 *		sock.init();
 *		while (1) {
 *			try {
 *				sock.accept();
 *				InputStream &inp = sock.getInputStream();
 *				OutputStream &out = sock.getOutputStream();
 *				out << "Hello World" << std::endl;
 *			} catch (NoClientException) {
 *				continue;
 *			}
 *		}
 *	} catch (Exception &e) {
 *		std::cout << "Caught " << e.getType()
 *			<< ": " << e.toString() << std::endl;
 *	}
 *	\endcode
 */
// }}}1
class Socket
{ // {{{1 SOURCE
	public:
		Socket();
		Socket(int _port);
		Socket(std::string _ipaddr, int _port);
		~Socket();
		void init();
		void doAccept();
		void shutDown(int what);
		void close();
		void setSockOpt(int _level, int _optName, void *_optValue, socklen_t _optLength);
		bool isConnected() const;
		bool isListening() const;
		InputStream& getInputStream();
		OutputStream& getOutputStream();
		int getFd() const;
		int getPort() const;
		std::string getIpAddrPort() const;
		void setPort(int _port);
		std::string getClientAddress() const;
		void setTimeout(long sec, long msec)
		{
			this->timeOut.tv_sec = sec;
			this->timeOut.tv_sec = msec;
			this->input.setTimeout(sec, msec);
		}

	private:
		int fd;								///< socket descriptor to be used by all functions
		int clientFd;						///< client descriptor for use with accept and the streams
		bool connected;						///< indicates whether object is connected or not
		bool listening;						///< indicates whether socket is listened on or not
		InputStream input;					///< input stream from client
		OutputStream output;				///< output stream to client
		int port;							///< port to bind to
		struct sockaddr_in serverAddress;	///< structure to keep server data for bind()
		struct sockaddr_in clientAddress;	///< structure to keep client data for accept()
		int backLog;						///< backlog for ::listen(2)
		struct timeval timeOut;				///< input timeout
		std::string servIpAddr;				///< ip address to bind to
		void doSocket();
		void doBind();
		void doListen();
		void setListening(bool _listening);
		void setConnected(bool _connected);
		// hidden to make assignment and copy construction impossible
		Socket &operator=(const Socket &rhs);
		Socket(const Socket &rhs);
}; // }}}1

DECEPTION_NAMESPACE_END
#endif // _SOCKET_H
