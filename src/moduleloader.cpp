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
 * \file	moduleloader.cpp
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file implements the easy interface provided by ModuleLoader
 *
 */

// Project Headers
#include "moduleloader.h"
#include <unistd.h>

DECEPTION_NAMESPACE_USE;

//define static members
std::string ModuleLoader::moduleDir = "";
std::string ModuleLoader::className = "ModuleLoader";

//{{{1 DXG DOC
/**
 * Set the module directory path.
 *
 * The configuration facility uses this method to set the module directory.
 *
 * \param	modDir	Directory where modules reside
 */
//}}}1 DXG DOC
void ModuleLoader::setModDir(std::string modDir)
{
	this->moduleDir = modDir;
	if (access(modDir.c_str(), R_OK) == -1) {
		switch (errno) {
			case EACCES:
				throw IOException("Read access to module directory denied");
				break;
			case ENOENT:
				throw IOException("Module directory doesn't exist");
				break;
			case ENOTDIR:
				throw IOException("Module directory is not a directory");
				break;
			default:
				throw IOException("Module directory inaccessible");
				break;
		}
	}
	return;
}

//{{{1 DXG DOC
/**
 *
 * After the configuration file has been read the daemon process
 * needs to preload all modules, so it knows which modules have
 * registered. This is also has the benefit, that unresolved symbols
 * within the modules can be detected before an actual simulation is
 * supposed to happen.
 *
 */
//}}}1 DXG DOC
void ModuleLoader::loadAllModules(void)
{
	ModuleRegistry::ModuleRegistryMapIterator itBegin = this->store->begin();
	ModuleRegistry::ModuleRegistryMapIterator itEnd = this->store->end();
	std::string itModFileName;
	// loop through the registry
	for (; itBegin != itEnd; itBegin++) {
		itModFileName = (*itBegin).second->getFileName();

		// try to open
		try {
			this->open(itModFileName);
		}
		catch (Exception &e) {
			// remove that module
			std::string logMsg = e.toString();
			globLog.toLog(this->className, Error, logMsg);

			this->store->remove(itModFileName);
			this->close(itModFileName);
			itBegin = this->store->begin();
		}
	}
	return;
}

//{{{1 DXG DOC
/**
 * Opens a shared object file and loads its code into this program for
 * later execution.
 *
 * \param	modFileName Filename of the module to be opened
 */
//}}}1 DXG DOC
void ModuleLoader::open(std::string modFileName)
{
	// definition
	void *handle = NULL;
	// OR with RTLD_GLOBAL to enable export of this modules symbols to
	// subsequently loaded modules.
	int flags = RTLD_NOW;//| RTLD_GLOBAL;
	std::string modulePath = this->moduleDir + modFileName;

	// open the shared object and throw exception upon failure
	handle = ::dlopen(modulePath.c_str(), flags);
	if (handle == NULL) {
		throw NullPointerException(::dlerror());
	}

	// the framework needs to store the handle for later closure of the
	// shared object.
	this->store->insert(modFileName, handle);

	return;
}

//{{{1 DXG DOC
/**
 * Closes a shared object file and deletes all registration data, to
 * make sure no other object tries to use this modules code.
 *
 * \todo	The module registry data is not removed from the registry!
 *
 * \todo	What should be done when there was no valid module handle
 * 			stored within the registry.
 * \todo	In case \c ::dlclose() return with an error, how should this
 * 			be handled ? use some personal exception ?
 *
 * \bug		gcc 3.2 on OpenBSD 3.2 sigsegvs upon calling ::dlclose()
 * 			see source code moduleloader.cpp for details.
 *
 * \param	modFileName	Filename of the module to be closed
 */
//}}}1 DXG DOC
void ModuleLoader::close(std::string modFileName)
{
	// definition
	void *handle;

	// initalisation
	handle = NULL;

	// retrieve the module handler for the module
	handle = this->store->getHandle(modFileName);
	if (handle == NULL)
		throw NullPointerException("handle not stored in registry!");


	// delete all registry data
	//this->store->remove(modFileName);


// suppress icc warning of non-existent __GNUC__
#ifndef __GNUC__
#define __GNUC__ 0
#endif

//#undef OpenBSD	// uncomment only when trying to address the below mentioned problem !
#if defined OpenBSD && __GNUC__ > 2
	/*
	 * gcc 3.2 on OpenBSD 3.2 sigesgvs upon ::dlcose() for some weird reason.
	 * the handle variable seems to have a valid value .
	 * maybe it has something to do with the text relocations of the
	 * ModuleFactoryBase dtor? see ld output below.

--- BEGIN LD OUTPUT ---
% eg++  -shared -fpic modules/testmodule1.o -o modules/testmodule1.so
ld: modules/testmodule1.o: RRS text relocation at 0x696 for "operator delete(void*)"
ld: modules/testmodule1.o: RRS text relocation at 0x67c for "ModuleFactoryBase::~ModuleFactoryBase [not-in-charge]()"
ld: Total: 45 RRS text relocations
ld: modules/testmodule1.o: RRS text relocation at 0x696 for "__ZdlPv"
ld: modules/testmodule1.o: RRS text relocation at 0x67c for "__ZN17ModuleFactoryBaseD2Ev"
ld: Total: 45 RRS text relocations
--- END LD OUTPUT ---

	 */
#else
	// close the shared object
	//std::cerr << "calling ::dlclose(" << handle << ")\n";
	if (::dlclose(handle) != 0) {
		// TODO: how should this error be handled ? own exception ?
		throw DlCloseException("error closing handle");
	}
#endif

	return;
}

//{{{1 DXG DOC
/**
 * Creates a ModuleLoader object, and allocates a new \c ModuleRegistry
 * for the private member \c this->store.
 */
//}}}1 DXG DOC
ModuleLoader::ModuleLoader(void)
	:
	store(new ModuleRegistry)
{ // DEFAULT CONSTRUCTOR
	return;
}

//{{{1 DXG DOCS
/**
 * Destroys a ModuleLoader object and deallocates the memory used by \c
 * this->store.
 */
//}}}1 DXG DOC
ModuleLoader::~ModuleLoader(void)
{ // DESTRUCTOR
	delete(this->store);
	return;
}

