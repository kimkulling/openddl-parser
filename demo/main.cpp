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
#include <iostream>
#include <cassert>
#include <openddlparser/OpenDDLParser.h>

USE_ODDLPARSER_NS;

static const char *FileOption = "--file";
static const int   Error = -1;
static void showhelp() {
    std::cout << "OpenDDL Parser Demo version " << OpenDDLParser::getVersion() << std::endl << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "\topenddl_parser_demo --file <filename>" << std::endl << std::endl;
    std::cout << "Parameter:" << std::endl;
    std::cout << "\t--file : The Name of the FILE to load." << std::endl;
}

int main( int argc, char *argv[] ) {
    if( argc < 3 ) {
        showhelp();
        return Error;
    }

    char *filename( nullptr );
    if( 0 == strncmp( FileOption, argv[ 1 ], strlen( FileOption ) ) ) {
        filename = argv[ 2 ];
    }

    std::cout << "file to import: " << filename << std::endl;
    
    if( nullptr == filename ) {
        std::cerr << "Invalid filename." << std::endl;
        return Error;
    }

    FILE *fileStream = fopen( filename, "r+" );
    if( NULL == filename ) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return Error;
    }

    // obtain file size:
    fseek( fileStream, 0, SEEK_END );
    const size_t size( ftell( fileStream ) );
    
    rewind( fileStream );
    
    if( size > 0 ) {
        char *buffer = new char[ size ];
        const size_t readSize( fread( buffer, sizeof( char), size, fileStream ) );
        assert( readSize == size );
        OpenDDLParser theParser;
        theParser.setBuffer( buffer, size );
        const bool result( theParser.parse() );
        if( !result ) {
            std::cerr << "Error while parsing file " << filename << "." << std::endl;
        }
    }

    return 0;
}
