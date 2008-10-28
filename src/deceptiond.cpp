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
 * \file    deceptiond.cpp
 * \author  alexis hildebrandt <afh@2drop.net>
 * \author	Mathias Meyer <pom@beatsteaks.de>
 *
 * This file contains the main entry point for the deception daemon and some
 * functions it depends on.
 */


// C Headers
#ifdef DO_MCHECK
#include <mcheck.h>
#endif

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>

// Project Headers
#include "deceptiond.h"
#include "moduleconfig.h"
#include "socket.h"
#include "logging.h"
#include "signals.h"
#include "fw_pcap.h"

#include "exception.h"
#include "bindexception.h"
#include "socketexception.h"
#include "noclientexception.h"
#include "nullpointerexception.h"
#include "captureexception.h"

// global logging object
extern Deception::Logging globLog;

// global options
// from config file - user name for child processes
std::string runUser;
// from config file - group name for child processes
std::string runGroup;
// from config file - capture device
std::string capDevice;
// from config file - flag to en-/disable capturing
bool enableCapture = false;
// for daemonize()
int fdIn, fdOut;
// child id of capture engine
pid_t capChld;

// program stuff
// logging name
std::string logName = "deceptiond";
// version
const float progVersion = 0.3;
// program name
const char* progName = "deceptiond";
// authors
const char* progAuthors = "alexis hildebrandt <afh@2drop.net>, mathias meyer <pom@beatsteaks.de>\n";
// homepage
const char* progHome = "home: http://unit22.in-berlin.de/studies/diploma/\n";

// {{{1 DXG DOC
/**
 * Prints out infomartion about command line switches
 **/
// }}}1 DXG DOC
void printHelp()
{
	std::cout << progName << " v" << progVersion << "\nauthors: "
		<< progAuthors << progHome << "\nsupported options:\n"
		<< "-c\tpath to config file\n"
		<< "-D\tswitch to daemon mode (program will run in background)\n"
		<< "-h\tprint out this information\n" << std::endl;

	exit(EXIT_SUCCESS);
}

// {{{1 DXG DOC
/**
 * Send process to background and send stdin and stdout to /dev/null
 **/
// }}}1 DXG DOC
void daemonize()
{
	pid_t pid = ::fork();
	switch (pid) {
		case 0:
			// child
			// create new process group
			::setsid();
			::umask(0);
			// dup stdin, stderr and stdout to /dev/null
			// we only leave stderr active, since that will be the place
			// to put error information
			if ((fdIn = ::open("/dev/null", O_RDWR)) == -1) {
				throw Deception::Exception(errno);
			}
			if ((fdOut = ::open("/dev/null", O_RDWR)) == -1) {
				throw Deception::Exception(errno);
			}
			if (::dup2(fdIn, STDIN_FILENO) == -1) {
				throw Deception::Exception(errno);
			}
			close(fdIn);
			if (::dup2(fdOut, STDOUT_FILENO) == -1) {
				throw Deception::Exception(errno);
			}
			::close(fdOut);
			break;
		case -1:
			// error
			throw Deception::Exception(errno);
			break;
		default:
			// parent just exits and is detached from terminal
			::exit(0);
	}
}

// {{{1 DXG DOC
/**
 * Checks, if a given filename is really a file
 * or a symbolic link and if current user is allowed to
 * read it.
 *
 * \param &_fileName	Filename to be checked
 **/
// }}}1 DXG DOC
void checkFile(std::string &_fileName)
{
	struct stat stBuf;
	// get stat structure of file,dir,link,etc.
	if (::stat(_fileName.c_str(), &stBuf) == -1) {
		throw Deception::IOException(errno);
	}
	// we only accept regular files or a symlink
	if (!(S_ISREG(stBuf.st_mode) || (S_ISLNK(stBuf.st_mode)))) {
		throw Deception::IOException("not a file or symbolic link");
	}
	// check if it's readable
	if (stBuf.st_mode & S_IRUSR) {
		// if it is, we're done
		return;
	} else {
		// throw access denied error
		throw Deception::IOException(EACCES);
	}
}

int main(int argc, char **argv)
{
#ifdef DO_MCHECK
	mcheck(NULL);
	mtrace();
#endif
	// check command line arguments
	if (argc == 1) {
		printHelp();
		return(0);
	}
	// define & init vars
	// configuration object
	Deception::Config mc;
	// module registry for module data
	Deception::ModuleRegistry mr;
	// module loader (to load modules, obviously)
	Deception::ModuleLoader ml;
	// socket object to be used in initialization and client
	// handling
	Deception::Socket *sockobj;
	// pointer to module class
	Deception::Module *mod;
	// pid for fork()
	pid_t child;
	// user id for child processes
	uid_t runUid = ::getuid();
	// group id for child processes
	gid_t runGid = ::getgid();
	// structure to fetch user id
	struct passwd *userData;
	// structure to fetch group id
	struct group *groupData;
	// generic variable for log messages
	std::string logMsg;
	// filename for configuration
	std::string configFile;
	// option string for configuration stuff
	std::string optionstring;
	std::string modAddress;
	// strings to search in ModuleRegistry
	std::string ipaddrport, ipaddr;
	// specially for select() call
	std::string strError;
	// flag, if deceptiond should become a daemon, false by default
	bool doDaemonize = false;
	// variable to fetch getopt stuff from command line
	char opt;
	// set our nice signal handler
	// no more zombies in here
	try {
		setSigHandler(SIGCHLD, chldHandler);
	} catch (Deception::Exception &e) {
		std::cerr << "error while setting signal handler: " << e.toString();
		::exit(EXIT_FAILURE);
	}

	// we don't want getopt() to print errors
	opterr = 0;
	// fetch commandline option for path to config file
	while((opt = ::getopt(argc, argv, "c:Dh")) > 0) {
		switch (opt) {
			case 'c':
				configFile = optarg;
				// check if there is really filename given on command line
				// and not another option
				if ((configFile.compare("-D") == 0) || (configFile.compare("-h") == 0)) {
					std::cerr << "[error] no config file given\n\n";
					printHelp();
					::exit(EXIT_FAILURE);
				}
				try {
					// check file access
					checkFile(configFile);
				} catch (Deception::IOException &e) {
					std::cerr << "[error] could not open config file: " << e.toString() << "\n";
					::exit(EXIT_FAILURE);
				}
				break;
			case 'D':
				doDaemonize = true;
				break;
			case 'h':
				// we only print this help, if -h was the only
				// command line option
				if (argc == 2) {
					printHelp();
				}
				break;
		}
	}
	// check if we have been given a config file on command line
	if (configFile.length() > 0) {
		// if so, then read config
		mc.read(configFile);
	} else {
		// elsewise we print an error and get the hell outta here
		std::cerr << "[error] no config file given\n\n";
		printHelp();
		::exit(EXIT_FAILURE);
	}
	// if we have to, become a daemon
	if(doDaemonize) {
		try {
			daemonize();
		} catch (Deception::Exception &e) {
			std::cerr << "could to daemonize: " << e.toString() << "\n";
			::exit(EXIT_FAILURE);
		}
	}

	// set up logging facility we need to do the configuration reading
	// beforehand, since the configuration file specifies the file we
	// should log to.
	// XXX: no logging can take place before this line, because we can't guarantee,
	// that the log file is open
	try {
		globLog.init();
	} catch (Deception::LogFileException &e) {
		std::cerr << "error in logfile init: " << e.toString() << std::endl;
	}

	// write startup message
	logMsg = "deceptiond starting";
	globLog.toLog(logName, Deception::Info, logMsg);

	// get user id
	if (runUser.length() > 0) {
		if ((userData = ::getpwnam(runUser.c_str())) == NULL) {
			logMsg = "didn't find user in /etc/passwd";
			globLog.toLog(logName, Deception::Error, logMsg);
			::exit(EXIT_FAILURE);
		} else {
			runUid = userData->pw_uid;
		}
	}
	// get group id
	if (runGroup.length() > 0) {
		if ((groupData = ::getgrnam(runGroup.c_str())) == NULL) {
			logMsg = "didn't find group in /etc/group";
			globLog.toLog(logName, Deception::Error, logMsg);
			exit(EXIT_FAILURE);
		} else {
			runGid = groupData->gr_gid;
		}
	}

	// load modules
	ml.loadAllModules();

	// fetch iterators for ModuleRegistry
	Deception::ModuleRegistry::ModuleRegistryMapIterator itBegin = mr.begin();
	Deception::ModuleRegistry::ModuleRegistryMapIterator itEnd = mr.end();
	fd_set sockFdSet, selectSet;
	FD_ZERO(&sockFdSet);
	int maxfd = 0;

	// create a socket for every port and put the sockobj into the registry
	for (; itBegin != itEnd; itBegin++) {
		ipaddrport = (*itBegin).first;
		ipaddr = mr.getIpAddr(ipaddrport);
		unsigned int port = mr.getPort(ipaddrport);

		logMsg = "creating socket for " + ipaddrport;
		globLog.toLog(logName, Deception::Info, logMsg);

		// this is okay, where not losing the pointer to the new()'ed
		// memory, since its address is stored in the static registry
		// member of ModuleRegistry::mr !
		sockobj = new Deception::Socket(ipaddr, port);		//vhosts
		try {
			sockobj->init();
		}
		catch (Deception::Exception &e) {
			logMsg = e.getType() + "error in socket init: " + e.toString();
			globLog.toLog(logName, Deception::FatalError, logMsg);
			::exit(EXIT_FAILURE);
		}
		maxfd = sockobj->getFd();
		FD_SET(maxfd, &sockFdSet);
		mr.insert(ipaddr, port, sockobj);
	}
	sockobj = NULL;

	// and now for the capturing stuff
	// first fork a new process
	if (enableCapture) {
		try {
			switch (child = ::fork()) {
				case 0:
					// child
					// call capturing routine*/
					try {
						capture(mr, capDevice);
					} catch (Deception::CaptureException &e) {
						logMsg = "error in capture engine: " + e.toString();
						globLog.toLog(logName, Deception::Error, logMsg);
					}
					return 0;
					break;
				case -1:
					// error
					throw Deception::Exception(errno);
					break;
				default:
					// parent
					// save childish process id, in case of reload
					capChld = child;
					break;
			}
		} catch (Deception::Exception &e) {
			logMsg = "error while initializing packet capture engine: " + e.toString() + "\n";
			globLog.toLog(logName, Deception::FatalError, logMsg);
			::exit(EXIT_FAILURE);
		}
	}

	struct timeval timeout = { 0, 0 };
	// main event-loop
	for(int i = 0; ; i++) {
		// NOTE:
		// timeout and selectSet are modified by select() and therefore
		// need to be reset before another call to select()!
		timeout.tv_usec = 100000;
		selectSet = sockFdSet;

		int n = ::select(maxfd + 1, &selectSet, NULL, NULL, &timeout);
		if (n == -1) {
			// interrupted system call
			// pops up, if a child exits
			if (errno != EINTR) {
				strError = ::strerror(errno);
				logMsg = "select error: " + strError;
				globLog.toLog(logName, Deception::FatalError, logMsg);
				::exit(EXIT_FAILURE);
			}
			continue;
		}

		if (n == 0)
			// no file descriptpors ready timeout triggered
			continue;

		// test which fd's are ready 
		Deception::ModuleRegistry::ModuleRegistryMapIterator itBegin = mr.begin();
		Deception::ModuleRegistry::ModuleRegistryMapIterator itEnd = mr.end();
		for (; itBegin != itEnd; itBegin++) {
			sockobj = (*itBegin).second->getSocket();

			if (FD_ISSET(sockobj->getFd(), &selectSet) == 0)
				continue;

			try {
				sockobj->doAccept();
				logMsg = "client " + sockobj->getClientAddress() + " has connected";
				globLog.toLog(logName, Deception::Info, logMsg);
				child = ::fork();
				if (child == -1) {
					// Error
					logMsg.erase();
					logMsg.append("error during fork(): ").append(strerror(errno));
					globLog.toLog(logName, Deception::Error, logMsg);
					continue;
				}
				if (child > 0) {
					// Parent
					if(sockobj->isConnected()) {
						sockobj->close(); // close connection
					}
				} else {
					// Child (Module)
					// TODO: child cleanup

					// now loose those unnecessary privileges
					// first group privileges, since if we would loose them after loosing the
					// user privileges. we get EPERM, since we're not the superuser anymore
					if (runGid != getgid()) {
						if (::setregid(runGid, runGid) == -1) {
							if (errno == EPERM) {
								globLog.toLog(logName, Deception::Error, "not able to change real and eff. group id");
							}
						}
					}
					if (runUid != getuid()) {
						if (::setreuid(runUid, runUid) == -1) {
							if (errno == EPERM) {
								globLog.toLog(logName, Deception::Error, "not able to change real and eff. user id");
							}
						}
					}

					Deception::ModuleFactoryBase *fb = NULL;
					mod = NULL;
					optionstring = "";
					modAddress = sockobj->getIpAddrPort();

					// find by portNum

					fb = mr.find(modAddress);
					if (fb == NULL) {
						logMsg = "factory not found for " + modAddress;
						globLog.toLog(logName, Deception::Error, logMsg);
						::exit(EXIT_FAILURE);
					}

					mod = fb->createModObject();
					if (mod == NULL) {
						logMsg = "module not found for " + modAddress;
						globLog.toLog(logName, Deception::Error, logMsg);
						::exit(EXIT_FAILURE);
					}

					optionstring = mr.getOption(modAddress);

					// run module
					mod->modMain(sockobj, optionstring);

					// close connection
					if(sockobj->isConnected()) {
#ifdef DEBUG
						globLog.toLog(logName, Deception::Debug, "child closing connection");
#endif
						sockobj->close();
					}
					return 0;
				}
			} catch (Deception::NoClientException &e) {
				continue;
			} catch (Deception::Exception &e) {
				logMsg = "error in module spawning code: " + e.toString();
				globLog.toLog(logName, Deception::Error, logMsg);
			} catch (std::exception &e) {
				logMsg.erase();
				logMsg.append("error in standard library ").append(e.what());
				globLog.toLog(logName, Deception::Error, logMsg);
			}
		} // end iterating socket for-loop

	}// end for(;;) main-loop


#ifdef DO_MCHECK
	muntrace();
#endif
	// never reached.
	return 0;
}
