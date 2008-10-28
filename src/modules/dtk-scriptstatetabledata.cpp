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
 * \file    dtk-scriptstatetabledata.cpp
 * \brief	implements methods in class  StateTransitionData
 * \author  alexis hildebrandt <afh@2drop.net>
 *
 * This file implements the methods for class StateTransitionData.
 *
 */

// Module Headers
#include "dtk-scriptstatetabledata.h"


DTKSCRIPT_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * \brief   constructor
 *
 * used for NOTICE transitions. note that the \c operation and \c response
 * depend on the configured notice script. one script could put the
 * email address into the \c operation field and the message body into
 * the the \c response field.
 *
 * DTK default notify pl puts the email address into the \c response
 * field.
 *  
 * \param   _notice		notice script file
 * \param   _operation	unused 
 * \param   _response	message body
 */
//}}}1 DXG DOC
StateTransitionData::StateTransitionData(
	std::string _notice
	,std::string _operation
	,std::string _response)
	:
	doContinue(true)
	,notice(_notice)
	,operation(_operation)
	,response(_response)
{
	return;
}
//{{{1 DXG DOC
/**
 * \brief   default constructor
 *
 * used for most transition in init.
 *  
 * \param   _nextState		the next state the machine should change into
 * \param   _doContinue		should the state machine terminate?
 * \param   _operation		the DTK Operation for this transition
 * \param   _response		the output sent to the client
 */
//}}}1 DXG DOC
StateTransitionData::StateTransitionData(
	unsigned int _nextState
	,bool _doContinue
	,std::string _operation
	,std::string _response)
	:
	doContinue(_doContinue)
	,nextState(_nextState)
	,notice("")
	,operation(_operation)
	,response(_response)
{
	return;
}

//{{{1 DXG DOC
/**
 * \brief   default destructor
 */
//}}}1 DXG DOC
StateTransitionData::~StateTransitionData(void)
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
StateTransitionData::StateTransitionData(const StateTransitionData &rCopy)
{
        return;
}
*/

