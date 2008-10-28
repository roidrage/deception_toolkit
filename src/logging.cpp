/**
 * \file logging.cpp
 * 
 * \author Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains class implementations for logging
 */
#include "logging.h"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

// {{{1 DXG DOC
/**
 * Small function to set filename to log to.
 *
 * \note This has no affect, if logfile is already opened
 *
 * \param _logFile Filename to log to
 */
// }}}1 DXG DOC
void Deception::Logging::setLogFile(std::string _logFile)
{ // {{{1
	if (this->log.is_open()) {
		return;
	} else {
		this->logFileName = _logFile;
	}
} // }}}1

// {{{1 DXG DOC
/**
 * Initialize output file stream
 *
 * \note This function must be called before any logging happens.
 * Otherwise log messages will be put out to stderr
 *
 * \exception LogFileException Erroneous logfile
 */
// }}}1 DXG DOC
void Deception::Logging::init()
{ // {{{1
	// open logfile
	// write mode is to always append at the end of file
	if ((this->logFileName.length() > 0) && !this->log.is_open()) {
		try {
			this->log.open(this->logFileName.c_str(), std::ios::app);
			// is our stream a bad boy?
			if (this->log.fail()) {
				// then throw it right out
				throw LogFileException("erroneous output stream to logfile!");
			}
		} catch (const std::exception& e) {
			// we'll just catch stl exceptions and pretend that we've thrown our own
			throw LogFileException(std::string(e.what()));
		}
	} else {
		throw LogFileException("empty logfile name or log already opened");
	}
} // }}}1

// {{{1 DXG DOC
/**
 * Log a message. Logging string looks like this:
 * 05.04.2003 18:08:48 [module]: sent /etc/passwd to erika@mustermann.de
 *
 * \param _moduleName Name of module that logs a message
 * \param _logLevel Loglevel
 * \param _message Logmessage
 */
// }}}1 DXG DOC
void Deception::Logging::toLog(std::string &_moduleName, enum logLevels _logLevel, std::string &_message)
{ // {{{1

	std::string logType, logTime;
	// fetch time
	// FIXME: make time format configurable
	// FIXME: replace with stuff from <ctime>?
	int logLevelLen = sizeof(logTypes) / sizeof(char*);
	if ((_logLevel < 0) || (_logLevel > logLevelLen)) {
		return;
	}
	logType = logTypes[_logLevel];
	logTime = this->getTime();
	if (logTime.empty()) {
		// if we don't have a time, for now just set it to -
		logTime = "-";
	}
	// debug and fatal errors go right out to stderr
	// in case logfile isn't opened, dump all stuff to stderr
	if ((_logLevel == Debug) || (_logLevel == FatalError) || !this->log.is_open()) {
		std::cerr << logTime << " " << logType << " " << _moduleName
			<< " (" << getpid() << "): " << _message << std::endl;
	} else {
	// logging stuff goes right out to logfile
		this->log << logTime << " " << logType << " " << _moduleName
			<< " (" << getpid() << "): " << _message << std::endl;
	}
} // }}}1

// {{{1 DXG DOC
/**
 * Overloaded function for log messages of type const char*
 * 
 * \param _moduleName Name of module that logs a message
 * \param _logLevel Loglevel
 * \param _message Logmessage
 *
 * \overload void Deception::Logging::toLog(std::string &_moduleName, enum logLevels _logLevel, std::string &_message)
 */
// }}}1 DXG DOC
void Deception::Logging::toLog(std::string &_moduleName, enum logLevels _logLevel, const char *_message) {
	std::string message(_message);
	this->toLog(_moduleName, _logLevel, message);
}

// {{{1 DXG DOC
/**
 * Fetch current time
 *
 * \return Current time
 */
// }}}1 DXG DOC
std::string Deception::Logging::getTime()
{ // {{{1
	time_t pNow;
	time_t now;
	struct tm *lTime;
	char pLogT[30];
	size_t len;
	// fetch time
	// FIXME: make time format configurable
	// FIXME: replace with stuff from <ctime>?
	// FIXME: make pLogT static array either dynamic or reasonable in size.
	// FIXME: make pLogT static array either dynamic or reasonable in size.

	if ((now = ::time(&pNow)) == static_cast<time_t>(-1))
		return std::string("");

	if ((lTime = ::localtime(&pNow)) == NULL)
		return std::string("");

	if ((len = ::strftime(pLogT, 30, "%d.%m.%Y %H:%M:%S", lTime)) != 0)
		return std::string(pLogT);

	return std::string("");
} // }}}1

Deception::Logging globLog;
