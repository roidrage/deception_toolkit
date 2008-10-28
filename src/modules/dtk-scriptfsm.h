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
 * \file	dtk-scriptfsm.h
 * \brief	declares class DtkScriptFSM
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the main class for the Dtk-Script state machine.
 *
 */

#ifndef __DTK_SCRIPTFSM_H_
#define __DTK_SCRIPTFSM_H_ 1

// C++ Headers
#include <fstream>
#include <string>
#include <map>

// C Headers
#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pcre.h>

// Project Headers
#include "defs.h"
#include "inputstream.h"
#include "outputstream.h"

// Module Headers
#include "dtk-scriptdefs.h"
#include "dtk-scriptstatetabledata.h"

#define STATECOUNT 7


DECEPTION_NAMESPACE_USE;

DTKSCRIPT_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class   DtkScriptFSM
 * \brief   finite state machine to run the dtk response files (scripts)
 *
 * This class implements the a finite state machine that uses the
 * same plain text respond files as the Dtk to set up the transitions.
 *
 * \todo	not all features defined by the Dtk are currently supported
 * 			input match (!O, !A), operation (InfoCon, special)
 *
 * \todo	the echo-operation only works for directly connected
 * 			terminals, which not really useful. implementing some sort
 * 			of telnet like nvt compatability is needed.
 *
 * 	\todo	states is a statically defined array of 6 entires.
 * 			supporting some sort of statecount in the constructor might
 * 			be a good idea.
 */     
//}}}1 DXG DOC
class DtkScriptFSM
{
	public:
		//{{{ 2 DXG DOC
		/**
		 * \brief	convenience type definition for nvt
		 *
		 * to fully support all DTK operation we need to do some
		 * network virtual terminal emulation. this is just a define
		 * to easily distinguish between nvt echo on and off.
		 */
		//}}} 2 DXG DOC
		typedef enum termecho { ECHOOFF, ECHOON } TermEcho;

		void start(void);
		void init(std::string dir, std::string file);

		~DtkScriptFSM(void);
		DtkScriptFSM(InputStream &in, OutputStream &out);

	private:
		void parse(void);
		void initState(std::string file, unsigned int stateNum);
		void respond(StateTransitionData *entry);
		void changeState(unsigned int stateNum);
		void dtkSpecial(std::string command);

		void doExec(StateTransitionData *entry);
		void doCat(StateTransitionData *entry);
		void setEcho(TermEcho flag);

		//{{{ 2 DXG DOC
		/**
		 * \brief	convenience type definition for map
		 *
		 * all data is kept in a private map which has a lengthy
		 * declaration, so this is just a short name for it.
		 */
		//}}} 2 DXG DOC
		typedef std::map<std::string,StateTransitionData*> StateTransitionTable;
		//{{{ 2 DXG DOC
		/**
		 * \brief	convenience type definition for map-iterator
		 *
		 * all data is kept in a private map which has a lengthy
		 * declaration, so this is just a short name for its iterator.
		 */
		//}}} 2 DXG DOC
		typedef StateTransitionTable::iterator StateTransitionTableIterator;

		static unsigned int curLoop;		///< current loop the machine is in
		static unsigned int curState;		///< indicates the current state
		// FIXME: this could be a more appropriate data type (time_t) ?
		static unsigned int confDelay;		///< how many seconds will the response be delayed
		static std::string confDir;			///< script directory
		static std::string confFile;		///< script file
		static unsigned int confMaxLoops;	///< how many times to loop the machine the most
		static unsigned int confTimeout;	///< when to close the connection due to idle timeout.
		static bool debug;					///< run in debug mode
		StateTransitionTable matchAction;	///< transition table for actions
		StateTransitionTable matchDtk;		///< transition table for dtk special commands
		StateTransitionTable matchPattern;	///< transition table for patterns / regex's
		StateTransitionTable matchWord;		///< transition table for words
		static DtkScriptFSM *states[STATECOUNT];		///< array of all state objects

		InputStream &streamIn;
		OutputStream &streamOut;

		// hidden
		DtkScriptFSM(const DtkScriptFSM &rCopy);
};

DTKSCRIPT_NAMESPACE_END

#endif // __DTK_SCRIPTFSM_H_
