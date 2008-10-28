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
 * \file    dtk-scriptfsm.cpp
 * \brief	implements methods in class DtkScriptFSM
 * \author  alexis hildebrandt <afh@2drop.net>
 *
 * This file implements the methods for class for DtkScriptFSM.
 *
 */

// Module Headers
#include "dtk-scriptfsm.h"


DECEPTION_NAMESPACE_USE;
DTKSCRIPT_NAMESPACE_USE;

// define static variables
DtkScriptFSM *DtkScriptFSM::states[STATECOUNT] = { NULL };
unsigned int DtkScriptFSM::curState = 0;
unsigned int DtkScriptFSM::curLoop = 0;
unsigned int DtkScriptFSM::confMaxLoops = 65535;
unsigned int DtkScriptFSM::confTimeout = 0;
unsigned int DtkScriptFSM::confDelay = 0;
std::string  DtkScriptFSM::confDir = "";
std::string  DtkScriptFSM::confFile = "";
bool DtkScriptFSM::debug = false;

//{{{1 DXG DOC
/**
 * \brief   change the state of the state machine.
 *  
 * changes the State of the state machine and calls the start script
 * for that state and notifies via the logging facility.
 *
 * \todo	log notification needs to include the new state number
 *  
 * \return  void
 * \retval  none
 *
 * \param   stateNum	the state to go over to
 */
//}}}1 DXG DOC
void DtkScriptFSM::changeState(unsigned int stateNum)
{
	this->curState = stateNum;
	this->states[stateNum]->dtkSpecial("NOTICE");

	// write notification to the logging mechanism
	char c[2];
	snprintf(c, 2, "%d", stateNum);
	std::string logMsg = this->confFile + " S" + c;
	globLog.toLog(moduleName, ModuleInfo, logMsg);

	return;
}

//{{{1 DXG DOC
/**
 * \brief   respond according to a dtk special command
 *  
 * Each state should define several special commands. These are called
 * upon entering the specific state or when a simulated error occurs 
 * for example.  Currently START, ERROR, NIL, NOTICE and ! are supported.
 *
 * \todo	NIL does not work as expected. which also brings up the
 * 			question: "what is the intended operation of NIL ?"
 *  
 * \return  void
 * \retval  none
 *
 * \param   command	one of the special dtk commands
 */
//}}}1 DXG DOC
void DtkScriptFSM::dtkSpecial(std::string command)
{
	std::string logMsg;
	// NOTICE DTK command needs special treatment, since it is supposed
	// to exec a configured script with configured parameters
	if (command.compare("NOTICE") == 0) {
		StateTransitionData *entry = this->matchDtk[command];
		if (entry == NULL)
			return;

		if (entry->notice.empty() == true)
			return;

		logMsg = "Notice: " + entry->notice + ": " + entry->response;
		globLog.toLog(moduleName, ModuleInfo, logMsg);

		pid_t child = ::fork();
		if (child == -1) {
			// Error
			globLog.toLog(moduleName, ModuleError, strerror(errno));
		} else
		if (child > 0) {
			// Parent
			// FIXME: react to errors of waitpid.
			int status;
			::waitpid(child, &status, 0);
		} else {
			// Child
			char *(args[]) = {
				const_cast<char *>(entry->notice.c_str()),
				const_cast<char *>(entry->response.c_str()),
				NULL,
			};

			::execve(entry->notice.c_str(), args, NULL);
			exit (EXIT_SUCCESS);
		}

		return;
	}


	// all other DTK commands can be handled through this->respond
	if (this->matchDtk.find(command) != this->matchDtk.end())
		this->respond(this->matchDtk[command]);

	return;
}

//{{{1 DXG DOC
/**
 * \brief   dummy method.
 *  
 * This method is the actual output method, that sends data to the
 * client. Depending on the operation that was configured for this
 * state and transition the data is sent in different ways.
 *
 * \todo	currently the infocon and special operation are unsupported
 * \todo	currently the -echo and +echo operation does only work for ttys
 *  
 * \return  void
 * \retval  none
 *
 * \param   *entry	pointer to the object
 * 			that stores the transition data.
 */
//}}}1 DXG DOC
void DtkScriptFSM::respond(StateTransitionData *entry)
{
	// wait with the respond and simulate a busy machine, slow network
	// connection (tarpit).
	if (this->confDelay > 0)
		sleep(this->confDelay);

	// parse operation field
	if (entry->operation.compare("infocon") == 0) {
		 // FIXME: infocon not supported
		std::string logMsg = "unsupported infocon operation requested!";
		globLog.toLog(moduleName, Error, logMsg);
	} else
	if (entry->operation.compare("special") == 0) {
		// FIXME: special not supported
		std::string logMsg = "unsupported special operation requested!";
		globLog.toLog(moduleName, Error, logMsg);
	} else
	if (entry->operation.compare("-echo") == 0) {
		// switch echo off
		this->setEcho(ECHOOFF);
		this->streamOut << entry->response ; //<< "\r\n"; //<< std::endl;
	} else 
	if (entry->operation.compare("+echo") == 0) {
		// switch echo on
		this->setEcho(ECHOON);
		this->streamOut << entry->response ; //<< "\r\n"; //<< std::endl;
	} else 
	if (entry->operation.compare("exec") == 0)
		// run binary
		this->doExec(entry);
	else
	if ((entry->operation.compare("cat") == 0)
		|| (entry->operation.compare("2") == 0))
		this->doCat(entry);
	else
	if (entry->operation.compare("@") == 0)
		// start the fsm with a new config
		this->init(this->confDir, entry->response);
	else
	if (entry->operation.compare("1") == 0)
		// respond to the request adding crlf
		this->streamOut << entry->response << "\r\n"; //<< std::endl;
		// dtk README specifies 1 to add <crlf> so i assume
		// "\r\n" is more accurate that std::endl;
	else
		// default no crlf
		this->streamOut << entry->response;

	// check whether the fsm should terminate
	if (entry->doContinue == false) {
		std::string logMsg = this->confFile + " terminating.";
		globLog.toLog(moduleName, ModuleInfo, logMsg);
		exit(EXIT_SUCCESS);
	}

	// check whether we have a state transition to do
	if (this->curState != entry->nextState)
		this->changeState(entry->nextState);

	return;
}

//{{{1 DXG DOC
/**
 * \brief   toggle the terminal echo on / off
 *  
 * The method sets the terminal echo on or off, depending on the flag
 * that was given.
 *
 * \todo
 * currently only support for real connected ttys is existent.
 * support for telnet like nvt needs to be added !
 *  
 * \return  void
 * \retval  none
 *
 * \param   flag	flag to indicate whether echo should be set or not
 */
//}}}1 DXG DOC
void DtkScriptFSM::setEcho(TermEcho flag)
{

	// FIXME: need to rethink this whole echo thing.
	/*
	int input = this->streamIn.getFd();

	if (isatty(input) == 1) {
		struct termios term, oterm;
		if (tcgetattr(input, &oterm) != 0) {
			memset(&term, 0, sizeof(term));
			memset(&oterm, 0, sizeof(oterm));
			return;
		}

		memcpy(&term, &oterm, sizeof(term));

		switch(flag) {
			case ECHOOFF:
				if (term.c_lflag & ECHO)
					term.c_lflag &= ~ECHO;
				//system("stty -echo");
				break;
			case ECHOON:
				if (!(term.c_lflag & ECHO))
					term.c_lflag |= ECHO;
				//system("stty echo");
				break;
			default:
				break;
		}

		(void)tcsetattr(input, TCSANOW|TCSAFLUSH, &term);
	} else {
		// XXX: handle telnet nvt here, if appropriate!
	}
	*/

	return;
}

//{{{1 DXG DOC
/**
 * \brief   execute another program and send its output to the client
 *
 * The Dtk allows for other programs to be run through the exec
 * operation.  A combination of fork() and execve() has been chosen
 * over popen(), since the latter uses a pipe to its parent which is 
 * not really what is needed here.
 * Instead it has been considered sufficient to dup() the stderr and
 * stdout file descriptors to the socket file descriptor.
 * 
 * \todo 	it is not yet fully understood, whether dtk's exec operation
 * 			allows for commandline arguments. if it does they are not
 * 			(yet) supported by this implementation.
 *
 * \return  void
 * \retval  none
 *
 * \param   *entry	pointer to the object that stores the transition data.
 */
//}}}1 DXG DOC
void DtkScriptFSM::doExec(StateTransitionData *entry)
{
	pid_t child = ::fork();
	if (child == -1) {
		// Error
		globLog.toLog(moduleName, ModuleError, strerror(errno));
	} else
	if (child > 0) {
		// Parent
		// FIXME: react to errors of waitpid.
		int status;
		::waitpid(child, &status, 0);
	} else {
		// Child
		// FIXME: think about chroot()'ing.
		// get the program that is supposed to be run
		std::string program = entry->response;
		char *(args[]) = {
			const_cast<char *>(entry->response.c_str()),
			NULL,
		};

		// get the socket file descriptor
		int std_out = this->streamOut.getFd();
		int std_err = std_out;

		// map stdin of this process to the socket fd.
		if (std_out != STDOUT_FILENO)
			if (::dup2(std_out,STDOUT_FILENO) != STDOUT_FILENO) {
				std::string strError = strerror(errno);
				std::string logMsg = "dup'ing stdout" + strError;
				globLog.toLog(moduleName, ModuleError, logMsg);
			}
		// map stderr of this process to the socket fd.
		if (std_err != STDERR_FILENO)
			if (::dup2(std_err,STDERR_FILENO) != STDERR_FILENO) {
				std::string strError = strerror(errno);
				std::string logMsg = "dup'ing stderr" + strError;
				globLog.toLog(moduleName, ModuleError, logMsg);
			}

		// since std_err refers to the same file descriptor
		// as std_out we only need to close it one of them.
		(void) ::close(std_out);

		// run the new programm
		::execve(program.c_str(), args, NULL);

		::exit(EXIT_SUCCESS);
	}

	return;
}

//{{{1 DXG DOC
/**
 * \brief   write the contents of a file to the client
 *
 * This method can be used to write any file to the client. Its main
 * purpose will probably be to deliver fake system configuration files
 * like /etc/passwd or /etc/hosts.allow to the client.
 *  
 * \todo	XXX: what happens if the line is bigger than our static buffer ?
 *  
 * \return  void
 * \retval  none
 *
 * \param   *entry	pointer to the object that stores the transition data.
 */
//}}}1 DXG DOC
void DtkScriptFSM::doCat(StateTransitionData *entry)
{
	// open the response file
	std::string filename = this->confDir + entry->response;
	std::ifstream responseFile(filename.c_str());
	if (!responseFile) {
		std::string logMsg = "could not open file: " + entry->response;
		globLog.toLog(moduleName, ModuleError, logMsg);
		return;
	}

	// write the file out to the client.
	char buf[1024]; // FIXME: static buffer
	while (responseFile.getline(buf, 1023, '\n'))
		// XXX: what happens if the line is bigger than our buffer ?
		this->streamOut << buf << std::endl;

	return;
}

//{{{1 DXG DOC
/**
 * \brief   start running the finite state machine.
 *  
 * This method is loops calling parse() indefinitely. It was needed,
 * since a non-state instance of DtkScriptFSM is created and used to
 * initiate the finite state machine.
 * This method can be seen as an entry point for outside object, not
 * being part of the state machine.
 *  
 * \return  void
 * \retval  none
 */
//}}}1 DXG DOC
void DtkScriptFSM::start(void)
{
	while (true)
		this->states[this->curState]->parse();

	return;
}

//{{{1 DXG DOC
/**
 * \brief   read input from the client and react accordingly
 *
 * The parse method reads input from the client via the network
 * and then parses this instance's StateTransitionTables to look
 * for a suitable transition.  It is important to note that scoring is
 * implemented in a very casual manner, which means the table with the
 * most signifact data (in this case matchAction) is checked first, then
 * the pattern matching table is tested and last but not least the
 * methods searches for any word that is contained within the wordMatch.
 * This method also increments and compares a loop counter ensuring that
 * the state machine does not run infinitly.
 *  
 * \todo	NIL does not work as expected. which also brings up the
 * 			question: "what is the intended operation of NIL ?"
 *
 * \bugs	the test whether buf is empty or not will always evaluate to
 * 			true under solaris, so the module does not get run at all!
 *  
 * \return  void
 * \retval  none
 */
//}}}1 DXG DOC
void DtkScriptFSM::parse(void)
{
	// bail out in case we've looped too often
	if (++this->curLoop >= this->confMaxLoops)
		::exit(EXIT_SUCCESS);

	char buf[1024]; // FIXME: static buffer
	std::string input;
	std::string logMsg;

	// retrieve the data from the network
	this->streamIn.getline(buf, 1023, '\n');
	if (buf[0] == 0) // FIXME: catches EOF / ^D but NOT on solaris !
		::exit(EXIT_SUCCESS);
	input = buf;
	logMsg = "recv '" + input + "'";
	globLog.toLog(moduleName, Debug, logMsg);



	// FIXME: what is NIL supposed to be acting on ?
	if (input.compare("\n") == 0) {
		this->dtkSpecial("NIL");
		return;
	}
	
	// log input
	// XXX: could use some special char parsing (p.e. '\n'->^M)
	logMsg = this->confFile + "Input '" + input + "'";
	globLog.toLog(moduleName, ModuleInfo, logMsg);

	// set up come common variables needed for the match scoring
	StateTransitionData *entry = NULL;
	StateTransitionTableIterator transition;
	StateTransitionTableIterator end;

	// matchAction
	// loop through the actionMatch table looking for an appropriate entry
	transition = this->matchAction.begin();
	end = this->matchAction.end();
	std::string dtkAction;
	int found;
	for (; transition != end; transition++) {
		dtkAction = (*transition).first;

		found = input.find(dtkAction);
		if (found >= 0)
			entry = (*transition).second;
	}

	// respond and bail out if we have a valid actionMatch
	if (entry != NULL) {
		this->respond(entry);
		return;
	}

	// matchPattern
	// loop through the patternMatch table looking for an appropriate entry
	transition = this->matchPattern.begin();
	end = this->matchPattern.end();
	std::string dtkRegex;
	for (; transition != end; transition++) {
		dtkRegex = (*transition).first;
		pcre *re;
		const char *error;
		int errOffset;
		int rc;
		int ovector[30];

		// compile perl compatible regex from the table
		re = pcre_compile(dtkRegex.c_str(), 0, &error, &errOffset, NULL);
		if (re == NULL) {
			std::cerr << "compile of regex '" << dtkRegex << "' failed\n";
			continue;
		}

		// test perl compatible regex from the table with the input
		rc = pcre_exec(re, NULL, input.c_str(), input.length(), 0, 0, ovector, 30);
		if (rc < 0)
			continue; // do better error handling see pcre(3)

		// set a variable to the entry found for later processing below
		entry = (*transition).second;
	}

	// respond and bail out if we have a valid patternMatch
	if (entry != NULL) {
		this->respond(entry);
		return;
	}

	// matchWord
	// loop through the wordMatch table looking for an appropriate entry
	transition = this->matchWord.begin();
	end = this->matchWord.end();
	for (; transition != end; transition++) {
		std::string word = (*transition).first;
		int pos = input.find(word);
		if (pos < 0)
			continue;

		// set a variable to the entry found for later processing below
		entry = (*transition).second;
	}

	// respond with the wordMatch or display error message
	if (entry != NULL)
		this->respond(entry);
	else
		this->dtkSpecial("ERROR");

	return;
}

//{{{1 DXG DOC
/**
 * \brief   initialise the finite state machine
 *  
 * This method sets up the whole finite state machine, it reads the
 * configuration from the response file (script) and creates the state
 * objects.
 *  
 * \return  void
 * \retval  none
 *
 * \param   dir		direcotrypath to the Dtk response files (scripts)
 * \param   file	filename of the Dtk response file (script)
 */
//}}}1 DXG DOC
void DtkScriptFSM::init(std::string dir, std::string file)
{
#define DO_PROF
#ifdef DO_PROF
	timeval start;
	gettimeofday(&start, NULL);
#endif
	std::string logMsg;
	// check if the directory is accesible
	if (access(dir.c_str(), F_OK) < 0) {
		logMsg = "could not open directory: " + dir;
		globLog.toLog(moduleName, ModuleError, logMsg);

		exit(EXIT_FAILURE);
	}

	this->confDir = dir + "/";
	// check if the file is accesible
	std::string pathToFile = this->confDir + file;
	if (access(pathToFile.c_str(), F_OK) < 0) {
		logMsg = "could not open file: " + pathToFile;
		globLog.toLog(moduleName, ModuleError, logMsg);

		exit(EXIT_FAILURE);
	}
	this->confFile = file;

	// loop over all states creating and initializing a new state object
	for (int i = 0; i < STATECOUNT; i++) {
		this->states[i] = new DtkScriptFSM(this->streamIn, this->streamOut);
		this->states[i]->initState(pathToFile, i);
	}

	// set the starting state of the fsm
	this->changeState(0);

#ifdef DO_PROF
    timeval end;
    gettimeofday(&end, NULL);

    long us = ((end.tv_sec - start.tv_sec) * 1000000)
      + (end.tv_usec - start.tv_usec);

    //std::cout << "Total time for request = " << us << " us" << " (" << (double) (us/1000000.0) << " s)";
	char lus[32];
	snprintf(lus, 32, "%ld", us);
	char ls[32];
	snprintf(ls, 32, "%f", (double) (us/1000000.0));
	logMsg = "initalization of " + this->confFile + " took " + ls + " sec.";
	globLog.toLog(moduleName, Debug, logMsg);
#endif

	// START is only run once upon fsm startup.
	// it is not taken into account for DO_PROF, since START may use a
	// delay plus initialisation of the fsm is already done at this point.
	this->states[0]->dtkSpecial("START");


	return;
}

//{{{1 DXG DOC
/**
 * \brief   initalise a state object with its transitions
 *  
 * This method does the actual initalisation for a state object within
 * the finite state machine.  It reads the configuration file and
 * extracts all lines that are of interest for this \c stateNum.
 * After that all relevant lines are split into their fields and
 * the StateTransitionData is put into the appropriate StateTransitionTable.
 *
 * \todo
 * sttdDtkFields loops from the end of the line assuming there are only
 * 3 fields (dtkspecial = '!', variable and value), but what if
 * there is a fourth comment field.
 * we probably need to change the line parsing alogrithm.
 *  
 * \return  void
 * \retval  none
 *
 * \param   file	the Dtk response filename
 * \param   stateNum	the state we'd like to get the 	transitions for
 */
//}}}1 DXG DOC
void DtkScriptFSM::initState(std::string file, unsigned int stateNum)
{
	unsigned int i;
	std::string operation;
	std::string infocon = "I";
	std::string stimulus = "!";
	std::string otp = "!O";
	std::string alg = "!A";
	std::string logMsg;
	// XXX: we already checked the existance of the file in init(), so
	// this might be a little too paranoid, on the other hand: 'better
	// safe than sorry'.
    // open script file
    std::ifstream confFile(file.c_str());
    if (!confFile) {
		logMsg = "could not open file: " + file;
		globLog.toLog(moduleName, ModuleError, logMsg);
        return;
    }

    char *row, *split;
	std::string line;
    std::string data[7];
	// the following 2 variable represent the starting deliminator for
	// pcre's wthiin the config file
    const std::string regExStr1 = "/";
    const std::string regExStr2 = "M!";
	typedef enum matchTableIdent{DTK, PATTERN, ACTION, WORD} TableIdent;
    enum sttdFields { STATE = 0, INPUT, NEXTSTATE, CONTINUE, OPERATION, RESPONSE };
    enum sttdDtkActionFields { STIMULUS = 5, ST_RESPONSE };
    enum sttdDtkConfigFields { VARIABLE = 1, VALUE };
    enum sttdDtkNoticeFields { SCRIPT = 2, EMAIL, MESSAGE };
	StateTransitionTable *whatTable = &this->matchAction;

    // read the whole line
    char buf[1024]; // FIXME: static buffer!
    while (confFile.getline(buf, 1023, '\n')) {
        // skip lines that are not interesting to us.
        if ((strlen(buf) == 0)			// skip blanks
				|| (buf[0] == '#')		// skip comments
				|| ((buf[0] != '!')		// skip non-fsm-config lines
					// skip lines that are not the same state as this obj
					&& (unsigned int)atoi(&buf[0]) != stateNum))
            continue;

		row = buf;
		// strip fields from line and store them as temporary data.
		for (i = 0; i < 6; i++) {
			data[i] = "";
			split = strchr(row,'\t');
			if (split == NULL)
				break;
			*split = '\0';
			data[i] = row;
			row = split + 1;
		}   
		data[i] = row;

		if (row == buf) {
			logMsg = this->confFile + ": Transition did not contain tabstop!";
			globLog.toLog(moduleName, ModuleError, logMsg);
			continue;
		}

		// set fsm configuration variables
		if (buf[0] == '!') {
			if (data[VARIABLE].compare("maxloops") == 0) {
				this->confMaxLoops = (unsigned int) atoi(data[VALUE].c_str());
			} else
			if (data[VARIABLE].compare("timeout") == 0) {
				this->confTimeout = (unsigned int) atoi(data[VALUE].c_str());
				this->streamIn.setTimeout(this->confTimeout, 0);
			} else
			if ((data[VARIABLE].compare("delay") == 0)
				|| (data[VARIABLE].compare("slowly") == 0)) {
				this->confDelay = (unsigned int) atoi(data[VALUE].c_str());
			} else
			if (data[VARIABLE].compare("debug") == 0) {
				this->debug = (atoi(data[VALUE].c_str()) == 1) ? true : false;
			}
			continue;
		}

		// some more helper variables to convert data types
		unsigned int nextState = atoi(data[NEXTSTATE].c_str());
		char c[6];
		::snprintf(c, 6, "%s", data[CONTINUE].c_str());
		bool doContinue = (atoi(c) == 1) ? true : false;
		std::string idx = data[INPUT];

#if __GNUC__ == 2
		// match InfoCon Operation
        if (data[OPERATION].compare(infocon, 0, 1) == 0) {
#else
        if (data[OPERATION].compare(0, 1, infocon) == 0) {
#endif
			logMsg = this->confFile + ": InfoCon Classification not supported!";
			globLog.toLog(moduleName, ModuleInfo, logMsg);

			operation = data[OPERATION];
			// delete I[0-9] second number is dtk operation.
			operation.erase(0,2);

			data[OPERATION] = operation;
		}

		// match notice
	   if (data[INPUT].compare("NOTICE") == 0) {
			this->matchDtk[idx] = new StateTransitionData(data[SCRIPT], data[EMAIL], data[MESSAGE]);
			continue;
	   }

        // switch INPUT for appropiate script line entry
		// matchDtk
        if ((data[INPUT].compare("START") == 0)
            || (data[INPUT].compare("NIL") == 0)
            || (data[INPUT].compare("ERROR") == 0)) {
            whatTable = &this->matchDtk;
        } else
// non-gcc compiler compatability hook
#ifndef __GNUC__
#define __GNUC__ 0
#endif

// method prototype for std::string.compare() has changed in gcc2 -> gcc3
		// matchAction
#if __GNUC__ == 2
        if (data[INPUT].compare(otp, 0, 2) == 0) {
#else
        if (data[INPUT].compare(0, 2, otp) == 0) {
#endif
			logMsg = this->confFile + ": One Time Password match not supported";
			globLog.toLog(moduleName, ModuleInfo, logMsg);
        } else
#if __GNUC__ == 2
        if (data[INPUT].compare(alg, 0, 2) == 0) {
#else
        if (data[INPUT].compare(0, 2, alg) == 0) {
#endif
			logMsg = this->confFile + ": Algorithmic Identification match not supported";
			globLog.toLog(moduleName, ModuleInfo, logMsg);
        } else
#if __GNUC__ == 2
        if (data[INPUT].compare(stimulus, 0, 1) == 0) {
#else
        if (data[INPUT].compare(0, 1, stimulus) == 0) {
#endif
			idx = data[STIMULUS];
			data[RESPONSE] = data[ST_RESPONSE];
            whatTable = &this->matchAction;
        } else
#if __GNUC__ == 2
		// matchPattern
        if ((data[INPUT].compare(regExStr1, 0, regExStr1.length()) == 0)
            || (data[INPUT].compare(regExStr2, 0, regExStr2.length()) == 0)) {
#else
        if ((data[INPUT].compare(0, regExStr1.length(), regExStr1) == 0)
            || (data[INPUT].compare(0, regExStr2.length(), regExStr2) == 0)) {
#endif
			// pcre won't allow for '/' or 'M!' perl compat regex markup!
			// so strip these away.
			char *perlPattern = const_cast<char *>(data[INPUT].c_str());
			if (perlPattern[0] == '/')
				perlPattern += 1;
			else
				perlPattern += 2;
			int len = strlen(perlPattern) - 1;
			perlPattern[len] = '\0';
			idx = perlPattern;

            whatTable = &this->matchPattern;
        } else {
		// matchWord
            whatTable = &this->matchWord;
        }

		(*whatTable)[idx] = new StateTransitionData(nextState, doContinue, data[OPERATION], data[RESPONSE]);
    } // end while

	return;
}

//{{{1 DXG DOC
/**
 * \brief	default destructor
 */
//}}}1 DXG DOC
DtkScriptFSM::~DtkScriptFSM(void)
{
	return;
}

//{{{1 DXG DOC
/**
 * \brief	default destructor
 *
 * initialises the private i/o members.
 *
 * \param	in	the input stream
 * \param	out	the output sream
 */
//}}}1 DXG DOC
DtkScriptFSM::DtkScriptFSM(InputStream &in, OutputStream &out)
	:
	streamIn(in)
	,streamOut(out)
{
	return;
}


///////////////////
// HIDDEN METHODS
///////////////////

//{{{1 DXG DOC
/**
 * \brief       copy destructor
 *
 * this copy constructor is hidden, since it is currently not needed.
 */
//}}}1 DXG DOC
/*
DtkScriptFSM::DtkScriptFSM(const DtkScriptFSM &rCopy)
{
        return;
}
*/
