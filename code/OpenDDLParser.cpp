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

#ifdef _WIN32
#  include <windows.h>
#endif // _WIN32

BEGIN_ODDLPARSER_NS

static const char* PrimitiveTypes[ ddl_types_max ] = {
    "bool",
    "int8",
    "int16",
    "int32",
    "int64",
    "unsigned_int8",
    "unsigned_int16",
    "unsigned_int32",
    "unsigned_int64",
    "half",
    "float",
    "double",
    "string",
    "ref"
};


template<typename T, typename... Args>
void debugLog( const char *s, T value, Args... args ) {
    while( *s ) {
        if( *s == '%' ) {
            if( *( s + 1 ) == '%' ) {
                ++s;
            } else {
                std::cout << value;
                s += 2;
                printf( s, args... ); // call even when *s == 0 to detect extra arguments
//                ::OutputDebugString( buffer );
                return;
            }
        }
        std::cout << *s++;
    }
}

static PrimData *allocPrimData( PrimitiveDataType type, size_t len = 1 ) {
    if( type == ddl_none || ddl_types_max == type ) {
        return nullptr;
    }

    PrimData *data = new PrimData;
    data->m_type = type;
    switch( type ) {
        case ddl_bool:
            data->m_size = sizeof( bool );
            break;
        case ddl_int8:
            data->m_size = sizeof( char );
            break;
        case ddl_int16:
            data->m_size = sizeof( short );
            break;
        case ddl_int32:
            data->m_size = sizeof( int );
            break;
        case ddl_int64:
            data->m_size = sizeof( long );
            break;
        case ddl_unsigned_int8:
            data->m_size = sizeof( unsigned char );
            break;
        case ddl_unsigned_int32:
            data->m_size = sizeof( unsigned int );
            break;
        case ddl_unsigned_int64:
            data->m_size = sizeof( unsigned long );
            break;
        case ddl_half:
            data->m_size = sizeof( short );
            break;
        case ddl_float:
            data->m_size = sizeof( float );
            break;
        case ddl_double:
            data->m_size = sizeof( double );
            break;
        case ddl_string:
            data->m_size = sizeof( char );
            break;
        case ddl_ref:
            data->m_size = sizeof( char );
            break;
        case ddl_none:
        case ddl_types_max:
        default:
            break;
    }

    if( data->m_size ) {
        data->m_size *= len;
        data->m_data = new unsigned char[ data->m_size ];
    }

    return data;
}

static void releasePrimData( PrimData * data ) {
    if( !data ) {
        return;
    }

    delete data;
}

template<class T>
inline
bool isValidToken( std::string &token, size_t index ) {

    return true;
}

int getNextToken( const std::vector<char> &buffer, size_t index ) {
    return true;
}

OpenDDLParser::OpenDDLParser()
: m_buffer( nullptr )
, m_root( nullptr ) {
    // empty;
}

OpenDDLParser::OpenDDLParser( const std::vector<char> &buffer ) 
: m_buffer( &buffer )
, m_root( nullptr ) {
    // empty
}

OpenDDLParser::~OpenDDLParser() {
    delete m_root;
    m_root = nullptr;
}

bool OpenDDLParser::parse() {
    if( !m_buffer ) {
        return false;
    }

    size_t index( 0 );
    const size_t buffersize( m_buffer->size() );
    while( index < buffersize ) {
        int next = getNextToken( *m_buffer, index );
        if( -1 == next ) {
            break;
        }

        index = ( int ) next;
    }
    return true;
}

PrimData *OpenDDLParser::parsePrimitiveDataType( const char *in, size_t len, size_t &offset ) {
    if( !in ) {
        return nullptr;
    }

    PrimitiveDataType type( ddl_none );
    for( unsigned int i = 0; i < ddl_types_max; i++ ) {
        const size_t prim_len( strlen( PrimitiveTypes[ i ] ) );
        if( 0 == strncmp( in, PrimitiveTypes[ i ], prim_len ) ) {
            type = ( PrimitiveDataType ) i;
            break;
        }
    }

    if( ddl_none == type ) {
        offset += len;
        return nullptr;
    }
    else {
        offset += strlen( PrimitiveTypes[ type ] );
    }

    size_t size = 1;
    if( in[ offset ] == '[' ) {
        size = 0;
        size_t start( offset+1 );
        while( ( offset ) < len ) {
            offset++;
            if( in[ offset ] == ']' ) {
                size = atoi( &in[ start ] );
                break;
            }
        }
        if( size ) {
            len = size;
        }
    }

    if( !size ) {
        return nullptr;
    }
     
    PrimData *data = allocPrimData( type, len );
    if( !data ) {
        return nullptr;
    }
    return data;
}

bool OpenDDLParser::parseDataList( const std::vector<char> &buffer, size_t index ) {
    return false;
}

bool OpenDDLParser::parseName() {
    return false;
}

bool OpenDDLParser::parseDataArrayList( const std::vector<char> &buffer, size_t index ) {
    return false;
}

bool OpenDDLParser::parseIdentifier() {
    return false;
}

bool OpenDDLParser::parseReference() {
    return false;
}

bool OpenDDLParser::parseProperty() {
    return false;
}

bool OpenDDLParser::parseBoolean() {
    return false;
}

bool OpenDDLParser::parseInteger() {
    return false;
}

bool OpenDDLParser::parseFloatingNo() {
    return false;
}

bool OpenDDLParser::parseString() {
    return false;
}

DDLNode *OpenDDLParser::getRoot() const {
    return m_root;
}

END_ODDLPARSER_NS
