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

BEGIN_ODDLPARSER_NS
    
enum PrimitiveDataType {
    ddl_none = -1,
    ddl_bool = 0,
    ddl_int8,
    ddl_int16,
    ddl_int32,
    ddl_int64,
    ddl_unsigned_int8,
    unsigned_int16,
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
    bool isUpperCase( const T in ) {
    return ( in >= 'A' && in <= 'Z' );
}

template<class T>
inline
    bool isLowerCase( const T in ) {
    return ( in >= 'a' && in <= 'z' );
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

struct PrimData {
    PrimitiveDataType m_type;
    size_t m_size;
    unsigned char *m_data;

    PrimData()
        : m_type( ddl_none )
        , m_size( 0 )
        , m_data( nullptr ) {
        // empty
    }
};

struct DDLNode {
    DDLNode *m_parent;
    std::vector<DDLNode*> m_children;
    DDLNode() 
    : m_parent( nullptr )
    , m_children() {
        // empty
    }
};

class DLL_ODDLPARSER_EXPORT OpenDDLParser {
public:
    OpenDDLParser();
    OpenDDLParser( const std::vector<char> &buffer );
    ~OpenDDLParser();
    bool parse();
    static PrimData *parsePrimitiveDataType( const char *in, size_t len, size_t &offset );
    bool parseDataList( const std::vector<char> &buffer, size_t index );
    bool parseName();
    bool parseDataArrayList( const std::vector<char> &buffer, size_t index );
    bool parseIdentifier();
    bool parseReference();
    bool parseProperty();
    bool parseBoolean();
    bool parseInteger();
    bool parseFloatingNo();
    bool parseString();
    DDLNode *getRoot() const;

private:
    OpenDDLParser( const OpenDDLParser & );
    OpenDDLParser &operator = ( const OpenDDLParser & );

private:
    const std::vector<char> *m_buffer;
    DDLNode *m_root;
};

END_ODDLPARSER_NS

#endif // OPENDDLPARSER_OPENDDLPARSER_H_INC
