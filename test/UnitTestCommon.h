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
#ifndef OPENDDLPARSER_TESTUTILITIES_H_INC
#define OPENDDLPARSER_TESTUTILITIES_H_INC

#include <openddlparser/OpenDDLCommon.h>
#include <openddlparser/OpenDDLParser.h>

BEGIN_ODDLPARSER_NS

inline 
char *findEnd( char *in, size_t &len ) {
    len = strlen( in );
    char *end( &in[ len ] );

    return end;
}

inline
size_t countItems( Value *data ) {
    if( nullptr == data ) {
        return 0;
    }
    size_t numItems( 1 );
    while( nullptr != data->getNext() ) {
        numItems++;
        data = data->getNext();
    }
    return numItems;
}

END_ODDLPARSER_NS

#endif // OPENDDLPARSER_TESTUTILITIES_H_INC
