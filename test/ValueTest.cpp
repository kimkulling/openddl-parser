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

#include <openddlparser/Value.h>

BEGIN_ODDLPARSER_NS

class ValueTest : public testing::Test {
};

TEST_F( ValueTest, ValueDataAllocTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_NE( nullptr, data );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( ValueTest, ValueAccessBoolTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_NE( nullptr, data );
    data->setBool( true );
    EXPECT_EQ( true, data->getBool() );
    data->setBool( false );
    EXPECT_EQ( false, data->getBool() );
    ValueAllocator::releasePrimData( &data );
    EXPECT_EQ( nullptr, data );
}

TEST_F( ValueTest, ValueAccessNextTest ) {
    Value *data = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_EQ( nullptr, data->getNext() );

    Value *dataNext = ValueAllocator::allocPrimData( Value::ddl_bool );
    EXPECT_EQ( nullptr, dataNext->getNext() );

    data->setNext( dataNext );
    EXPECT_EQ( dataNext, data->getNext() );

}

END_ODDLPARSER_NS
