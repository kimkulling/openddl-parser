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
#include <openddlparser/OpenDDLParser.h>

BEGIN_ODDLPARSER_NS

template<class T>
inline
bool isValidToken( std::string &token, size_t index ) {
    return true;
}

int getNextToken(const std::vector<char> &buffer, size_t index) {
    return true;
}

bool parse( const std::vector<char> &buffer ) {
    size_t index( 0 );
    const size_t buffersize( buffer.size() );
    while( index < buffersize ) {
        int next = getNextToken( buffer, index );
        if( -1 == next ) {
            break;
        }

        index = ( int ) next;
    }
    return true;
}

bool parseDataList( const std::vector<char> &buffer, size_t index ) {
    return true;
}

bool parseName() {
    return true;
}

bool parseDataArrayList( const std::vector<char> &buffer, size_t index ) {
    return true;
}

bool parseIdentifier() {
    return true;
}

bool parsePrimitiveDataType( const std::vector<char> &buffer, size_t index ) {
    return true;
}


bool parseReference() {
    return true;
}

bool parseProperty() {
    return true;
}

OpenDDLParser::OpenDDLParser() {
}

OpenDDLParser::~OpenDDLParser() {
}

END_ODDLPARSER_NS
