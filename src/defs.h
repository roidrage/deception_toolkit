#ifndef _DEFS_H
#define _DEFS_H
/**
 * \file defs.h
 * 
 * \author Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains common declarations for the deception framework.
 * Currently only namespace makros are defined
 */

//{{{1 DXG DOC
/**
 * \namespace	Deception
 *
 * The \c Deception namespace groups the symbols needed by the
 * framework together. Multiple definitions may still be possible
 * depending on the compiler that is used.
 */
//}}}1 DXG DOC
/// name of our namespace
#define DECEPTION_NAMESPACE_NAME Deception
/// begin code, that belongs to our namespace
#define DECEPTION_NAMESPACE_BEGIN namespace DECEPTION_NAMESPACE_NAME {
/// end namespace code
#define DECEPTION_NAMESPACE_END }
// macro to use es identifier when working with types, functions or 
// whatever included in the namespace
// XXX: it's not useful, since this looks bad
// 		DECEPTION_NAMESPACELogging, so it's better to use
// 		DECEPTION_NAMESPACE_NAME::Logging, or is it?
//#define DECEPTION_NAMESPACE DECEPTION_NAMESPACE_NAME::	

// define using macro, it's fraund upon, but always think of other clients
// using this framework and maybe they prefer using to writing the
// namespace name everytime
/// use namespace definition
#define DECEPTION_NAMESPACE_USE using namespace DECEPTION_NAMESPACE_NAME;

#endif // _DEFS_H
