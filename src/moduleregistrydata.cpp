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
 * \file	moduleregistrydata.cpp
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file implements class ModuleRegistryData
 *
 */

// Project Headers
#include "moduleregistrydata.h"


DECEPTION_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * Set the filename that this regdata belongs to
 *
 * \param	modFileName	The module filename that this regdata belongs to
 */
//}}}1 DXG DOC
void ModuleRegistryData::setFileName(std::string modFileName)
{
	// allow this->fileName to be set once!
	if (this->fileName.empty() == true)
		this->fileName = modFileName;

	return;
}

//{{{1 DXG DOC
/**
 * Return the filename
 *
 * \return Valid filename of module
 */
//}}}1 DXG DOC
std::string ModuleRegistryData::getFileName(void) const
{
	return this->fileName;
}

//{{{1 DXG DOC
/**
 * Set the factory for this module
 *
 * \return	void
 *
 * \param	*factory	The module factory pointer.
 */
//}}}1 DXG DOC
void ModuleRegistryData::setFactory(ModuleFactoryBase *factory)
{
	// allow this->pFactory to be set once!
	if (this->pFactory == NULL)
		this->pFactory = factory;

	return;
}

//{{{1 DXG DOC
/**
 * Return the factory
 *
 * \return	ModuleFactoryBase pointer
 * \retval	"void *" address of the factory object
 * \retval	"NULL" if none has registered.
 */
//}}}1 DXG DOC
ModuleFactoryBase* ModuleRegistryData::getFactory(void) const
{
	return this->pFactory;
}

//{{{1 DXG DOC
/**
 * Set the handle for this module
 *
 * \param	*handle	The module handle.
 */
//}}}1 DXG DOC
void ModuleRegistryData::setHandle(void *handle)
{
	// allow this->pFactory to be set once!
	if (this->pHandle == NULL)
		this->pHandle = handle;

	return;
}

//{{{1 DXG DOC
/**
 * Return the filename
 *
 * \return	void pointer
 * \retval	"void *" valid handle of module
 * \retval	"NULL" if none has been registerd.
 */
//}}}1 DXG DOC
void* ModuleRegistryData::getHandle(void) const
{
	return this->pHandle;
}

//{{{1 DXG DOC
/**
 * Set the name for this module
 *
 * \param	modOption	String containing the options for this module
 */
//}}}1 DXG DOC
void ModuleRegistryData::setOption(std::string modOption)
{
	// allow this->option to be set once!
	if (this->option.empty() == true)
		this->option = modOption;

	return;
}

//{{{1 DXG DOC
/**
 * Return the module option string
 *
 * \return	Option string
 */
//}}}1 DXG DOC
std::string ModuleRegistryData::getOption(void) const
{
	return this->option;
}

//{{{1 DXG DOC
/**
 * Set the name for this module
 *
 * \param	modName The module name.
 */
//}}}1 DXG DOC
void ModuleRegistryData::setModName(std::string modName)
{
	// allow this->name to be set once!
	if (this->name.empty() == true)
		this->name = modName;

	return;
}

//{{{1 DXG DOC
/**
 * Return the module name
 *
 * \return	Valid module name
 */
//}}}1 DXG DOC
std::string ModuleRegistryData::getModName(void) const
{
	return this->name;
}

//{{{1 DXG DOC
/**
 * Set the socket for this module
 *
 * \param	*socket	The module's socket.
 */
//}}}1 DXG DOC
void ModuleRegistryData::setSocket(Socket *socket)
{
	// allow this->pSocket to be set once!
	if (this->pSocket == NULL)
		this->pSocket = socket;

	return;
}

//{{{1 DXG DOC
/**
 * Return the module name
 *
 * \return Valid module name
 */
//}}}1 DXG DOC
Socket* ModuleRegistryData::getSocket(void) const
{
	return this->pSocket;
}

//{{{1 DXG DOC
/**
 * Initialize private members
 */
//}}}1 DXG DOC
ModuleRegistryData::ModuleRegistryData(void)
	:
	fileName("")
	,pFactory(NULL)
	,pHandle(NULL)
	,name("")
	,option("")
	,pSocket(NULL)
{ // DEFAULT CONSTRUCTOR
	return;
}

//{{{1 DXG DOC
/**
 * Initialize private members
 *
 * \param modFileName	Filename of module
 * \param modName		Name of module
 */
//}}}1 DXG DOC
ModuleRegistryData::ModuleRegistryData(std::string modFileName,
		std::string modName)
	:
	fileName(modFileName)
	,pFactory(NULL)
	,pHandle(NULL)
	,name(modName)
	,option("")
	,pSocket(NULL)
{ // CONSTRUCTOR
	return;
}

//{{{1 DXG DOC
/**
 * Does nothing usefule
 */
//}}}1 DXG DOC
ModuleRegistryData::~ModuleRegistryData(void)
{ // DESTRUCTOR
	return;
}

///////////////////
// HIDDEN METHODS
///////////////////

//{{{1 DXG DOC
/**
 * \brief	copy destructor
 *
 * this copy constructor is hidden, since it is currently not needed.
 */
//}}}1 DXG DOC
/*
ModuleRegistryData::ModuleRegistryData(const ModuleRegistryData &rCopy)
{
	return;
}
*/
