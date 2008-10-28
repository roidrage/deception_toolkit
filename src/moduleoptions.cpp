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
 * \file	moduleoptions.cpp
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file implements the methods for class  ModuleOptions
 *
 */

// Project Headers
#include "moduleoptions.h"


DECEPTION_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * Search for key in the private data member that has been initialized
 * within this class' constructor and return the value for key if
 * found.
 *
 * \retval	""	when key was not found
 *
 * \param	key	Name of the option
 */
//}}}1 DXG DOC
std::string ModuleOptions::getOption(std::string key)
{
	if (this->data.find(key) != this->data.end())
		return this->data[key];

	return std::string("");
}

//{{{1 DXG DOC
/**
 * \todo	We might want to put this functionality into a separate
 * 			method \c parseOptString() maybe ? It could be enabled to
 * 			accept a deliminator char for key=value and one for
 * 			key-value-pair to distinguish between the two.
 *
 * \todo	Allow for a custom deliminator
 *
 * \param	optString	The option string from the option attribute
 */ 
//}}}1 DXG DOC
ModuleOptions::ModuleOptions(std::string optString)
{ // CONSTRUCTOR
	// set up some helper variables
	int curPos;
	char keyValDelim = '=';
	std::string value;
	std::string key = optString;

	// don't waste time on an empty string
	if (optString.empty() == true)
		return;

	// find deliminator
	curPos = key.find(keyValDelim);
	if (curPos == 0)
		return;

	// split key and value from the optionstring
	value.assign(key, curPos + 1, key.npos);
	key.erase(curPos, key.npos);

	// store key-value pair
	this->data[key] = value;

	return;
}

//{{{1 DXG DOC
/**
 * Does nothing useful
 */
//}}}1 DXG DOC
ModuleOptions::~ModuleOptions(void)
{ // DESTRUCTOR
	return;
}

