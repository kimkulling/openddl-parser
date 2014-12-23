/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014 Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once
#ifndef OPENDDLPARSER_OPENDDLPARSERCOMMON_H_INC
#define OPENDDLPARSER_OPENDDLPARSERCOMMON_H_INC

#include <cstddef>
#include <string.h>

#ifdef _WIN32
#   define TAG_DLL_EXPORT __declspec(dllexport)
#   define TAG_DLL_IMPORT __declspec(dllimport )
#   ifdef OPENDDLPARSER_BUILD
#       define DLL_ODDLPARSER_EXPORT TAG_DLL_EXPORT
#   else
#        define DLL_ODDLPARSER_EXPORT TAG_DLL_IMPORT
#   endif
#   pragma warning( disable : 4251 )
#else
#   define DLL_ODDLPARSER_EXPORT
#endif
#define BEGIN_ODDLPARSER_NS namespace ODDLParser {
#define END_ODDLPARSER_NS   }
#define USE_ODDLPARSER_NS   using namespace ODDLParser;

#define ODDL_NO_COPYING( classname ) \

#endif // OPENDDLPARSER_OPENDDLPARSERCOMMON_H_INC

