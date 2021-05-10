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

#include <openddlparser/Value.h>

BEGIN_ODDLPARSER_NS

class ValueTest : public testing::Test {
protected:
    Value *m_start;

    void SetUp() override {
        m_start = nullptr;
    }

    void TearDown() override {
        delete m_start;
    }

    Value *createValueList() {
        Value *current(ValueAllocator::allocPrimData(Value::ValueType::ddl_bool));
        m_start = current;
        current->m_next = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
        current = current->m_next;
        current->m_next = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
        current = current->m_next;
        current->m_next = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
        current = current->m_next;

        return m_start; 
    }
};

TEST_F( ValueTest, ValueDataAllocTest ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
    EXPECT_FALSE(nullptr == data);
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ(nullptr, data);
}

TEST_F( ValueTest, ValueAccessBoolTest ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
    ASSERT_FALSE(nullptr == data);
    data->setBool( true );
    EXPECT_TRUE( data->getBool() );
    data->setBool( false );
    EXPECT_FALSE( data->getBool() );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ(nullptr, data);
}

TEST_F( ValueTest, ValueAccessStringTest ) {
    std::string text = "hallo";
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_string, text.size());
    ASSERT_FALSE(nullptr == data);

    data->setString( text );
    int res = ::strncmp( text.c_str(), data->getString(), text.size() );
    EXPECT_EQ( 0, res );
    delete data;
}

TEST_F( ValueTest, ValueAccessNextTest ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
    ASSERT_FALSE(nullptr == data);
    EXPECT_EQ(nullptr, data->getNext());

    Value *dataNext = ValueAllocator::allocPrimData(Value::ValueType::ddl_bool);
    EXPECT_EQ(nullptr, dataNext->getNext());

    data->setNext( dataNext );
    EXPECT_EQ( dataNext, data->getNext() );
    delete data;
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

    Value *v = nullptr;
    while( it.hasNext() ) {
        v = it.getNext();
        EXPECT_FALSE(nullptr == v);
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
    Value *val= nullptr;
    Value::Iterator it( val );
    Value::Iterator tmp = ++it;
    EXPECT_FALSE( tmp.hasNext() );
}

TEST_F( ValueTest, IteratePostIncWithNullptrTest ) {
    Value *val = nullptr;
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
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_int8);
    data->setInt8( 10 );
    EXPECT_EQ( 10, data->getInt8() ); 
    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessInt16Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_int16);
    data->setInt16( 10 );
    EXPECT_EQ( 10, data->getInt16() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessInt32Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_int32);
    data->setInt32( 10 );
    EXPECT_EQ( 10, data->getInt32() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessInt64Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_int64);
    data->setInt64( 10 );
    EXPECT_EQ( 10, data->getInt64() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt8Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_unsigned_int8);
    data->setUnsignedInt8( 10 );
    EXPECT_EQ( 10, data->getUnsignedInt8() );
    
    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt16Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_unsigned_int16);
    data->setUnsignedInt16( 10 );
    EXPECT_EQ( 10, data->getUnsignedInt16() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt32Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_unsigned_int32);
    const uint32 value = 10U;
    data->setUnsignedInt32( value );
    EXPECT_EQ( value, data->getUnsignedInt32() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessUInt64Test ) {
    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_unsigned_int64);
    data->setUnsignedInt64( 10U );
    EXPECT_EQ( 10U, data->getUnsignedInt64() );

    ValueAllocator::releasePrimData( &data );
}

TEST_F( ValueTest, accessReferenceTest ) {
    Reference *ref = new Reference;
    Name *name = new Name( GlobalName, new Text( "hello", 4 ) );
    ref->m_numRefs = 1;
    ref->m_referencedName = new Name*[1];
    ref->m_referencedName[0]=name;

    Value *data = ValueAllocator::allocPrimData(Value::ValueType::ddl_ref);
    data->setRef( ref );
    Reference *newRef( data->getRef() );
    EXPECT_EQ( ref->m_numRefs, newRef->m_numRefs );
    for ( size_t i = 0; i < ref->m_numRefs; i++ ) {
        Name *orig( ref->m_referencedName[ i ] ), *newName( newRef->m_referencedName[ i ] );
        EXPECT_EQ( orig->m_type, newName->m_type );
        EXPECT_EQ( *orig->m_id, *newName->m_id );
    }
    delete ref;
    delete data;

}

TEST_F(ValueTest, sizeTest) {
    Value *data1 = ValueAllocator::allocPrimData(Value::ValueType::ddl_ref);
    size_t size( data1->size() );
    EXPECT_EQ( 1U, size );

    Value *data2 = ValueAllocator::allocPrimData(Value::ValueType::ddl_ref);
    data1->setNext( data2 );
    size = data1->size();
    EXPECT_EQ(2U, size);

    delete data1;
}

END_ODDLPARSER_NS
