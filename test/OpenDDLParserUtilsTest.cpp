/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014-2020 Kim Kulling

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

#include <openddlparser/OpenDDLParserUtils.h>

#include "UnitTestCommon.h"

BEGIN_ODDLPARSER_NS

class OpenDDLParserUtilsTest : public testing::Test {
    // empty
};

TEST_F( OpenDDLParserUtilsTest, isCommentTest ) {
    size_t len( 0 );
    bool result( false );
    char *end(nullptr);
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

    char token4[] = "//f/";
    end = findEnd( token4, len );
    result = isComment( token3, end );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserUtilsTest, isUpperCaseTest ) {
    char val = 'a';
    EXPECT_TRUE( isLowerCase<char>( val ) );
    val = 'c';
    EXPECT_TRUE( isLowerCase<char>( val ) );
    val = 'z';
    EXPECT_TRUE( isLowerCase<char>( val ) );
    val = 'A';
    EXPECT_FALSE( isLowerCase<char>( val ) );
    val = '0';
    EXPECT_FALSE( isLowerCase<char>( val ) );
}

TEST_F( OpenDDLParserUtilsTest, isLowerCaseTest ) {
    char val = 'A';
    EXPECT_TRUE( isUpperCase<char>( val ) );
    val = 'C';
    EXPECT_TRUE( isUpperCase<char>( val ) );
    val = 'Z';
    EXPECT_TRUE( isUpperCase<char>( val ) );
    val = 'a';
    EXPECT_FALSE( isUpperCase<char>( val ) );
    val = '0';
    EXPECT_FALSE( isUpperCase<char>( val ) );
}

TEST_F( OpenDDLParserUtilsTest, isNumericTest ) {
    char val1 = '0';
    EXPECT_TRUE( isNumeric<char>( val1 ) );
    char val2 = '9';
    EXPECT_TRUE( isNumeric<char>( val2 ) );
    char val3 = '4';
    EXPECT_TRUE( isNumeric<char>( val3 ) );
    char notChar = 'a';
    EXPECT_FALSE( isNumeric<char>( notChar ) );
}

TEST_F( OpenDDLParserUtilsTest, isIntegerTest ) {
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

TEST_F( OpenDDLParserUtilsTest, isFloatTest ) {
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

TEST_F( OpenDDLParserUtilsTest, isFloatWithSeparatorTest ) {
    bool result( false );
    size_t len( 0 );
    char val1[] = "-12.0}";
    char*end = findEnd( val1, len );
    result = isFloat( val1, end );
    EXPECT_TRUE( result );

    char val2[] = "-12.0,";
    end = findEnd( val2, len );
    result = isFloat( val2, end );
    EXPECT_TRUE( result );
}

TEST_F( OpenDDLParserUtilsTest, isCharacterTest ) {
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

TEST_F( OpenDDLParserUtilsTest, isStringLiteralTest ) {
    char val = '\"';
    EXPECT_TRUE( isStringLiteral<char>( val ) );
    val = 'a';
    EXPECT_FALSE( isStringLiteral<char>( val ) );
}

TEST_F( OpenDDLParserUtilsTest, isHexLiteralTest ) {
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

TEST_F( OpenDDLParserUtilsTest, isReferenceTest ) {
    size_t len( 0 );
    char token1[] = "ref";
    char *end = findEnd( token1, len );
    bool result = isReference( token1, end );
    EXPECT_TRUE( result );

    char token2[] = "rf";
    end = findEnd( token2, len );
    result = isReference( token2, end );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserUtilsTest, isSpaceTest ) {
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

TEST_F( OpenDDLParserUtilsTest, isNewLineTest ) {
    bool result( false );
    char val;
    val = '\n';
    result = isNewLine( val );
    EXPECT_TRUE( result );

    val = '\r';
    result = isNewLine( val );
    EXPECT_TRUE( result );

    val = ' ';
    result = isNewLine( val );
    EXPECT_FALSE( result );
}

TEST_F( OpenDDLParserUtilsTest, isSeparatorTest ) {
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

TEST_F( OpenDDLParserUtilsTest, isCommendOpenTag_Success ) {
    size_t len(0);
    bool result(false);
    char *end(nullptr);
    char token1[] = "/*";
    end = findEnd(token1, len);
    result = isCommentOpenTag( token1, end );
    EXPECT_TRUE(result);
}


TEST_F(OpenDDLParserUtilsTest, isCommendOpenTag_False) {
    size_t len(0);
    bool result(false);
    char *end(nullptr);
    char token1[] = "*/";
    end = findEnd( token1, len );
    result = isCommentOpenTag( token1, end );
    EXPECT_FALSE(result);

    char token2[] = "//";
    end = findEnd( token2, len );
    result = isCommentOpenTag( token2, end );
    EXPECT_FALSE(result);

    char token3[] = "/ *";
    end = findEnd( token3, len);
    result = isCommentOpenTag(token3, end);
    EXPECT_FALSE(result);

    char token4[] = "*/";
    end = findEnd( token4, len );
    result = isCommentOpenTag( token4, end );
    EXPECT_FALSE(result);
}

TEST_F( OpenDDLParserUtilsTest, isCommendCloseTag_Success ) {
    size_t len( 0 );
    bool result(false);
    char *end(nullptr);
    char token1[] = "*/";
    end = findEnd( token1, len );
    result = isCommentCloseTag( token1, end );
    EXPECT_TRUE(result);
}

TEST_F(OpenDDLParserUtilsTest, isCommendCloseTag_False) {
    size_t len(0);
    bool result(false);
    char *end(nullptr);
    char token1[] = "/*";
    end = findEnd( token1, len );
    result = isCommentCloseTag( token1, end );
    EXPECT_FALSE( result );

    char token2[] = "//";
    end = findEnd(token2, len);
    result = isCommentCloseTag(token2, end);
    EXPECT_FALSE(result);

    char token3[] = "/ *";
    end = findEnd(token3, len);
    result = isCommentCloseTag(token3, end);
    EXPECT_FALSE(result);
}

END_ODDLPARSER_NS
