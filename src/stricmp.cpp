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
 * \file    stricmp.cpp
 * \author  alexis hildebrandt <afh@2drop.net>
 *
 * This files wraps stricmp and strnicmp around their strcasecmp
 * fellows. stricmp() is needed by Xerces C++ under Solaris for some weird
 * reason. So this is a little workaround.
 *
 */

#ifndef Darwin
#include <strings.h>

//{{{1 DXG DOC
/**
 * Proxy function for strcasecmp, since stricmp does not seem to be portable.
 *
 * \param	s1	string 1
 * \param	s2	string 2
 */
//}}}1 DXG DOC
int stricmp(const char *const s1, const char *const s2)
{
	return strcasecmp(s1,s2);
}

//{{{1 DXG DOC
/**
 * Proxy function for strncasecmp, since strnicmp does not seem to be portable.
 *
 * \param	s1	string 1
 * \param	s2	string 2
 * \param	n	count
 */
//}}}1 DXG DOC
int strnicmp(const char *const s1, const char *const s2, const unsigned int n)
{
	return strncasecmp(s1,s2,n);
}
#endif
