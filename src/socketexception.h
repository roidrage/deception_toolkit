#ifndef _SOCKETEXCEPTION_H
#define _SOCKETEXCEPTION_H
/**
 * \file socketexception.h
 * \author	Mathias Meyer <pom@beatsteaks.de>
 * 
 * Contains declaration of generic SocketException class.
 */

#include <string>

#include "defs.h"
#include "ioexception.h"


DECEPTION_NAMESPACE_BEGIN

// {{{1
/**
 * \class SocketException
 *
 * This class represents a generic socket error handler.
 *
 * \note This class generally is to be used for all possible socket errors
 * except error when calling ::bind(2)
 */
// }}}1
class SocketException : public IOException
{
	public:
		SocketException() {}
		SocketException(std::string _message) : IOException(_message) {}
		SocketException(int _errno) : IOException(_errno) {}
		// {{{1 DXG DOC
		/**
		 * Return name of current exception.
		 *
		 * \return Name of exception
		 **/
		// }}}1 DXG DOC
		virtual std::string getType() const
		{
			return "SocketException";
		}
};

DECEPTION_NAMESPACE_END

#endif // _SOCKETEXCEPTION_H
