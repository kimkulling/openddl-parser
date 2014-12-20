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
#ifndef OPENDDLPARSER_OPENDDLPARSER_H_INC
#define OPENDDLPARSER_OPENDDLPARSER_H_INC

#include <vector>

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
#   define DLL_CPPCORE_EXPORT
#endif
#define BEGIN_ODDLPARSER_NS namespace ODDLParser {
#define END_ODDLPARSER_NS   }

#define ODDL_NO_COPYING( classname ) \

BEGIN_ODDLPARSER_NS
    
struct Identifier;
struct Reference;
struct PrimData;

typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;
typedef long  int64_t;

enum PrimitiveDataType {
    ddl_none = -1,
    ddl_bool = 0,
    ddl_int8,
    ddl_int16,
    ddl_int32,
    ddl_int64,
    ddl_unsigned_int8,
    ddl_unsigned_int16,
    ddl_unsigned_int32,
    ddl_unsigned_int64,
    ddl_half,
    ddl_float,
    ddl_double,
    ddl_string,
    ddl_ref,
    ddl_types_max
};

template<class T>
inline
bool isComment( T *in, T *end ) {
    if( *in == '/' ) {
        if( in + 1 != end ) {
            if( *( in + 1 ) == '/' ) {
                return true;
            }
        }
    }
    return false;
}

template<class T>
inline
bool isUpperCase( T in ) {
    return ( in >= 'A' && in <= 'Z' );
}

template<class T>
inline
bool isLowerCase( T in ) {
    return ( in >= 'a' && in <= 'z' );
}

template<class T>
inline
bool isSeparator( T in ) {
    if ( ' ' == in || '\n' == in || '\t' == in || ',' == in ) {
        return true;
    }
    return false;
}

static const unsigned char chartype_table[ 256 ] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-15
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16-31
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 32-47
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, // 48-63

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64-79
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-95
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 96-111
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 112-127

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // > 127 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

template<class T>
inline
bool isNumeric( const T in ) {
    return ( in >= '0' && in <= '9' );
    //return ( chartype_table[in] );
    /*if (in >= '0' &&  in <= '9' )
    return true;

    return false;*/
}

template<class T>
inline
static T *getNextSeparator( T *in, T *end ) {
    while( !isSeparator( *in ) || in == end ) {
        in++;
    }
    return in;
}

struct DLL_ODDLPARSER_EXPORT PrimDataAllocator {
    static PrimData *allocPrimData( PrimitiveDataType type, size_t len = 1 );
    static void releasePrimData( PrimData **data );
};

struct DLL_ODDLPARSER_EXPORT PrimData {
    PrimitiveDataType m_type;
    size_t m_size;
    unsigned char *m_data;

    PrimData()
        : m_type( ddl_none )
        , m_size( 0 )
        , m_data( nullptr ) {
        // empty
    }

    void setBool( bool value );
    bool getBool();
    void setInt8( int8_t value );
    int8_t getInt8();
    void setInt16( int16_t value );
    int16_t  getInt16();
    void setInt32( int32_t value );
    int32_t  getInt32();
    void setInt64( int64_t value );
    int64_t  getInt64();
    void setFloat( float value );
    float getFloat() const;
};

enum NameType {
    GlobalName,
    LocalName
};

struct Name {
    NameType m_type;
    Identifier *m_id;

    Name( NameType type, Identifier *id )
    : m_type( type ), m_id( id ) {
        // empty
    }
};

struct Reference {
    size_t  m_numRefs;
    Name *m_referencedName;

    Reference( size_t numrefs, Name *names )
    : m_numRefs( numrefs )
    , m_referencedName( names ) {
        // empty
    }
};

struct Identifier {
    size_t m_len;
    char *m_buffer;

    Identifier( size_t len, char *buffer )
    : m_len( len )
    , m_buffer( buffer ) {
        // empty
    }
};

class DLL_ODDLPARSER_EXPORT DDLNode {
public:
    typedef std::vector<DDLNode*> DllNodeList;

public:
    DDLNode( const std::string &name, DDLNode *parent = nullptr );
    ~DDLNode();
    void attachParent( DDLNode *parent );
    void detachParent();
    DDLNode *getParent() const;
    const DllNodeList &getChildNodeList() const;
    void setName( const std::string &name );
    const std::string &getName() const;

private:
    DDLNode();
    DDLNode( const DDLNode & );
    DDLNode &operator = ( const DDLNode & );

private:
    std::string m_name;
    DDLNode *m_parent;
    std::vector<DDLNode*> m_children;
};

class DLL_ODDLPARSER_EXPORT OpenDDLParser {
public:
    OpenDDLParser();
    OpenDDLParser( const std::vector<char> &buffer );
    ~OpenDDLParser();
    void setBuffer( const std::vector<char> &buffer );
    void clear();
    bool parse();
    char *parseDataType( char *in, char *end );
    char *parseIdentifier( char *in, char *end );
    static bool isDDLDataType( char *in, char *end );
    static void normalizeBuffer( std::vector<char> &buffer );
    static char *parseName( char *in, char *end, Name **name );
    static char *parseIdentifier( char *in, char *end, Identifier **id );
    static char *parsePrimitiveDataType( char *in, char *end, PrimData **primData );
    static char *parseReference( char *in, char *end, std::vector<Name*> &names );
    static char *parseBooleanLiteral( char *in, char *end, PrimData **boolean );
    static char *parseIntegerLiteral( char *in, char *end, PrimData **integer, PrimitiveDataType integerType = ddl_int32 );
    static char *parseFloatingLiteral( char *in, char *end, PrimData **floating );
    static char *parseStringLiteral( char *in, char *end, PrimData **stringData );
    char *parseDataList( char *in, char *end );
    char *parseDataArrayList( char *in, char *end );
    char *parseProperty( char *in, char *end );
    DDLNode *getRoot() const;

private:
    OpenDDLParser( const OpenDDLParser & );
    OpenDDLParser &operator = ( const OpenDDLParser & );

private:
    std::vector<char> m_buffer;
    DDLNode *m_root;
};

END_ODDLPARSER_NS

#endif // OPENDDLPARSER_OPENDDLPARSER_H_INC
