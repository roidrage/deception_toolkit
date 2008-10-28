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
 * \file	moduleregistry.cpp
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file implements class ModuleRegistry.
 *
 */

#include <iostream>

// Project Headers
#include "moduleregistry.h"


DECEPTION_NAMESPACE_USE;

// define static members
ModuleRegistry::ModuleRegistryMap ModuleRegistry::data;
std::string ModuleRegistry::className = "ModuleRegistry";

// {{{1 DXG DOC
/**
 * Remove a pointer to ModuleRegistryData from the map
 *
 * \param fileName Filename of module to look for
 */
// }}}1
void ModuleRegistry::remove(std::string fileName)
{ // {{{1
	ModuleRegistry::ModuleRegistryMapIterator itr = this->data.begin();
	ModuleRegistry::ModuleRegistryMapIterator itrEnd = this->data.end();
	ModuleRegistryData *mrData;
	std::string logMsg, modFileName;
	// loop over the list of configured modules
	for (; itr != itrEnd; itr++) {
		modFileName = (*itr).second->getFileName();
		if (modFileName.compare(fileName) != 0)
			continue;

		mrData = (*itr).second;
		// remove that darn module
		this->data.erase((*itr).first);
		delete(mrData);

		break;
	}
	return;
} // }}}1

// {{{1 DXG DOC
/**
 * Retrive the optionstring for a module
 *
 * \param idx	Index string to find
 */
// }}}1
std::string ModuleRegistry::getOption(std::string idx)
{
	return this->data[idx]->getOption();
}


// {{{1 DXG DOC
/**
 * Remove a pointer to ModuleRegistryData from the map
 *
 * \param idx	Index string to find
 */
// }}}1
ModuleFactoryBase* ModuleRegistry::find(std::string idx)
{
	if (this->data.find(idx) == this->data.end())
		return NULL;
	else
		return this->data[idx]->getFactory();
}

// {{{1 DXG DOC
/**
 * Point iterator the first element of the data member
 *
 * \return	Valid iterator
 */
// }}}1
ModuleRegistry::ModuleRegistryMapIterator ModuleRegistry::begin(void)
{
	return this->data.begin();
}

// {{{1 DXG DOC
/**
 * Point iterator the last element of the data member
 *
 * \return	Valid iterator
 */
// }}}1
ModuleRegistry::ModuleRegistryMapIterator ModuleRegistry::end(void)
{
	return this->data.end();
}

// {{{1 DXG DOC
/**
 * Extract the portnumber from the index string
 *
 * \return	Portnumber portion of the index string
 * 
 * \param ipport index string consisting of ipaddress:port
 */
// }}}1
unsigned int ModuleRegistry::getPort(std::string ipport)
{
	return atoi(ipport.erase(0,ipport.find(':')+1).c_str());
}

// {{{1 DXG DOC
/**
 * Extract the IP address from the index string
 *
 * \return	IP address portion of the index string
 *
 * \note	Validity check of the IP address is not done!
 * 
 * \param ipport Index string consisting of ipaddress:port
 */
// }}}1
std::string ModuleRegistry::getIpAddr(std::string ipport)
{
	return ipport.erase(ipport.find(':'),ipport.npos);
}

//{{{1 DXG DOC
/**
 * Add a new ModuleRegistryData to the registry member
 *
 * \warning	all \c new 'ed elements are never \c delete d !!!
 *
 * \param	ipAddr IP address to bind this module to
 * 
 * \param	portNum	the port number for this module
 *
 * \param	modFileName
 * 			the module filename that this registry data should be registered to.
 *
 * \param	modName
 * 			the module name that this registry data should be registered to.
 *
 * \param	option	Option string
 * 			
 */
//}}}1 DXG DOC
void ModuleRegistry::addModule(std::string ipAddr
		,unsigned int portNum
		,std::string modFileName
		,std::string modName
		,std::string option)
{
	// build the index string 'ipaddr:port'
	char strPort[6];
	snprintf(strPort, 6, "%d", portNum);
	std::string idx = ipAddr + ':' + strPort;

	// add a new element
	this->data[idx] = new ModuleRegistryData(modFileName, modName);

	// add optionstring if existent
	if (option.empty() == false)
		this->data[idx]->setOption(option);
}

//{{{1 DXG DOC
//  first method does not need to define dxg doc \overload
// \overload	"ModuleRegistry::insert(int portNum, Socket *socket)"
/**
 * Insert by module portnum & socket
 *
 * \todo	Error handling, if portNum not found.
 *
 * \param	ipAddr
 * 			Vhost ip address
 *
 * \param	portNum
 * 			The port this module was registered for
 *
 * \param	*pSocket
 * 			The socket for this modules' specific port and ip address
 */
//}}}1 DXG DOC
void ModuleRegistry::insert(std::string ipAddr, unsigned int portNum, Socket *pSocket)
{
	// build the index string 'ipaddr:port'
	char strPort[6];
	snprintf(strPort, 6, "%d", portNum);
	std::string idx = ipAddr + ':' + strPort;

	// set the socket if there is a valid member in data
	if (this->data.find(idx) != this->data.end())
		this->data[idx]->setSocket(pSocket);

	return;
}
//{{{1 DXG DOC
//  first method does not need to define dxg doc \overload
// \overload	"ModuleRegistry::insert(std::string modName,ModuleFactoryBase *pFactory)"
/**
 * Insert by module name & module factory
 *
 * Support for virtual hosts calls for a slightly different approach on
 * inserting factories. Since all module factories are going to be the
 * same for the same dynamic object, what we do is check whether other
 * ModuleRegistryData objects with the same module name as ours exist but
 * that whose factory is NULL in the ModuleRegistry and then set all
 * those factories to our factory.
 * Of course this is not a very optimal way but for this
 * proof of concept implementation it will do.
 *
 * \param	modName
 * 			The module name that this registry data should be registered to.
 *
 * \param	*pFactory
 * 			The module registry data to be inserted for \c modName
 *
 */
//}}}1 DXG DOC
void ModuleRegistry::insert(std::string modName, ModuleFactoryBase *pFactory)
{
	std::string idx = "";
	ModuleRegistryMapIterator itBegin = this->begin();
	ModuleRegistryMapIterator itEnd = this->end();

	// loop through the map of modules
	for (; itBegin != itEnd; itBegin++) {
		idx = (*itBegin).first;
		std::string regModName = (*itBegin).second->getModName();

		// add the factory for all modules with the same modulename
		// that do not already have a factory set.
		if ((modName.compare(regModName) == 0)
			&& (this->data[idx]->getFactory() == NULL))
			this->data[idx]->setFactory(pFactory);
	}

	return;

}

//{{{1 DXG DOC
/**
 * \overload	"ModuleRegistry::insert(std::string modFileName, void *pHandle)"
 * Insert by module flename & module handle
 *
 * Support for virtual hosts calls for a slightly different approach on
 * inserting module handles. Since all module handle are going to be the
 * same for the same dynamic object, what we do is check whether other
 * ModuleRegistryData objects with the same filename as ours exist but
 * that whose handle is NULL in the ModuleRegistry and then set all
 * those handles to our handle.
 * Of course this is not a very optimal way but for this
 * proof-of-concept implementation it will do.
 *
 * \param	modFileName
 * 			The module filename that this handle should be registered to.
 *
 * \param	*pHandle
 * 			The module handle to be inserted for \c modFileName
 *
 */
//}}}1 DXG DOC
void ModuleRegistry::insert(std::string modFileName, void *pHandle)
{
	std::string idx = "";
	ModuleRegistryMapIterator itBegin = this->begin();
	ModuleRegistryMapIterator itEnd = this->end();

	// loop through the map of modules
	for (; itBegin != itEnd; itBegin++) {
		idx = (*itBegin).first;
		std::string regFileName = (*itBegin).second->getFileName();

		// add module handle for all modules with the same module
		// filename that do not already have a handle set.
		if ((modFileName.compare(regFileName) == 0)
			&& (this->data[idx]->getHandle() == NULL))
			this->data[idx]->setHandle(pHandle);
	}

	return;
}

//{{{1 DXG DOC
/**
 * Fetch dl handle for a specific module file
 * 
 * \param	modFileName
 * 			The module filename of the handle to be fetched
 */
//}}}1 DXG DOC
void* ModuleRegistry::getHandle(std::string modFileName)
{
	// find map entry for module filename
	std::string idx = this->findPort(modFileName, ModuleRegistry::FILENAME);

	// return the handle if found.
	if (idx.empty() == false)
		return this->data[idx]->getHandle();
		/* icc warning:
		 * remark #383: value copied to temporary, reference to temporary used
		 *         this->data[portNum]->setHandle(pHandle);
		 */

	// return 'not found'
	return NULL;
}

//{{{1 DXG DOC
/**
 * Default constructor
 */
//}}}1 DXG DOC
ModuleRegistry::ModuleRegistry(void)
{ // DEFAULT CONSTRUCTOR
	return;
}

//{{{1 DXG DOC
/**
 * Destructor
 */
//}}}1 DXG DOC
ModuleRegistry::~ModuleRegistry(void)
{ // DESTRUCTOR
	return;
}

///////////////////
// PRIVATE METHODS
///////////////////

//{{{1 DXG DOC
/**
 * Find port by module name or module filename
 *
 * \param	&name the name to do a lookup on
 *
 * \param	type	the type of the name
 */
//}}}1 DXG DOC
std::string ModuleRegistry::findPort(std::string &name, NameType type)
{
	ModuleRegistryMapIterator itBegin = this->begin();
	ModuleRegistryMapIterator itEnd = this->end();

	// loop through the map of modules
	for (; itBegin != itEnd; itBegin++) {
		std::string idx = (*itBegin).first;
		std::string itName;

		// determine what we should compare to
		switch (type) {
			case MODNAME:
				itName = (*itBegin).second->getModName();
				break;
			case FILENAME:
				itName = (*itBegin).second->getFileName();
				break;
		}

		// return index string of the found module reg data object
		if (name.compare(itName) == 0)
			return idx;
	}

	// return 'idx not found'.
	return std::string("");
}

