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
    EXPECT_FALSE( ddl_nullptr == data );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ( ddl_nullptr, data );
}

TEST_F( ValueTest, ValueAccessBoolTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    ASSERT_FALSE( ddl_nullptr == data );
    data->setBool( true );
    EXPECT_EQ( true, data->getBool() );
    data->setBool( false );
    EXPECT_EQ( false, data->getBool() );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ( ddl_nullptr, data );
}

TEST_F( ValueTest, ValueAccessStringTest ) {
    std::string text = "hallo";
    Value *data = ValueAllocator::allocPrimData( Value::ddl_string, text.size() );
    ASSERT_FALSE( ddl_nullptr == data );

    data->setString( text );
    int res = ::strncmp( text.c_str(), data->getString(), text.size() );
    EXPECT_EQ( 0, res );
}

TEST_F( ValueTest, ValueAccessNextTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    ASSERT_FALSE( ddl_nullptr == data );
    EXPECT_EQ( ddl_nullptr, data->getNext() );

    Value *dataNext = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_EQ( ddl_nullptr, dataNext->getNext() );

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
        EXPECT_FALSE( ddl_nullptr == v );
    }
}

TEST_F( ValueTest, IteratePreIncTest ) {
    Value *val( createValueList() );
    Value::Iterator it( val );
    while( it.hasNext() ) {
        ++it;
    }
}

TEST_F( ValueTest, IteratePreIncWithNullptrTest ) {
    Value *val( ddl_nullptr );
    Value::Iterator it( val );
    Value::Iterator tmp = ++it;
    EXPECT_FALSE( tmp.hasNext() );
}

TEST_F( ValueTest, IteratePostIncWithNullptrTest ) {
    Value *val( ddl_nullptr );
    Value::Iterator it( val );
    Value::Iterator tmp = it++;
    EXPECT_FALSE( tmp.hasNext() );
}

TEST_F( ValueTest, IteratePostIncTest ) {
    Value *val( createValueList() );
    Value::Iterator it( val );
    while( it.hasNext() ) {
        it++;
    }
}

TEST_F( ValueTest, accessInt8Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_int8 );
    data->setInt8( 10 );
    EXPECT_EQ( 10, data->getInt8() ); 
    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessInt16Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_int16 );
    data->setInt16( 10 );
    EXPECT_EQ( 10, data->getInt16() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessInt32Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_int32 );
    data->setInt32( 10 );
    EXPECT_EQ( 10, data->getInt32() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessInt64Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_int64 );
    data->setInt64( 10 );
    EXPECT_EQ( 10, data->getInt64() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt8Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_unsigned_int8 );
    data->setUnsignedInt8( 10 );
    EXPECT_EQ( 10, data->getUnsignedInt8() );
    
    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt16Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_unsigned_int16 );
    data->setUnsignedInt16( 10 );
    EXPECT_EQ( 10, data->getUnsignedInt16() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt32Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_unsigned_int32 );
    data->setUnsignedInt32( 10 );
    EXPECT_EQ( 10, data->getUnsignedInt32() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt64Test ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_unsigned_int64 );
    data->setUnsignedInt64( 10 );
    EXPECT_EQ( 10, data->getUnsignedInt64() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F(ValueTest, sizeTest) {
    Value *data1 = ValueAllocator::allocPrimData( Value::ddl_unsigned_int64 );
    size_t size( data1->size() );
    EXPECT_EQ( 1, size );

    Value *data2 = ValueAllocator::allocPrimData(Value::ddl_unsigned_int64);
    data1->setNext( data2 );
    size = data1->size();
    EXPECT_EQ(2, size);

    ValueAllocator::releasePrimData(&data2);
    ValueAllocator::releasePrimData(&data1);
}

END_ODDLPARSER_NS
