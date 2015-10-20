/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014-2015 Kim Kulling

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

#include <openddlparser/OpenDDLCommon.h>

BEGIN_ODDLPARSER_NS

static const char TestText[] = "hello";

class OpenDDLCommonTest : public testing::Test {
    Text *m_text;

public:
    virtual void SetUp() {
        m_text = ddl_nullptr;
    }

    virtual void TearDown() {
        delete m_text;
        m_text = ddl_nullptr;
    }

    const char *getTestText() const {
        return &TestText[ 0 ];
    }

    size_t getTestTextLen() {
        return strlen( TestText );
    }

    Text *createTestText() {
        m_text = new Text( getTestText(), getTestTextLen() );
        return m_text;
    }
};

TEST_F( OpenDDLCommonTest, typeSizesTest ) {
    EXPECT_EQ( 1, sizeof( int8 ) );
    EXPECT_EQ( 2, sizeof( int16 ) );
    EXPECT_EQ( 4, sizeof( int32 ) );
    EXPECT_EQ( 8, sizeof( int64 ) );

    EXPECT_EQ( 1, sizeof( uint8 ) );
    EXPECT_EQ( 2, sizeof( uint16 ) );
    EXPECT_EQ( 4, sizeof( uint32 ) );
    EXPECT_EQ( 8, sizeof( uint64 ) );
}

TEST_F( OpenDDLCommonTest, createNameTest ) {
    bool ok( true );
    Name *name( ddl_nullptr );
    try {
        name = new Name( GlobalName, ddl_nullptr );
    } catch( ... ) {
        ok = false;
        name = ddl_nullptr;
    }
    EXPECT_TRUE( ok );
    delete name;
}

TEST_F( OpenDDLCommonTest, createTextTest ) {
    bool ok( true );
    try {
        static_cast<void>( createTestText() );
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( OpenDDLCommonTest, accessTextTest ) {
    Text *theText = createTestText();
    ASSERT_TRUE( ddl_nullptr != theText );

    EXPECT_EQ( getTestTextLen(), theText->m_len );
    int res( strncmp( getTestText(), theText->m_buffer, getTestTextLen() ) );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLCommonTest, clearTextTest ) {
    Text *theText = createTestText();
    ASSERT_TRUE( ddl_nullptr != theText );

    theText->clear();
    EXPECT_EQ( 0, theText->m_len );
    EXPECT_TRUE( ddl_nullptr == theText->m_buffer );
}

TEST_F( OpenDDLCommonTest, setTextTest ) {
    Text *theText = createTestText();
    ASSERT_FALSE( ddl_nullptr == theText );

    static const std::string test2 = "Hello, World!";
    theText->set( test2.c_str(), test2.size() );
    
    EXPECT_EQ( test2.size(), theText->m_len );
    int res( strncmp( test2.c_str(), theText->m_buffer, test2.size() ) );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLCommonTest, CompareIdentifierTest ) {
    Identifier id1( "test" ), id2( "test" );
    bool equal( id1 == id2 );
    EXPECT_TRUE( equal );
}

END_ODDLPARSER_NS
