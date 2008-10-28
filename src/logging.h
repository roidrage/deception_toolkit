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

#ifndef _LOGGING_H
#define _LOGGING_H

/**
 * \file logging.h
 *
 * Defines class for use to log events. Can be used by both
 * the deceptiond and the modules. See class documentation for
 * further details.
 *
 * \author Mathias Meyer <pom@beatsteaks.de>
 */


// Project Headers
#include "defs.h"
#include "ioexception.h"
#include "logfileexception.h"
#include "exception.h"

// C++ Headers
#include <fstream>
#include <string>
#include <exception>
#include <iostream>

// C Headers
#include <time.h>
#include <sys/types.h>


DECEPTION_NAMESPACE_BEGIN

enum logLevels {Info = 0, Error, FatalError, ModuleInfo, ModuleError, Debug};
static const std::string logTypes[] = {
	"[info]",
	"[error]",
	"[fatalerror]",
	"[mod.info]",
	"[mod.error]",
	"[debug]",
};

// {{{1 DXG DOC
/**
 * \class Logging
 *
 * \todo	Maybe we can allow for logging via syslog.
 *
 * Central logging and debugging facility.
*/
// }}}1 DXG DOC
class Logging
{ // {{{1
	private:
		std::ofstream log;			///< output stream to logfile
		std::string logFileName;	///< filename to log to
	public:
		Logging() { }
		~Logging() { }
		void init();
		void setLogFile(std::string _logFile);
		void toLog(std::string &_moduleName, enum logLevels _logLevel, std::string &_message);
		void toLog(std::string &_moduleName, enum logLevels _logLevel, const char *_message);
		std::string getTime();
	private:
		// hide the usual candidates
		// since we're dealing with system ressources allowing copy construction
		// and assignment is not what we want
		Logging(const Logging& rhs);
		Logging &operator=(const Logging& rhs);
}; // }}}1

DECEPTION_NAMESPACE_END
#endif // _LOGGING_H
