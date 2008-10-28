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
 * \file	modulefactory.tcc
 * Declares and implements the template class ModuleFactory
 * The template class ModuleFactory implements the modules creation upon
 * connection request.
 *
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 */

#ifndef __MODULEFACTORY_H_
#define __MODULEFACTORY_H_ 1

// C++ Headers
#include <string>

// Project Headers
#include "defs.h"
#include "modulefactorybase.h"


DECEPTION_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////
// DECLARATION
//////////////////////////////////////////////////////////////////////////

//{{{1 DXG DOC
/**
 * \class	ModuleFactory
 *
 * This class parametrized for a specific module class will take care of
 * registration and later on creation of the requested module that is to
 * simulate a vulnerability.
 */
//}}}1 DXG DOC
template <typename T> class ModuleFactory : public ModuleFactoryBase
{
	public:
		Module* createModObject(void) const;

		~ModuleFactory(void);
		ModuleFactory(std::string modName);
		
	private:
		// hidden
		ModuleFactory(void);
		ModuleFactory(const ModuleFactory &rCopy);

};


//////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION
//////////////////////////////////////////////////////////////////////////

//{{{1 DXG DOC
/**
 * Creates a module object from this parametrized class instance
 *
 * \todo	What possible return values does \c new have ?
 * 			needed for dxg doc retval.
 *
 * \return	Pointer to the new object
 */
//}}}1 DXG DOC
template <typename T> Module* ModuleFactory<T>::createModObject(void) const
{
	return new T;
}

//{{{1 DXG DOC
/**
 * Creates a ModuleFactory object and passes the module name to its
 * base class for auto-registration
 *
 * \param	modName	the module name that this factory should be registered to.
 */
//}}}1 DXG DOC
template <typename T> ModuleFactory<T>::ModuleFactory(std::string modName)
	:
	ModuleFactoryBase(modName)
{ // CONSTRUCTOR
}

//{{{1 DXG DOC
/**
 * Does nothing useful.
 */
//}}}1 DXG DOC
template <typename T> ModuleFactory<T>::~ModuleFactory(void)
{ // DESTRUCTOR
	std::cerr << "in ModuleFactrory dtor\n";
}

DECEPTION_NAMESPACE_END

#endif // __MODULEFACTORY_H_
