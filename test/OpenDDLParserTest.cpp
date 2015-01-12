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
#include "gtest/gtest.h"

#include <openddlparser/OpenDDLParser.h>

#include <iostream>

BEGIN_ODDLPARSER_NS

static char *findEnd( char *in, size_t &len ) {
    len = strlen( in );
    char *end( &in[ len ] );

    return end;
}

static size_t countItems( PrimData *data ) {
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
class OpenDDLParserTest : public testing::Test {
    static OpenDDLParserTest *s_instance;
    std::vector<DDLNode*> m_nodes;
    std::vector<PrimData*> m_data;
    std::vector<std::string> m_logs;

public:
    DDLNode *createNode( const std::string &type, const std::string &name, DDLNode *parent ) {
        DDLNode *node = new DDLNode( type, name, parent );
        m_nodes.push_back( node );

        return node;
    }

    static void setInstance( OpenDDLParserTest *instance ) {
        s_instance = instance;
    }

    static OpenDDLParserTest *getInstance() {
        return s_instance;
    }

    static void testLogCallback( LogSeverity severity, const std::string &msg ) {
        std::stringstream stream;
        stream << static_cast<int>( severity ) << msg;
        OpenDDLParserTest::getInstance()->addLog( stream.str() );
    }

    void addLog( const std::string &msg ) {
        m_logs.push_back( msg );
    }
    
    void clearTestLog() {
        m_logs.clear();
    }

    void registerPrimDataForDeletion( PrimData *data ) {
        if( data ) {
            m_data.push_back( data );
        }
    }
protected:
    virtual void SetUp() {
        OpenDDLParserTest::setInstance( this );
    }

    virtual void TearDown() {
        for( size_t i = 0; i < m_data.size(); i++ ) {
            delete m_data[ i ];
        }
        m_data.clear();

        for( size_t i=0; i < m_nodes.size(); i++ ) {
            delete m_nodes[ i ];
        }
        m_nodes.resize( 0 );
        clearTestLog();
        OpenDDLParserTest::setInstance( nullptr );
    }
};

OpenDDLParserTest *OpenDDLParserTest::s_instance = nullptr;

TEST_F( OpenDDLParserTest, isCommentTest ) {
    size_t len( 0 );
    bool result( false );
    char *end( nullptr );
    char token1[] = "//";
    end = findEnd( token1, len );
    result = isComment( token1, end );
    EXPECT_TRUE( result );

    char token2[] = "/*";
    end = findEnd( token2, len );
    result = isComment( token2, end );
    EXPECT_FALSE( result );

    char token3[] = "/";
    end = findEnd( token3, len );
    result = isComment( token3, end );
    EXPECT_FALSE( result );

}
TEST_F( OpenDDLParserTest, isUpperCaseTest) {
    char val = 'a';
    EXPECT_TRUE(isLowerCase<char>(val));
    val = 'c';
    EXPECT_TRUE(isLowerCase<char>(val));
    val = 'z';
    EXPECT_TRUE(isLowerCase<char>(val));
    val = 'A';
    EXPECT_FALSE(isLowerCase<char>(val));
    val = '0';
    EXPECT_FALSE(isLowerCase<char>(val));
}

TEST_F( OpenDDLParserTest, isLowerCaseTest ) {
    char val = 'A';
    EXPECT_TRUE(isUpperCase<char>(val));
    val = 'C';
    EXPECT_TRUE(isUpperCase<char>(val));
    val = 'Z';
    EXPECT_TRUE(isUpperCase<char>(val));
    val = 'a';
    EXPECT_FALSE(isUpperCase<char>(val));
    val = '0';
    EXPECT_FALSE(isUpperCase<char>(val));
}

TEST_F( OpenDDLParserTest, isNumericTest) {
    char val1 = '0';
    EXPECT_TRUE( isNumeric<char>(val1) );
    char val2 = '9';
    EXPECT_TRUE(isNumeric<char>(val2));
    char val3 = '4';
    EXPECT_TRUE(isNumeric<char>(val3));
    char notChar = 'a';
    EXPECT_FALSE(isNumeric<char>(notChar));
}

TEST_F( OpenDDLParserTest, isIntegerTest ) {    
    size_t len( 0 );
    
    // positive value
    char val1[] = "12";
    char *end = findEnd( val1, len );
    bool result( false );
    result = isInteger( val1, end );
    EXPECT_TRUE( result );

    // positive value, separated by a comma
    char val2[] = "12,";
    end = findEnd( val2, len );
    result = isInteger( val2, end );
    EXPECT_TRUE( result );

    // negative value
    char val3[] = "-12";
    end = findEnd( val3, len );
    result = isInteger( val3, end );
    EXPECT_TRUE( result );

    // float value
    char val4[] = "12.";
    end = findEnd( val4, len );
    result = isInteger( val4, end );
    EXPECT_FALSE( result );

    // string literal
    char val5[] = "test";
    end = findEnd( val5, len );
    result = isInteger( val5, end );
    EXPECT_FALSE( result );

    // positive value, separated by a curly bracket
    char val6[] = "12}";
    end = findEnd( val6, len );
    result = isInteger( val6, end );
    EXPECT_TRUE( result );

}

TEST_F( OpenDDLParserTest, isFloatTest ) {
    size_t len( 0 );
    char val1[] = "12.0";
    char *end = findEnd( val1, len );
    bool result( false );
    result = isFloat( val1, end );
    EXPECT_TRUE( result );

    char val2[] = "-12.0";
    end = findEnd( val2, len );
    result = isFloat( val2, end );
    EXPECT_TRUE( result );

    char val3[] = "-12";
    end = findEnd( val3, len );
    result = isFloat( val3, end );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserTest, isCharacterTest ) {
    char val = 'a';
    EXPECT_TRUE( isCharacter<char>( val ) );
    val = 'z';
    EXPECT_TRUE( isCharacter<char>( val ) );
    val = 'A';
    EXPECT_TRUE( isCharacter<char>( val ) );
    val = 'Z';
    EXPECT_TRUE( isCharacter<char>( val ) );
    val = '1';
    EXPECT_FALSE( isCharacter<char>( val ) );
}

TEST_F( OpenDDLParserTest, isStringLiteralTest ) {
    char val = '\"';
    EXPECT_TRUE( isStringLiteral<char>( val ) );
    val = 'a';
    EXPECT_FALSE( isStringLiteral<char>( val ) );
}

TEST_F( OpenDDLParserTest, isHexLiteralTest ) {
    size_t len( 0 );
    char token1[] = "0x10";
    char *end = findEnd( token1, len );
    bool result = isHexLiteral( token1, end );
    EXPECT_TRUE( result );

    char token11[] = "0X10";
    end = findEnd( token11, len );
    result = isHexLiteral( token11, end );
    EXPECT_TRUE( result );


    char token2[] = "10";
    result = isHexLiteral( token2, end );
    EXPECT_FALSE( result );

    char token3[] = "lala";
    result = isHexLiteral( token3, end );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserTest, isSpaceTest ) {
    bool result( false );
    char val;
    val = ' ';
    result = isSpace( val );
    EXPECT_TRUE( result );

    val = '\t';
    result = isSpace( val );
    EXPECT_TRUE( result );
    
    val = '1';
    result = isSpace( val );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserTest, isNewLineTest ) {
    bool result( false );
    char val;
    val = '\n';
    result = isNewLine( val );
    EXPECT_TRUE( result );

    val = ' ';
    result = isNewLine( val );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserTest, isSeparatorTest ) {
    char val;
    val = ' ';
    EXPECT_TRUE( isSeparator( val ) );
    val = '\t';
    EXPECT_TRUE( isSeparator( val ) );
    val = '1';
    EXPECT_FALSE( isSeparator( val ) );
    val = 'a';
    EXPECT_FALSE( isSeparator( val ) );
    val = 'Z';
    EXPECT_FALSE( isSeparator( val ) );
    val = '{';
    EXPECT_TRUE( isSeparator( val ) );
    val = '}';
    EXPECT_TRUE( isSeparator( val ) );
    val = '[';
    EXPECT_TRUE( isSeparator( val ) );
}

TEST_F( OpenDDLParserTest, getNextSeparatorTest ) {
    char tokenlist[] = "abc, \na";
    char *in( &tokenlist[ 0 ] );
    char *end( &tokenlist[ strlen( "abc, \na" ) ] );

    bool res( true );
    res = isSeparator( *in );
    EXPECT_FALSE( res );
    in = getNextSeparator( in, end );
    res = isSeparator( *in );
    EXPECT_TRUE( res );
    in = getNextSeparator( in, end );
    res = isSeparator( *in );
    EXPECT_TRUE( res );
}

TEST_F( OpenDDLParserTest, hex2DecimalTest ) {
    int res = hex2Decimal( '1' );
    EXPECT_EQ( 1, res );

    res = hex2Decimal( '0' );
    EXPECT_EQ( 0, res );

    res = hex2Decimal( '9' );
    EXPECT_EQ( 9, res );

    res = hex2Decimal( 'a' );
    EXPECT_EQ( 10, res );

    res = hex2Decimal( 'f' );
    EXPECT_EQ( 15, res );

    res = hex2Decimal( 'A' );
    EXPECT_EQ( 10, res );

    res = hex2Decimal( 'F' );
    EXPECT_EQ( 15, res );
}

TEST_F( OpenDDLParserTest, PrimDataAllocTest ) {
    PrimData *data = PrimDataAllocator::allocPrimData( ddl_bool );
    EXPECT_NE( nullptr, data );
    PrimDataAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, PrimDataAccessBoolTest ) {
    PrimData *data = PrimDataAllocator::allocPrimData( ddl_bool );
    EXPECT_NE( nullptr, data );
    data->setBool( true );
    EXPECT_EQ( true, data->getBool() );
    data->setBool( false );
    EXPECT_EQ( false, data->getBool() );
    PrimDataAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, PrimDataAccessNextTest ) {
    PrimData *data = PrimDataAllocator::allocPrimData( ddl_bool );
    EXPECT_EQ( nullptr, data->getNext() );

    PrimData *dataNext = PrimDataAllocator::allocPrimData( ddl_bool );
    EXPECT_EQ( nullptr, dataNext->getNext() );

    data->setNext( dataNext );
    EXPECT_EQ( dataNext, data->getNext() );

}

TEST_F( OpenDDLParserTest, createTest ) {
    bool success( true );
    try {
        OpenDDLParser myParser;
        EXPECT_EQ( nullptr, myParser.getRoot() );
    } catch( ... ) {
        success = false;
    }
    EXPECT_TRUE( success );
}

TEST_F( OpenDDLParserTest, setLogCallbackTest ) {    
    OpenDDLParser myParser;

    myParser.setLogCallback( OpenDDLParserTest::testLogCallback );
    EXPECT_EQ( &OpenDDLParserTest::testLogCallback, myParser.getLogCallback() );

    myParser.setLogCallback( nullptr );
    EXPECT_NE( &OpenDDLParserTest::testLogCallback, myParser.getLogCallback() );
}

TEST_F( OpenDDLParserTest, accessBufferTest ) {
    bool success( true );
    static const size_t len = 100;
    char *buffer = new char[ len ];
    OpenDDLParser myParser;
    myParser.setBuffer( buffer, len );
    EXPECT_EQ( len, myParser.getBufferSize() );
    EXPECT_EQ( buffer, myParser.getBuffer() );

    try {
        myParser.clear();
        buffer[ 1 ] = 'c';
    } catch( ... ) {
        success = false;
    }
    EXPECT_TRUE( success );

    myParser.setBuffer( buffer, len, true );
    myParser.clear();
    EXPECT_EQ( 0, myParser.getBufferSize() );
    EXPECT_EQ( nullptr, myParser.getBuffer() );
}

TEST_F( OpenDDLParserTest, isDDLDataTypeTest ) {

}

TEST_F( OpenDDLParserTest, normalizeBufferTest ) {
    char buffer[] = {
        "line 1 // comment\n"
        "line 2\n"
        "//comment\n"
        "line 4"
    };

    OpenDDLParser::normalizeBuffer( buffer, strlen( buffer ) );
}

TEST_F( OpenDDLParserTest, parseIdentifierTest ) {
    int res( 0 );
    size_t len1( 0 );
    char name1[] = "testname", *end1( findEnd( name1, len1 ) );

    Identifier *id( nullptr );
    char *in = OpenDDLParser::parseIdentifier( name1, end1, &id );
    ASSERT_TRUE( id != nullptr );
    res = strncmp( id->m_buffer, name1, len1 );
    EXPECT_EQ( 0, res );
    
    size_t len2( 0 );
    char name2[] = " testname ", *end2( findEnd( name2, len2 ) );
    in = OpenDDLParser::parseIdentifier( name2, end2, &id );
    EXPECT_TRUE( id != nullptr );
    const size_t tokenLen( strlen( id->m_buffer ) );
    res = strncmp( id->m_buffer, name1, tokenLen );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLParserTest, parseNameTest ) {
    size_t len1( 0 );
    char name1[] = "$testname", *end1( findEnd( name1, len1 ) );

    Name *name(  nullptr );
    char *in = OpenDDLParser::parseName( name1, end1, &name );
    EXPECT_TRUE( name != nullptr );
    EXPECT_NE( name1, in );
}

TEST_F( OpenDDLParserTest, parsePrimitiveDataTypeTest ) {
    PrimitiveDataType type;
    size_t len( 0 );
    char *in = OpenDDLParser::parsePrimitiveDataType( nullptr, nullptr, type, len );
    EXPECT_EQ( ddl_none, type );
    EXPECT_EQ( 0, len );

    size_t len1( 0 );
    char token1[] = "float", *end1( findEnd( token1, len1 ) );
    in = OpenDDLParser::parsePrimitiveDataType( token1, end1, type, len );
    EXPECT_EQ( ddl_float, type );
    EXPECT_EQ( 0, len );

    size_t len2( 0 );
    char invalidToken[] = "foat", *end2( findEnd( token1, len2 ) );
    in = OpenDDLParser::parsePrimitiveDataType( invalidToken, end2, type, len );
    EXPECT_EQ( ddl_none, type );
    EXPECT_EQ( 0, len );
}
 
TEST_F( OpenDDLParserTest, parsePrimitiveDataTypeWithArrayTest ) {
    size_t len1( 0 );
    char token[] = "float[3]", *end( findEnd( token, len1 ) );

    PrimitiveDataType type;
    size_t arrayLen( 0 );

    char *in = OpenDDLParser::parsePrimitiveDataType( token, end, type, arrayLen );
    EXPECT_EQ( ddl_float, type );
    EXPECT_EQ( 3, arrayLen );
    EXPECT_NE( in, token );
}

TEST_F( OpenDDLParserTest , parsePrimitiveArrayWithSpacesTest ) {
    PrimitiveDataType type;
    size_t arrayLen( 0 ), len( 0 );
    char token[] = "float[ 16 ]", *end( findEnd( token, len ) );
    char *in = OpenDDLParser::parsePrimitiveDataType( token, end, type, arrayLen );
    EXPECT_NE( in, token );
    EXPECT_EQ( ddl_float, type );
    EXPECT_EQ( 16, arrayLen );
}

TEST_F( OpenDDLParserTest, parsePrimitiveArrayHexTest ) {
    size_t len( 0 );
    char token[] = 
        "{0x01,0x02,0x03}\n";
    char *end( findEnd( token, len ) );
    PrimData *data( nullptr );
    char *in = OpenDDLParser::parseDataList( token, end, &data );
    EXPECT_NE( in, token );
}

TEST_F( OpenDDLParserTest, parsePrimitiveDataTypeWithInvalidArrayTest ) {
    size_t len1( 0 );
    char token1[] = "float[3", *end( findEnd( token1, len1 ) );
    PrimitiveDataType type;
    size_t arrayLen( 0 );
    char *in = OpenDDLParser::parsePrimitiveDataType( token1, end, type, arrayLen );
    EXPECT_EQ( ddl_none, type );
    EXPECT_EQ( 0, arrayLen );
    EXPECT_NE( in, token1 );
}

TEST_F( OpenDDLParserTest, parseReferenceTest ) {
    size_t len1( 0 );
    char token1[] = "ref { $name1, %name2 }", *end( findEnd( token1, len1 ) );

    std::vector<Name*> names;
    char *in = OpenDDLParser::parseReference( token1, end, names );
    ASSERT_EQ( 2, names.size() );
    EXPECT_NE( in, token1 );

    int res( 0 );
    Name *name( nullptr );
    name = names[ 0 ];
    EXPECT_NE( nullptr, name );
    EXPECT_EQ( GlobalName, name->m_type );
    res = strncmp( name->m_id->m_buffer, "$name1", strlen( "$name1" ) );
    EXPECT_EQ( 0, res );

    name = names[ 1 ];
    EXPECT_NE( nullptr, name );
    EXPECT_EQ( LocalName, name->m_type );
    res = strncmp( name->m_id->m_buffer, "%name2", strlen( "%name2" ) );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLParserTest, parseBooleanLiteralTest ) {
    char *in( nullptr );
    PrimData *data( nullptr );
    size_t len1( 0 );
    char token1[] = "true", *end1( findEnd( token1, len1 ) );
    in = OpenDDLParser::parseBooleanLiteral( token1, end1, &data );
    ASSERT_NE( nullptr, data );
    EXPECT_EQ( ddl_bool, data->m_type );
    EXPECT_EQ( true, data->getBool() );
    registerPrimDataForDeletion( data );

    size_t len2( 0 );
    char token2[] = "false", *end2( findEnd( token2, len2 ) );
    in = OpenDDLParser::parseBooleanLiteral( token2, end2, &data );
    ASSERT_NE( nullptr, data );
    EXPECT_EQ( ddl_bool, data->m_type );
    EXPECT_EQ( false, data->getBool() );
    registerPrimDataForDeletion( data );

    size_t len3( 0 );
    char token3[] = "fallse", *end3( findEnd( token3, len3 ) );
    in = OpenDDLParser::parseBooleanLiteral( token3, end3, &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, parseIntegerLiteralTest ) {
    char *in( nullptr );
    size_t len1( 0 );
    PrimData *data( nullptr );

    char token1[] = "1", *end1( findEnd( token1, len1 ) );
    in = OpenDDLParser::parseIntegerLiteral( token1, end1, &data );
    ASSERT_NE( nullptr, data );
    EXPECT_EQ( ddl_int32, data->m_type );
    EXPECT_EQ( 1, data->getInt32() );
    registerPrimDataForDeletion( data );

    size_t len2( 0 );
    char token2[] = "aaa", *end2( findEnd( token2, len2 ) );
    in = OpenDDLParser::parseIntegerLiteral( token2, end2, &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, parseInvalidIntegerLiteralTest ) {
    size_t len1( 0 );
    PrimData *data( nullptr );
    char token1[] = "1", *end1( findEnd( token1, len1 ) );
    char *in( token1 );
    char *out = OpenDDLParser::parseIntegerLiteral( token1, end1, &data, ddl_float );
    EXPECT_EQ( out, in );
    ASSERT_EQ( nullptr, data );
}

TEST_F( OpenDDLParserTest, parseFloatingLiteralTest ) {
    size_t len1( 0 );
    PrimData *data( nullptr );
    char token1[] = "1.0f", *end1( findEnd( token1, len1 ) );
    char *out = OpenDDLParser::parseFloatingLiteral( token1, end1, &data );
    EXPECT_NE( out, token1 );
    EXPECT_NE( nullptr, data );
    EXPECT_EQ( ddl_float, data->m_type );
    EXPECT_EQ(1.0f, data->getFloat() );
}

TEST_F( OpenDDLParserTest, parseStringLiteralTest ) {
    size_t len1( 0 );
    PrimData *data( nullptr );
    char token1[] = "\"teststring\"", *end1( findEnd( token1, len1 ) );
    char *in( token1 );

    char *out = OpenDDLParser::parseStringLiteral( token1, end1, &data );
    EXPECT_NE( in, out );
    EXPECT_NE( nullptr, data );
    EXPECT_EQ( ddl_string, data->m_type );
    std::string str( (char*) data->m_data );
    int res( ::strncmp( "teststring", str.c_str(), str.size() ) );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLParserTest, parseHexaLiteralTest ) {
    size_t len( 0 );
    char token1[] = "0x01", *end( findEnd( token1, len ) );
    PrimData *data( nullptr );
    char *in = OpenDDLParser::parseHexaLiteral( token1, end, &data );
    ASSERT_NE( nullptr, data );
    registerPrimDataForDeletion( data );

    int v( data->getInt32() );
    EXPECT_EQ( 1, data->getInt32() );

    char token2[] = "0xff";
    end = findEnd( token2, len );
    in = OpenDDLParser::parseHexaLiteral( token2, end, &data );
    ASSERT_NE( nullptr, data );
    v = data->getInt32();
    EXPECT_EQ( 255, data->getInt32() );
    registerPrimDataForDeletion( data );

    char token3[] = "0xff";
    end = findEnd( token3, len );
    in = OpenDDLParser::parseHexaLiteral( token3, end, &data );
    ASSERT_NE( nullptr, data );
    v = data->getInt32();
    EXPECT_EQ( 255, data->getInt32() );
    registerPrimDataForDeletion( data );
}

TEST_F( OpenDDLParserTest, parsePropertyTest ) {
    char *in( nullptr );
    size_t len( 0 );
    char *prop1 = "lod = 2", *end1( findEnd( prop1, len ) );
    Property *prop( nullptr );
    in = OpenDDLParser::parseProperty( prop1, end1, &prop );
    ASSERT_NE( nullptr, prop );
    ASSERT_NE( nullptr, prop->m_id );
    int res = strncmp( "lod", prop->m_id->m_buffer, prop->m_id->m_len );
    EXPECT_EQ( 0, res ); 
        
    char *prop2 = "key = \"angle\"", *end2( findEnd( prop2, len ) );
    in = OpenDDLParser::parseProperty( prop2, end2, &prop );
    ASSERT_NE( nullptr, prop );
    ASSERT_NE( nullptr, prop->m_id );
    res = strncmp( "key", prop->m_id->m_buffer, prop->m_id->m_len );
    EXPECT_EQ( 0, res );

    EXPECT_EQ( ddl_string, prop->m_primData->m_type );
    EXPECT_NE( nullptr, prop->m_primData->m_data );
    res = strncmp( "angle", (char*) prop->m_primData->m_data, prop->m_primData->m_size );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLParserTest, parseDataArrayListTest ) {
    char token[] =
        "float[ 3 ]\n"
        "{\n"
        "    { 1.0, 2.0, 3.0 },\n"
        "    { 0.5, 0.0, 0.5 },\n"
        "    { 0.0, -1.0, 4.0 }\n"
        "}\n";
    size_t len( 0 );
    char *end( findEnd( token, len ) );
    PrimData *data( nullptr );
    char *in = OpenDDLParser::parseDataArrayList( token, end, &data );
    ASSERT_NE( nullptr, data );
    registerPrimDataForDeletion( data );
    EXPECT_NE( token, in );
}

TEST_F( OpenDDLParserTest, getVersionTest ) {
    const char *version( OpenDDLParser::getVersion() );
    ASSERT_NE( nullptr, version );
}

TEST_F( OpenDDLParserTest, parseDataListTest ) {
    PrimData *data( nullptr );
    size_t len1( 0 );
    
    char *in( nullptr ), *end( nullptr );
    char token1[] = "{1,2,3,4}";
    end = findEnd( token1, len1 );
    in = OpenDDLParser::parseDataList( token1, end, &data );
    ASSERT_NE( nullptr, data );
    registerPrimDataForDeletion( data );

    // check intrinsic list with integers
    EXPECT_EQ( 4, countItems( data ) );

    size_t len2( 0 );
    char token2[] = "{ \"string1\",\"string2\"}";
    end = findEnd( token2, len2 );
    in = OpenDDLParser::parseDataList( token2, end, &data );
    ASSERT_NE( nullptr, data );
    registerPrimDataForDeletion( data );

    // check intrinsic list with strings
    EXPECT_EQ( 2, countItems( data ) );
}

TEST_F( OpenDDLParserTest, pushTest ) {
    OpenDDLParser theParser;

    DDLNode *current = theParser.top();
    EXPECT_EQ( nullptr, current );
    DDLNode *node = createNode( "test", "", nullptr );
    theParser.pushNode( node );
    current = theParser.top();
    EXPECT_EQ( node, current );
}

TEST_F( OpenDDLParserTest, popTest ) {
    OpenDDLParser theParser;

    DDLNode *current = theParser.top();
    EXPECT_EQ( nullptr, current );

    DDLNode *node1 = createNode( "test1", "", nullptr );
    theParser.pushNode( node1 );
    current = theParser.top();
    EXPECT_EQ( node1, current );

    DDLNode *node2 = createNode( "test2", "", nullptr );
    theParser.pushNode( node2 );
    current = theParser.top();
    EXPECT_EQ( node2, current );

    DDLNode *node3 = createNode( "test3", "", nullptr );
    theParser.pushNode( node3 );
    current = theParser.top();
    EXPECT_EQ( node3, current );

    current = theParser.popNode();
    EXPECT_EQ( node3, current );
    current = theParser.popNode();
    EXPECT_EQ( node2, current );
    current = theParser.popNode();
    EXPECT_EQ( node1, current );

    current = theParser.top();
    EXPECT_EQ( nullptr, current );
}

END_ODDLPARSER_NS
