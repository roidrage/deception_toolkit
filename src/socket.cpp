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
 * \file socket.cpp
 * 
 * \author Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains implementation for class Socket
 */
#include "socket.h"
// include c++ headers
#include <iostream>

// include c header files
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
//#include <error.h>
#include <strings.h> 	// needed for SunOS bzero
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

// include exceptions
#include "bindexception.h"
#include "socketexception.h"
#include "noclientexception.h"
#include "nullpointerexception.h"
#include "illegalargumentexception.h"

DECEPTION_NAMESPACE_BEGIN

// {{{1
/**
 * Default constructor, that's it. Initializes timeout structure.
 *
 */
// }}}1
Socket::Socket() : connected(false), listening(false), port(-1), servIpAddr("")
{ // DEFAULT CONSTRUCTOR
	this->timeOut.tv_sec = 10;
	this->timeOut.tv_usec = 0;
}

// {{{1
/**
 * Constructor to use in factory
 *
 * \param _port Port number to bind to
 */
// }}}1
Socket::Socket(int _port) : connected(false), listening(false), port(_port), servIpAddr("")
{ // CONSTRUCTOR
	this->timeOut.tv_sec = 10;
	this->timeOut.tv_usec = 0;
}
//
// {{{1
/**
 * Constructor to use in factory
 *
 * \param _ipaddr IP address to bind to
 * \param _port Port number to bind to
 */
// }}}1
Socket::Socket(std::string _ipaddr, int _port) : connected(false), listening(false), port(_port), servIpAddr(_ipaddr)
{ // CONSTRUCTOR
	this->timeOut.tv_sec = 10;
	this->timeOut.tv_usec = 0;
}

// {{{1
/**
 * Socket destructor. Closes connection.
 */
// }}}1
Socket::~Socket()
{ // DESTRUCTOR
	// close connections if still connected
	::close(this->clientFd);
	::close(this->fd);
}

// {{{1
/**
 * Init current Object. Here socket, bind and listen are being called to create
 * us a nice and healthy Socket object
 *
 * \retval true if all went great
 * \retval false if something went wrong
 * 
 * \exception SocketException
 * \exception IllegalArgumentException
 */
// }}}1
void Socket::init()
{ // {{{1
	// check if we are already connected
	if(this->listening) {
		return;
	} else {
		// let's go for it
		// port correct?
		if(this->port < 0) {
			throw IllegalArgumentException("No port specified");
		} else {
			// using ::bzero, ::htonl and ::htons will err out on sun spro
			bzero(&this->serverAddress, sizeof(this->serverAddress));
			this->serverAddress.sin_family = AF_INET;

			// this can be used for some pseude virtual hosting
			if (!this->servIpAddr.empty()) {
				if (inet_aton(this->servIpAddr.c_str(), &this->serverAddress.sin_addr) == 0)
					// handle error correctly!
					throw SocketException("Could not convert IP Address");
			} else {
				// in case no ip has been config'd for the host
				// we assume it should listen on all addrs.
				this->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
			}
			this->serverAddress.sin_port = htons(this->port);
			this->doSocket();
			this->doBind();
			this->doListen();
		}
	}
} // }}}1

// {{{1
/**
 * Accept an incoming client connection. Internally calls ::accept(2)
 *
 * \exception NoClientException
 */
// }}}1
void Socket::doAccept()
{
	// try to accept from client
	unsigned int size = sizeof(this->clientAddress);
	if ((this->clientFd = ::accept(this->fd, reinterpret_cast<struct sockaddr*>(&this->clientAddress), &size)) == -1) {
		// did something go wrong?
		// then throw it right out
		throw NoClientException();
	} else {
		// otherwise 
		this->input.doInit(this->clientFd);
		this->input.setTimeout(this->timeOut.tv_sec, this->timeOut.tv_usec);
		this->output.doInit(this->clientFd);
		this->connected = true;
	}
	// create streams
	// set flags
}

// {{{1
/**
 * Shut down the transmission or the reception part of a socket or both.
 * Internally calls shutdown(2)
 *
 * \param what Takes SHUT_RD, SHUT_WR or SHUT_RDWR as an argument
 * 
 * \exception SocketException
 */
// }}}1
void Socket::shutDown(int what)
{
	// FIXME: throw exception?
	if(!this->connected) {
		return;
	} else {
		if(::shutdown(this->clientFd, what) == -1) {
			throw SocketException();
		} else {
			if ((what == SHUT_RD) || (what == SHUT_RDWR)) {
				this->input.disable();
			}
			if ((what == SHUT_WR) || (what == SHUT_RDWR)) {
				this->output.disable();
			}
		}
	}
}

// {{{1
/**
 * Check whether socket is connected or not
 *
 * \retval true if socket is connected
 * \retval false if it is not
 */
// }}}1
bool Socket::isConnected() const
{
	return this->connected;
}

// {{{1
/**
 * Fetch the socket's file descriptor
 *
 * \note	making these methods inline won't allow for code similar to
 * 			this: \code
 * 			Socket *sokobj = new Socket(port);
 * 			sockobj->init();
 * 			sockobj->getFg();
 * 			\endcode
 * 			haven't done further research why. here's the errmsg:
 * 			\code
 *			test/modconftestmain.o(.text+0x669): In function
 *			`std::_Rb_tree_base_iterator::_M_increment()':
 			/usr/include/c++/3.2/iostream: undefined reference to
			`Deception::Socket::isConnected() const'
			collect2: ld returned 1 exit status
			make: *** [modconftestmain] Error 1
			\endcode

 *
 * 		
 *
 * \return File descriptor
 */
// }}}1
int Socket::getFd() const
{
	return this->fd;
}

// {{{1
/**
 * Fetch the port, the object is connected to
 *
 * \note	making these methods inline won't allow for code similar to
 * 			this: \code
 * 			Socket *sockobj = new Socket(port);
 * 			sockobj->init();
 * 			sockobj->getPort();
 * 			\endcode
 * 			haven't done further research why. afh.
 * 
 * \return Bound port
 */
// }}}1
int Socket::getPort() const
{
	return this->port;
}

// {{{1
/**
 * Fetch the port, the object is connected to
 *
 * \note	making these methods inline won't allow for code similar to
 * 			this: \code
 * 			Socket *sokobj = new Socket(port);
 * 			sockobj->init();
 * 			sockobj->getPort();
 * 			\endcode
 * 			haven't done further research why. afh.
 * 
 * \return bound ip and port
 */
// }}}1
std::string Socket::getIpAddrPort() const
{
	char strPort[6];
	sprintf(strPort, "%d", this->port);
	return this->servIpAddr + ':' + strPort;
}

// {{{1
/**
 * Fetch the client's IP address
 *
 * \return client's ip as string
 *
 */
// }}}1
std::string Socket::getClientAddress() const
{
	char aClAddr[INET_ADDRSTRLEN];
	const char *ptr = ::inet_ntop(AF_INET, &(this->clientAddress).sin_addr, aClAddr, sizeof(aClAddr));
	return std::string(const_cast<char*>(ptr));
}

// {{{1
/**
 * Creates a socket for use with the rest of the member functions
 * and set some necessary socket options
 *
 * \exception SocketException
 */
// }}}1
void Socket::doSocket()
{
	if (this->listening)
		throw SocketException("Socket already in use");

	// FIXME: create attributes for domain, type and protocol
	if ((this->fd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw SocketException(errno);

#ifndef Darwin
	// set socket options for virtual hosts
	int yes = 1;
	int sockopts = 0;
#ifdef SO_REUSEPORT
	sockopts |= SO_REUSEPORT;
#endif
#if defined(SO_REUSEADDR) && !defined(__FreeBSD__)
	sockopts |= SO_REUSEADDR;
#endif
	if (::setsockopt(this->fd, SOL_SOCKET, sockopts, &yes, sizeof(yes)) == -1) {
		throw SocketException(errno);
	}
#endif
}

// {{{1
/**
 * Binds to given port and ip address
 *
 * \exception BindException
 */
// }}}1
void Socket::doBind()
{
	// FIXME: check if object is already bound
	if (this->listening) {
		throw BindException("Socket already in use");
	} else {
		// try to bind to port
		if (::bind(this->fd, reinterpret_cast<struct sockaddr*>(&this->serverAddress), sizeof(this->serverAddress)) == -1) {
			throw BindException(errno);
		}
	}
}

// {{{1
/**
 * Listens on socket descriptor
 *
 * \exception SocketException
 */
// }}}1
void Socket::doListen()
{
	if (this->listening) {
		throw SocketException("Socket already listening");
	} else {
		if (::listen(this->fd, this->backLog) == -1) {
			throw SocketException(errno);
		} else {
			this->listening = true;
		}
	}
}

// {{{1
/**
 * Set listening status flag
 *
 * \param _listening True or false to indicate if is listening or not
 */
// }}}1
inline void Socket::setListening(bool _listening)
{
	this->listening = _listening;
}

// {{{1
/**
 * Fetch listening status flag
 *
 * \retval true If object is listening on socket
 * \retval false If it's not
 */
// }}}1
inline bool Socket::isListening() const
{
	return this->listening;
}

// {{{1
/**
 * Set connected status flag
 *
 * \param _connected Indicates, if object should be connected or not
 *
 * \todo decide, if this function should also close the connection
 */
// }}}1
inline void Socket::setConnected(bool _connected)
{
	this->connected = _connected;
}

// {{{1
/**
 * Return inputstream of current socket.
 *
 * \return Reference to object of type InputStream
 */
// }}}1
InputStream& Socket::getInputStream()
{
	return this->input;
}

// {{{1
/**
 * Return output stream of current socket object.
 *
 * \return Reference to object of type OutputStream
 *
 */
// }}}1
OutputStream& Socket::getOutputStream()
{
	return this->output;
}

// {{{1
/**
 * Set port
 *
 * \param _fd Port number
 *
 * \exception IllegalArgumentException
 */
// }}}1
void Socket::setPort(int _fd)
{
	if ((_fd < 0) || (_fd > 65535)) {
		throw IllegalArgumentException("Port already defined");
	} else {
		this->fd = _fd;
	}
}

// {{{1
/**
 * Close client descriptor
 *
 * \exception SocketException
 */
// }}}1
void Socket::close()
{
	if (::close(this->clientFd) == -1) {
		if (errno == EBADF) {
			return;
		} else {
			throw SocketException(errno);
		}
	}
}

// {{{1
/**
 * Set socket option. equivalent to ::setsockopt()
 *
 * \param _level		Socket level
 * \param _optName		Name of option to set
 * \param *_optValue	Value to set
 * \param _optLength	Length of option
 *
 * \exception SocketException
 */
void Socket::setSockOpt(int _level, int _optName, void *_optValue, socklen_t _optLength)
{
	if (::setsockopt(this->fd, _level, _optName, _optValue, _optLength) == -1) {
		throw SocketException(errno);
	}
}

DECEPTION_NAMESPACE_END
