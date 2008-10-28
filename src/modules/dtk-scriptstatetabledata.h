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
 * \file	dtk-scriptstatetabledata.h
 * \brief	declares class StateTransitionTableData
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares the main class for the Dtk-Script Transitions.
 *
 */

#ifndef __DTK_SCRIPTSTATETABLEDATA_H_
#define __DTK_SCRIPTSTATETABLEDATA_H_ 1

// C++ Headers
#include <string>

// Project Headers
#include "defs.h"

// Module Headers
#include "dtk-scriptdefs.h"


DTKSCRIPT_NAMESPACE_BEGIN

//{{{1 DXG DOC
/**
 * \class   StateTransitionData
 * \brief   hold the data for a state transition
 *
 * For each transition additional data must be provided. Should the
 * state machine terminate? What is the exact response to the client,
 * how will it be delivered, and what is the next state the state
 * machine should go into?
 * All that data will be kept within this class.
 *
 * \todo	currently all memebers are public, it may be a better a idea
 * 			to make them private and define appropriate methods.
 */     
//}}}1 DXG DOC
class StateTransitionData
{
	public:
		~StateTransitionData(void);
		StateTransitionData(std::string _notice
			,std::string _operation
			, std::string _response);
		StateTransitionData(unsigned int _nextState
			,bool _doContinue
			,std::string _operation
			, std::string _response);

		// XXX: public members
		bool doContinue;			///< indicates whether to keep on running
		unsigned int nextState;		///< next state of the transition
		std::string notice;			///< notify script
		std::string operation;		///< define how to respond to the client
		std::string response;		///< actual response to the client

	private:
		StateTransitionData(const StateTransitionData &rCopy);
};

DTKSCRIPT_NAMESPACE_END

#endif // __DTK_SCRIPTSTATETABLEDATA_H_
