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

#include <cassert>
#include <iostream>
#include <sstream>
#include <algorithm>

#ifdef _WIN32
#  include <windows.h>
#endif // _WIN32

BEGIN_ODDLPARSER_NS

static const char *Version = "0.1.0";

static const char* PrimitiveTypeToken[ ddl_types_max ] = {
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

static const char *BoolTrue  = "true";
static const char *BoolFalse = "false";
static const char *RefToken  = "ref";

static void logInvalidTokenError( char *in, char *exp, OpenDDLParser::logCallback callback ) {
    std::stringstream stream;
    stream << "Invalid token " << *in << ", " << exp << " expected." << std::endl;
    callback( ddl_error_msg, stream.str() );
}

static bool isIntegerType( PrimitiveDataType integerType ) {
    if( integerType != ddl_int8 && integerType != ddl_int16 && integerType != ddl_int32 && integerType != ddl_int64 ) {
        return false;
    }

    return true;
}

static DDLNode *createDDLNode( Identifier *id, Property *first, OpenDDLParser *parser ) {
    if( nullptr == id || nullptr == parser ) {
        return nullptr;
    }

    const std::string type( id->m_buffer );
    DDLNode *parent( parser->top() );
    DDLNode *node = new DDLNode( type, "", parent );
    if( nullptr != first ) {
        node->setProperties( first );
    }

    return node;
}

PrimData *PrimDataAllocator::allocPrimData( PrimitiveDataType type, size_t len ) {
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

void PrimDataAllocator::releasePrimData( PrimData **data ) {
    if( !data ) {
        return;
    }

    delete *data;
    *data = nullptr;
}

template<class T>
inline
T *getNextToken( T *in, T *end ) {
    while( ( isSpace( *in ) || isNewLine( *in ) || ',' == *in ) && ( in != end ) ) {
        in++;
    }
    return in;
}

void PrimData::setBool( bool value ) {
    assert( ddl_bool == m_type );
    ::memcpy( m_data, &value, m_size );
}

bool PrimData::getBool() {
    assert( ddl_bool == m_type );
    return ( bool ) ( *m_data );
}

void PrimData::setInt8( int8_t value ) {
    assert( ddl_int8 == m_type );
    ::memcpy( m_data, &value, m_size );
}

int8_t PrimData::getInt8() {
    assert( ddl_int8 == m_type );
    return ( int8_t ) ( *m_data );
}

void PrimData::setInt16( int16_t value ) {
    assert( ddl_int16 == m_type );
    ::memcpy( m_data, &value, m_size );
}

int16_t PrimData::getInt16() {
    assert( ddl_int16 == m_type );
    return ( int16_t ) ( *m_data );
}

void PrimData::setInt32( int32_t value ) {
    assert( ddl_int32 == m_type );
    ::memcpy( m_data, &value, m_size );
}

int32_t PrimData::getInt32() {
    assert( ddl_int32 == m_type );
    return ( int32_t ) ( *m_data );
}

void PrimData::setInt64( int64_t value ) {
    assert( ddl_int32 == m_type );
    ::memcpy( m_data, &value, m_size );
}

int64_t PrimData::getInt64() {
    return ( int64_t ) ( *m_data );
}

void PrimData::setFloat( float value ) {
    assert( ddl_float == m_type );
    ::memcpy( m_data, &value, m_size );
}

float PrimData::getFloat() const {
    float v;
    ::memcpy( &v, m_data, m_size );
    return v;
}

void PrimData::setNext( PrimData *next ) {
    m_next = next;
}

PrimData *PrimData::getNext() const {
    return m_next;
}

void logMessage( LogSeverity severity, const std::string &msg ) {
    std::string log;
    if( ddl_debug_msg == severity ) {
        log += "Debug:";
    }
    else if( ddl_info_msg == severity ) {
        log += "Info :";
    }
    else if( ddl_warn_msg == severity ) {
        log += "Warn :";
    }
    else if( ddl_error_msg == severity ) {
        log += "Error:";
    }
    else {
        log += "None :";
    }

    log += msg;
    std::cout << log;
}

OpenDDLParser::OpenDDLParser()
: m_logCallback( logMessage )
, m_ownsBuffer( false )
,m_buffer( nullptr )
, m_len( 0 )
, m_root( nullptr )
, m_stack() {
    // empty
}

OpenDDLParser::OpenDDLParser( char *buffer, size_t len, bool ownsIt )
: m_logCallback( &logMessage )
, m_ownsBuffer( false )
, m_buffer( nullptr )
, m_len( 0 )
, m_root( nullptr ) {
    if( 0 != m_len ) {
        setBuffer( buffer, len, ownsIt );
    }
}

OpenDDLParser::~OpenDDLParser() {
    clear();
}

void OpenDDLParser::setLogCallback( logCallback callback ) {
    if( nullptr != callback ) {
        // install user-specific log callback
        m_logCallback = callback;
    } else {
        // install default log callback
        m_logCallback = &logMessage;
    }
}

OpenDDLParser::logCallback OpenDDLParser::getLogCallback() const {
    return m_logCallback;
}

void OpenDDLParser::setBuffer( char *buffer, size_t len, bool ownsIt ) {
    if( m_buffer && m_ownsBuffer ) {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_len = 0;
    }

    m_ownsBuffer = ownsIt;
    if( m_ownsBuffer ) {
        // when we are owning the buffer we will do a deep copy
        m_buffer = new char[ len ];
        m_len = len;
        ::memcpy( m_buffer, buffer, len );
    } else {
        // when we are not owning the buffer, we just do a shallow copy
        m_buffer = buffer;
        m_len = len;
    }
}

char *OpenDDLParser::getBuffer() const {
    return m_buffer;
}

size_t OpenDDLParser::getBufferSize() const {
    return m_len;
}

void OpenDDLParser::clear() {
    if( m_ownsBuffer ) {
        delete[] m_buffer;
    }
    m_buffer = nullptr;
    m_len = 0;
    
    delete m_root;
    m_root = nullptr;
}

bool OpenDDLParser::parse() {
    if( 0 == m_len ) {
        return false;
    }
    
    // remove comments
    normalizeBuffer( m_buffer, m_len );

    m_root = new DDLNode( "root", "", nullptr );
    pushNode( m_root );

    // do the main parsing
    char *current( &m_buffer[ 0 ] );
    char *end( &m_buffer[ m_len - 1 ] + 1 );
    while( current != end ) {
        current = parseNextNode( current, end );
    }
    return true;
}

char *OpenDDLParser::parseNextNode( char *in, char *end ) {
    in = parseHeader( in, end );
    in = parseStructure( in, end );

    return in;

}

char *OpenDDLParser::parseHeader( char *in, char *end ) {
    if( nullptr == in || in == end ) {
        return in;
    }

    Identifier *id( nullptr );
    in = OpenDDLParser::parseIdentifier( in, end, &id );
    in = getNextToken( in, end );
    Property *first( nullptr );
    if( nullptr != id ) {
        if( *in == '(' ) {
            in++;
            Property *prop( nullptr ), *prev( nullptr );
            while( *in != ')' && in != end ) {
                in = parseProperty( in, end, &prop );
                in = getNextToken( in, end );

                if( *in != ',' && *in != ')' ) {
                    logInvalidTokenError( in, ")", m_logCallback );
                    return in;
                }
                if( nullptr != prop && *in != ',' ) {
                    if( nullptr == first ) {
                        first = prop;
                    }
                    if( nullptr != prev ) {
                        prev->m_next = prop;
                    }
                    prev = prop;
                }
            }
            in++;
        }

        // store the node
        DDLNode *node( createDDLNode( id, first, this ) );
        if( nullptr != node ) {
            pushNode( node );
        } else {
            std::cerr << "nullptr returned by creating DDLNode." << std::endl;
        }

        Name *name( nullptr );
        in = OpenDDLParser::parseName( in, end, &name );
        if( nullptr != name ) {
            const std::string nodeName( name->m_id->m_buffer );
            node->setName( nodeName );
        }
    }

    return in;
}

char *OpenDDLParser::parseStructure( char *in, char *end ) {
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    if( *in == '{' ) {
        in++;
        in = getNextToken( in, end );
        PrimitiveDataType type( ddl_none );
        size_t arrayLen( 0 );
        in = OpenDDLParser::parsePrimitiveDataType( in, end, type, arrayLen );
        if ( ddl_none != type ) {
            in = getNextToken( in, end );
            if( *in == '{' ) {
                PrimData *primData( nullptr );
                in = parseDataList( in, end, &primData );
            }

            in = getNextToken( in, end );
            if( *in != '}' ) {
                logInvalidTokenError( in, "}", m_logCallback );
            }
        } else {
            in = parseHeader( in, end );
            in = parseStructure( in, end );
        }
    } else {
        in++;
        logInvalidTokenError( in, "{", m_logCallback );
        return in;

    }

    in++;

    return in;
}

void OpenDDLParser::pushNode( DDLNode *node ) {
    if( nullptr == node ) {
        return;
    }

    m_stack.push_back( node );
}

DDLNode *OpenDDLParser::popNode() {
    if( m_stack.empty() ) {
        return nullptr;
    }

    DDLNode *top( top() );
    m_stack.pop_back();

    return top;
}

DDLNode *OpenDDLParser::top() {
    if( m_stack.empty() ) {
        return nullptr;
    }
    
    DDLNode *top( m_stack.back() );
    return top;
}

DDLNode *OpenDDLParser::getRoot() const {
    return m_root;
}

void OpenDDLParser::normalizeBuffer( char *buffer, size_t len ) {
    if( nullptr == buffer || 0 == len ) {
        return;
    }

    size_t writeIdx( 0 );
    char *end( &buffer[ len ] + 1 );
    for( size_t readIdx = 0; readIdx<len; ++readIdx ) {
        char *c( &buffer[readIdx] );
        // check for a comment
        if( !isComment<char>( c, end ) ) {
            buffer[ writeIdx ] = buffer[ readIdx ];
            writeIdx++;
        } else {
            readIdx++;
            // skip the comment and the rest of the line
            while( !isEndofLine( buffer[ readIdx ] ) ) {
                readIdx++;
            }
            buffer[writeIdx] = '\n';
            writeIdx++;
        }
    }

    if( writeIdx < len ) {
        buffer[ writeIdx ] = '\0';
    }
}

char *OpenDDLParser::parseName( char *in, char *end, Name **name ) {
    *name = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    // ignore blanks
    in = getNextToken( in, end );
    if( *in != '$' && *in != '%' ) {
        return in;
    }

    NameType ntype( GlobalName );
    if( *in == '%' ) {
        ntype = LocalName;
    }

    Name *currentName( nullptr );
    Identifier *id( nullptr );
    in = parseIdentifier( in, end, &id );
    if( id ) {
        currentName = new Name( ntype, id );
        if( currentName ) {
            *name = currentName;
        }
    }
    
    return in;
}

char *OpenDDLParser::parseIdentifier( char *in, char *end, Identifier **id ) {
    *id = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    // ignore blanks
    in = getNextToken( in, end );
    
    // staring with a number is forbidden
    if( isNumeric<const char>( *in ) ) {
        return in;
    }

    // get size of id
    size_t idLen( 0 );
    char *start( in );
    while( !isSeparator( *in ) && ( in != end ) && *in != '(' && *in != ')' ) {
        in++;
        idLen++;
    }
    
    const size_t len( idLen + 1 );
    Identifier *newId = new Identifier( len, new char[ len ] );
    ::strncpy( newId->m_buffer, start, newId->m_len-1 );
    newId->m_buffer[ newId->m_len - 1 ] = '\0';
    *id = newId;

    return in;
}

char *OpenDDLParser::parsePrimitiveDataType( char *in, char *end, PrimitiveDataType &type, size_t &len ) {
    type = ddl_none;
    len = 0;
    if( nullptr == in || in == end ) {
        return in;
    }

    for( unsigned int i = 0; i < ddl_types_max; i++ ) {
        const size_t prim_len( strlen( PrimitiveTypeToken[ i ] ) );
        if( 0 == strncmp( in, PrimitiveTypeToken[ i ], prim_len ) ) {
            type = ( PrimitiveDataType ) i;
            break;
        }
    }

    if( ddl_none == type ) {
        in = getNextToken( in, end );
        return in;
    } else {
        in += strlen( PrimitiveTypeToken[ type ] );
    }

    bool ok( true );
    if( *in == '[' ) {
        ok = false;
        in++;
        char *start( in );
        while ( in != end ) {
            in++;
            if( *in == ']' ) {
                len = atoi( start );
                ok = true;
                in++;
                break;
            }
        }
    }
    if( !ok ) {
        type = ddl_none;
    }

    return in;
}

char *OpenDDLParser::parseReference( char *in, char *end, std::vector<Name*> &names ) {
    if( nullptr == in || in == end ) {
        return in;
    }

    if( 0 != strncmp( in, RefToken, strlen( RefToken ) ) ) {
        return false;
    } else {
        const size_t refTokenLen( strlen( RefToken ) );
        in += refTokenLen;
    }

    in = getNextToken( in, end );
    if( '{' != *in ) {
        return in;
    } else {
        in++;
    }

    in = getNextToken( in, end );
    Name *nextName( nullptr );
    in = parseName( in, end, &nextName );
    if( nextName ) {
        names.push_back( nextName );
    }
    while( '}' != *in ) {
        in = getNextSeparator( in, end );
        if( ',' == *in ) {
            in = parseName( in, end, &nextName );
            if( nextName ) {
                names.push_back( nextName );
            }
        } else {
            break;
        }
    }

    return in;
}

char *OpenDDLParser::parseBooleanLiteral( char *in, char *end, PrimData **boolean ) {
    *boolean = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    char *start( in );
    size_t len( 0 );
    while( !isSeparator( *in ) && in != end ) {
        in++;
        len++;
    }
    len++;
    int res = ::strncmp( BoolTrue, start, strlen( BoolTrue ) );
    if( 0 != res ) {
        res = ::strncmp( BoolFalse, start, strlen( BoolFalse ) );
        if( 0 != res ) {
            *boolean = nullptr;
            return in;
        }
        *boolean = PrimDataAllocator::allocPrimData( ddl_bool );
        (*boolean)->setBool( false );
    } else {
        *boolean = PrimDataAllocator::allocPrimData( ddl_bool );
        (*boolean)->setBool( true );
    }

    return in;
}

char *OpenDDLParser::parseIntegerLiteral( char *in, char *end, PrimData **integer, PrimitiveDataType integerType ) {
    *integer = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    if( !isIntegerType( integerType ) ) {
        return in;
    }

    in = getNextToken( in, end );
    char *start( in );
    while( !isSeparator( *in ) && in != end ) {
        in++;
    }

    if( isNumeric( *start ) ) {
        const int value( atoi( start ) );
        *integer = PrimDataAllocator::allocPrimData( integerType );
        switch( integerType ) {
            case ddl_int8:
                ( *integer )->setInt8( (int8_t) value );
                break;
            case ddl_int16:
                ( *integer )->setInt16( ( int16_t ) value );
                break;
            case ddl_int32:
                ( *integer )->setInt32( ( int32_t ) value );
                break;
            case ddl_int64:
                ( *integer )->setInt64( ( int64_t ) value );
                break;
            default:
                break;
        }
    } 

    return in;
}

char *OpenDDLParser::parseFloatingLiteral( char *in, char *end, PrimData **floating ) {
    *floating = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    char *start( in );
    while( !isSeparator( *in ) && in != end ) {
        in++;
    }
    if( isNumeric( *start ) ) {
        const float value( (float) atof( start ) );
        *floating = PrimDataAllocator::allocPrimData( ddl_float );
        ( *floating )->setFloat( value );
    }

    return in;
}

char *OpenDDLParser::parseStringLiteral( char *in, char *end, PrimData **stringData ) {
    *stringData = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    size_t len( 0 );
    char *start( in );
    if( *start == '\"' ) {
        start++;
        in++;
        while( *in != '\"' && in != end ) {
            in++;
            len++;
        }

        *stringData = PrimDataAllocator::allocPrimData( ddl_string, len+1 );
        ::strncpy( ( char* ) ( *stringData )->m_data, start, len );
        ( *stringData )->m_data[len] = '\0';
        in++;
    }

    return in;
}

static void createPropertyWithData( Identifier *id, PrimData *primData, Property **prop ) {
    if( nullptr != primData ) {
        ( *prop ) = new Property( id );
        ( *prop )->m_primData = primData;
    }
}

char *OpenDDLParser::parseProperty( char *in, char *end, Property **prop ) {
    *prop = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    Identifier *id( nullptr );
    in = parseIdentifier( in, end, &id );
    if( nullptr != id ) {
        in = getNextToken( in, end );
        if( *in == '=' ) {
            in++;
            in = getNextToken( in, end );
            PrimData *primData( nullptr );
            if( isInteger( in, end ) ) {
                in = parseIntegerLiteral( in, end, &primData );
                createPropertyWithData( id, primData, prop );
            } else if( isFloat( in, end ) ) {
                in = parseFloatingLiteral( in, end, &primData );
                createPropertyWithData( id, primData, prop );
            } else if( isStringLiteral( *in ) ) { // string data
                in = parseStringLiteral( in, end, &primData );
                createPropertyWithData( id, primData, prop );
            } else {                          // reference data
                std::vector<Name*> names;
                in = parseReference( in, end, names );
                if( !names.empty() ) {
                    Reference *ref = new Reference( names.size(), &names[ 0 ] );
                    ( *prop ) = new Property( id );
                    ( *prop )->m_ref = ref;
                }
            }
        } 
    }

    return in;
}

char *OpenDDLParser::parseDataList( char *in, char *end, PrimData **data ) {
    *data = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    if( *in == '{' ) {
        in++;
        PrimData *first( nullptr ), *prev( nullptr ), *current( nullptr );
        while( '}' != *in ) {
            current = nullptr;
            in = getNextToken( in, end );
            if( isInteger( in, end ) ) {
                in = parseIntegerLiteral( in, end, &current );
            } else if( isFloat( in, end ) ) {
                in = parseFloatingLiteral( in, end, &current );
            } else if( isStringLiteral( *in ) ) {
                in = parseStringLiteral( in, end, &current );
            } else if( *in == '{' ) {
                in = parseDataList( in, end, &current );
            }

            if( nullptr != current ) {
                *data = current;
                if( nullptr == first ) {
                    first = *data;
                }
                if( nullptr != prev ) {
                    prev->setNext( *data );
                }
                prev = *data;
            }

            in = getNextSeparator( in, end );
            if( ',' != *in && '}' != *in && !isSpace( *in ) ) {
                break;
            }
        }
        in++;
        *data = first;
    }

    return in;
}

char *OpenDDLParser::parseDataArrayList( char *in, char *end, PrimData **data ) {
    if( nullptr == in || in == end ) {
        return in;
    }

    in = getNextToken( in, end );
    PrimitiveDataType type( ddl_none );
    size_t len( 0 );
    in = parsePrimitiveDataType( in, end, type, len );
    if( ddl_none != type ) {
        in = getNextToken( in, end );
        if( *in == '{' ) {
            do {
                PrimData *prev( nullptr ), *next( nullptr );
                in = parseDataList( in, end, &next );
                if( nullptr != next ) {
                    if( nullptr == *data ) {
                        *data = next;
                        prev = *data;
                    } else {
                        if( nullptr != prev ) {
                            prev->m_next = next;
                        }
                    }
                }
                in = getNextToken( in, end );
            } while( ',' == *in && in != end );
        }
    }

    return in;
}

const char *OpenDDLParser::getVersion() {
    return Version;
}

END_ODDLPARSER_NS
