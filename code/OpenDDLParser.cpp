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

static const char *RefToken = "ref";

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

static bool isIntegerType( PrimitiveDataType integerType ) {
    if( integerType != ddl_int8 && integerType != ddl_int16 && integerType != ddl_int32 && integerType != ddl_int64 ) {
        return false;
    }

    return true;
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
    while( isSeparator( *in ) && ( in != end ) ) {
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

DDLNode::DDLNode( const std::string &name, DDLNode *parent )
: m_name( name )
, m_parent( parent )
, m_children() {
    if( m_parent ) {
        m_parent->m_children.push_back( parent );
    }
}

DDLNode::~DDLNode() {
    detachParent();
}

void DDLNode::attachParent( DDLNode *parent ) {
    if( m_parent == parent ) {
        return;
    }

    m_parent = parent;
    m_parent->m_children.push_back( parent );
}

void DDLNode::detachParent() {
    if( m_parent ) {
        std::vector<DDLNode*>::iterator it;
        it = std::find( m_parent->m_children.begin(), m_parent->m_children.end(), this );
        if( m_parent->m_children.end() != it ) {
            m_parent->m_children.erase( it );
        }
        m_parent = nullptr;
    }
}

DDLNode *DDLNode::getParent() const {
    return m_parent;
}

const DDLNode::DllNodeList &DDLNode::getChildNodeList() const {
    return m_children;
}


void DDLNode::setName( const std::string &name ) {
    m_name = name;
}

const std::string &DDLNode::getName() const {
    return m_name;
}

OpenDDLParser::OpenDDLParser()
: m_ownsBuffer( false )
,m_buffer( nullptr )
, m_len( 0 )
, m_root( nullptr ) {
    // empty
}

OpenDDLParser::OpenDDLParser( char *buffer, size_t len, bool ownsIt )
: m_ownsBuffer( false )
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

void OpenDDLParser::setBuffer( char *buffer, size_t len, bool ownsIt ) {
    if( m_buffer && m_ownsBuffer ) {
        delete[] m_buffer;
        m_buffer = nullptr;
        m_len = 0;
    }
    m_ownsBuffer = ownsIt;
    m_buffer = new char[ len ];
    m_len = len;
    ::memcpy( m_buffer, buffer, len );
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

    // do the main parsing
    const size_t buffersize( m_len );
    char *current( &m_buffer[ 0 ] );
    char *end( &m_buffer[ m_len - 1 ] + 1 );
    while( current != end ) {
        current = parseId( current, end );
    }
    return true;
}

char *OpenDDLParser::parseDataType( char *in, char *end ) {
    return in;
}

char *OpenDDLParser::parseId( char *in, char *end ) {
    if( nullptr == in || in == end ) {
        return in;
    }

    Identifier *id( nullptr );
    in = OpenDDLParser::parseIdentifier( in, end, &id );
    if( nullptr != id ) {
        Name *name( nullptr );
        in = OpenDDLParser::parseName( in, end, &name );
        if( nullptr != name ) {

        }
    }

    return in;
}

void OpenDDLParser::normalizeBuffer( char *buffer, size_t len ) {
    if( nullptr == buffer || 0 == len) {
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
        buffer[writeIdx] = '\0';
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
    while( !isSeparator( *in ) && ( in != end ) ) {
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

char *OpenDDLParser::parsePrimitiveDataType( char *in, char *end, PrimData **primData ) {
    *primData = nullptr;
    if( nullptr == in || in == end ) {
        return in;
    }

    PrimitiveDataType type( ddl_none );
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

    size_t size = 1;
    bool ok( true );
    if( *in == '[' ) {
        ok = false;
        in++;
        char *start( in );
        bool brackedClosed( false );
        while ( in != end ) {
            in++;
            if( *in == ']' ) {
                size = atoi( start );
                ok = true;
                break;
            }
        }
    }
    if( ok ) {
        *primData = PrimDataAllocator::allocPrimData( type, size );
    }

    return in;
}

char *OpenDDLParser::parseReference( char *in, char *end, std::vector<Name*> &names ) {
    if( nullptr == in || in == end ) {
        return in;
    }

    char *start( in );
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
    int res = ::strncmp( "true", start, len );
    if( 0 != res ) {
        res = ::strncmp( "false", start, len );
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
                ( *integer )->setInt8( value );
                break;
            case ddl_int16:
                ( *integer )->setInt16( value );
                break;
            case ddl_int32:
                ( *integer )->setInt32( value );
                break;
            case ddl_int64:
                ( *integer )->setInt64( value );
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
        const float value( atof( start ) );
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
    }

    return in;
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
            if( isNumeric( *in ) ) {        // numeric data
                in = parseIntegerLiteral( in, end, &primData );
                if( nullptr != primData ) {
                    ( *prop ) = new Property( id );
                    ( *prop )->m_primData = primData;
                }
            } else if( isStringLiteral( *in ) ) { // string data
                in = parseStringLiteral( in, end, &primData );
                if( nullptr != primData ) {
                    ( *prop ) = new Property( id );
                    ( *prop )->m_primData = primData;
                }
            } else {                          // reference data
                Reference *ref( nullptr );
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

const char *OpenDDLParser::getVersion() {
    return Version;
}

char *OpenDDLParser::parseDataList( char *in, char *end ) {
    if( nullptr == in ) {
        return in;
    }

    return false;
}

char *OpenDDLParser::parseDataArrayList( char *in, char *end ) {
    return false;
}

DDLNode *OpenDDLParser::getRoot() const {
    return m_root;
}

END_ODDLPARSER_NS
