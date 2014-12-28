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

#include <openddlparser/OpenDDLCommon.h>
#include <openddlparser/OpenDDLParserUtils.h>

#include <vector>
#include <string>

BEGIN_ODDLPARSER_NS

struct Identifier;
struct Reference;
struct PrimData;
struct Property;

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

struct DLL_ODDLPARSER_EXPORT PrimDataAllocator {
    static PrimData *allocPrimData( PrimitiveDataType type, size_t len = 1 );
    static void releasePrimData( PrimData **data );
};

struct DLL_ODDLPARSER_EXPORT PrimData {
    PrimitiveDataType m_type;
    size_t m_size;
    unsigned char *m_data;
    PrimData *m_next;

    PrimData()
    : m_type( ddl_none )
    , m_size( 0 )
    , m_data( nullptr )
    , m_next( nullptr ) {
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
    void setNext( PrimData *next );
    PrimData *getNext() const;
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
    size_t m_numRefs;
    Name **m_referencedName;

    Reference( size_t numrefs, Name **names )
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

struct Property {
    Identifier *m_id;
    PrimData *m_primData;
    Reference *m_ref;
    Property *m_next;

    Property( Identifier *id )
    : m_id( id )
    , m_primData( nullptr ) 
    , m_ref( nullptr )
    , m_next( nullptr ) {
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
    void setProperties( Property *first );
    Property *getProperties() const;

private:
    DDLNode();
    DDLNode( const DDLNode & );
    DDLNode &operator = ( const DDLNode & );

private:
    std::string m_name;
    DDLNode *m_parent;
    std::vector<DDLNode*> m_children;
    Property *m_properties;
};

class DLL_ODDLPARSER_EXPORT OpenDDLParser {
public:
    OpenDDLParser();
    OpenDDLParser( char *buffer, size_t len, bool ownsIt = false );
    ~OpenDDLParser();
    void setBuffer( char *buffer, size_t len, bool ownsIt = false );
    char *getBuffer() const;
    size_t getBufferSize() const;
    void clear();
    bool parse();
    char *parseNextNode( char *current, char *end );
    char *parseStructure( char *in, char *end );
    char *parseId( char *in, char *end );
    void push( DDLNode *node );
    DDLNode *pop();
    DDLNode *top();
    static void normalizeBuffer( char *buffer, size_t len );
    static char *parseName( char *in, char *end, Name **name );
    static char *parseIdentifier( char *in, char *end, Identifier **id );
    static char *parsePrimitiveDataType( char *in, char *end, PrimData **primData );
    static char *parseReference( char *in, char *end, std::vector<Name*> &names );
    static char *parseBooleanLiteral( char *in, char *end, PrimData **boolean );
    static char *parseIntegerLiteral( char *in, char *end, PrimData **integer, PrimitiveDataType integerType = ddl_int32 );
    static char *parseFloatingLiteral( char *in, char *end, PrimData **floating );
    static char *parseStringLiteral( char *in, char *end, PrimData **stringData );
    static char *parseProperty( char *in, char *end, Property **prop );
    static char *parseDataList( char *in, char *end, PrimData **data );
    static const char *getVersion();
    char *parseDataArrayList( char *in, char *end );
    DDLNode *getRoot() const;

private:
    OpenDDLParser( const OpenDDLParser & );
    OpenDDLParser &operator = ( const OpenDDLParser & );

private:
    bool m_ownsBuffer;
    char *m_buffer;
    size_t m_len;
    DDLNode *m_root;
    typedef std::vector<DDLNode*> DDLNodeStack;
    DDLNodeStack m_stack;
};

END_ODDLPARSER_NS

#endif // OPENDDLPARSER_OPENDDLPARSER_H_INC
