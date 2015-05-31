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

#include <openddlparser/Value.h>

BEGIN_ODDLPARSER_NS

class ValueTest : public testing::Test {
protected:
    Value *m_start;

    virtual void SetUp() {
        m_start = ddl_nullptr;
    }

    virtual void TearDown() {
        Value *current( m_start ), *tmp( ddl_nullptr );
        while( ddl_nullptr != current ) {
            tmp = current;
            current = current->m_next;
            delete tmp;
        }
    }

    Value *createValueList() {
        Value *current( ValueAllocator::allocPrimData( Value::ddl_bool ) );
        m_start = current;
        current->m_next = ValueAllocator::allocPrimData( Value::ddl_bool );
        current = current->m_next;
        current->m_next = ValueAllocator::allocPrimData( Value::ddl_bool );
        current = current->m_next;
        current->m_next = ValueAllocator::allocPrimData( Value::ddl_bool );
        current = current->m_next;

        return m_start; 
    }
};

TEST_F( ValueTest, ValueDataAllocTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_NE( nullptr, data );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( ValueTest, ValueAccessBoolTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    ASSERT_NE( nullptr, data );
    data->setBool( true );
    EXPECT_EQ( true, data->getBool() );
    data->setBool( false );
    EXPECT_EQ( false, data->getBool() );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( ValueTest, ValueAccessStringTest ) {
    std::string text = "hallo";
    Value *data = ValueAllocator::allocPrimData( Value::ddl_string, text.size() );
    ASSERT_NE( nullptr, data );

    data->setString( text );
    int res = ::strncmp( text.c_str(), data->getString(), text.size() );
    EXPECT_EQ( 0, res );
}

TEST_F( ValueTest, ValueAccessNextTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_EQ( nullptr, data->getNext() );

    Value *dataNext = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_EQ( nullptr, dataNext->getNext() );

    data->setNext( dataNext );
    EXPECT_EQ( dataNext, data->getNext() );

}

TEST_F( ValueTest, InitIteratorTest ) {
    Value::Iterator it1;
    EXPECT_FALSE(it1.hasNext() );
    Value::Iterator it2( m_start );
    EXPECT_FALSE( it2.hasNext() );
}

TEST_F( ValueTest, IterateTest ) {
    Value *val( createValueList() );
    Value::Iterator it( val );
    EXPECT_TRUE( it.hasNext() );

    Value *v( ddl_nullptr );
    while( it.hasNext() ) {
        v = it.getNext();
        EXPECT_NE( ddl_nullptr, v );
    }
}

TEST_F( ValueTest, IteratePreIncTest ) {
    Value *val( createValueList() );
    Value::Iterator it( val );
    while( it.hasNext() ) {
        ++it;
    }
}

TEST_F( ValueTest, IteratePostIncTest ) {
    Value *val( createValueList() );
    Value::Iterator it( val );
    while( it.hasNext() ) {
        it++;
    }
}

END_ODDLPARSER_NS
