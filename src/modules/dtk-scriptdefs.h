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
 * \file	dtk-scriptdefs.h
 * \brief	contains common definitions for the DtkScript module
 * \author	alexis hildebrandt <afh@2drop.net>
 *
 * This file declares some makros the use of the Dtkscript namespace.
 *
 */

// Project Headers
#include "defs.h"
#include "logging.h"

//{{{1 DXG DOC
/**
 * \namespace	Dtkscript
 * \brief		Namespace for the dtk script module
 *
 * The \c Dtkscript namespace groups the symbols needed for the Dtk
 * Script Compatability.  This is just a small safey hook to avoid two 
 * symbols of the same name colliding at runtime.  Depending on the
 * compiler multiply defined symbols still may occur.
 */
//}}}1 DXG DOC
/// namespace for the dtk script module
#define DTKSCRIPT_NAMESPACE_NAME Dtkscript
/// begin namespace definition
#define DTKSCRIPT_NAMESPACE_BEGIN namespace DTKSCRIPT_NAMESPACE_NAME {
/// end namespace definition
#define DTKSCRIPT_NAMESPACE_END }
/// use namespace definition
#define DTKSCRIPT_NAMESPACE_USE using namespace DTKSCRIPT_NAMESPACE_NAME


DECEPTION_NAMESPACE_USE;

//{{{1 DXG DOC
/**
 * \brief	the global logging facility
 *
 * this variable enables every module to log through the frameworks
 * logging facility.
 */
//}}}1 DXG DOC
extern Logging globLog;

//{{{1 DXG DOC
/**
 * \brief	the module's name
 *
 * Since the Dtk-Script Compatability module consists of several object
 * files, this variable is exported to all of the files scope.
 */
//}}}1 DXG DOC
extern std::string moduleName;
