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

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


TEST_F( ValueTest, ValueAccessStringTest ) {
    std::string text = "hallo";
    Value data(text);
    int res = ::strncmp( text.c_str(), data.getString(), text.size() );
    EXPECT_EQ( 0, res );

}

TEST_F( ValueTest, ValueAccessNextTest ) {

    Value data;
    EXPECT_EQ( ddl_nullptr, data.getNext() );

    Value *dataNext = new Value;

    data.setNext(dataNext);
    EXPECT_EQ( dataNext, data.getNext() );
}

    /*
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
*/

TEST_F( ValueTest, accessInt8Test ) {
    Value data((int8)0x12);
    EXPECT_EQ( 0x12, data.getInt8() );
}

TEST_F( ValueTest, accessInt16Test ) {
    Value data((int16)0x1234);
    EXPECT_EQ(0x1234, data.getInt16() );
}

TEST_F( ValueTest, accessInt32Test ) {
    Value data((int32)0x12345678);
    EXPECT_EQ(0x12345678 , data.getInt32() );
}

TEST_F( ValueTest, accessInt64Test ) {
    Value data( 0x0123456789ABCDEFL );
    EXPECT_EQ( 0x0123456789ABCDEFL, data.getInt64() );
}

TEST_F( ValueTest, accessUInt8Test ) {
    Value data((uint8)0x12);
    EXPECT_EQ( 0x12, data.getInt8() );
}

TEST_F( ValueTest, accessUInt16Test ) {
    Value data((uint16)0x1234);
    EXPECT_EQ(0x1234, data.getInt16() );
}

TEST_F( ValueTest, accessUInt32Test ) {
    Value data((int32)0x12345678);
    EXPECT_EQ(0x12345678 , data.getInt32() );
}

TEST_F( ValueTest, accessUInt64Test ) {
    Value data((uint64) 0x0123456789ABCDEFL );
    EXPECT_EQ( 0x0123456789ABCDEFL, data.getInt64() );
}


TEST_F( ValueTest, accessReferenceTest ) {
    Reference *ref = new Reference;
    Name *name = new Name( GlobalName, new Text( "hello", 4 ) );
    ref->m_numRefs = 1;
    ref->m_referencedName = new Name*[1];
    ref->m_referencedName[0]=name;

    Value data(ref);
    Reference *newRef( data.getRef() );
    EXPECT_EQ( ref->m_numRefs, newRef->m_numRefs );
    for ( size_t i = 0; i < ref->m_numRefs; i++ ) {
        Name *orig( ref->m_referencedName[ i ] ), *newName( newRef->m_referencedName[ i ] );
        EXPECT_EQ( orig->m_type, newName->m_type );
        EXPECT_EQ( *orig->m_id, *newName->m_id );
    }
    delete ref;

}

static Value *createValueList(const int length) {
    Value *head = new Value;
    Value *parent=head,*child= ddl_nullptr;
    for(int i=0;i<length-1;i++){
        child= new Value;
        parent->setNext(child);
        parent=child;
    }
    return head;
}

TEST_F(ValueTest, sizeTest) {
    Value *data1 = createValueList(0);
    EXPECT_EQ( 1,data1->size() );

    Value *data2 = createValueList(10);
    EXPECT_EQ(10,data2->size());

    delete data1;
    delete data2;
}

END_ODDLPARSER_NS
