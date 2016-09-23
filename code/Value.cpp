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
#include <openddlparser/Value.h>

#include <iostream>
#include <cassert>

BEGIN_ODDLPARSER_NS

static Value::Iterator end( ddl_nullptr );

Value::Iterator::Iterator()
: m_start( ddl_nullptr )
, m_current( ddl_nullptr ) {
    // empty
}

Value::Iterator::Iterator( Value *start )
: m_start( start )
, m_current( start ) {
    // empty
}

Value::Iterator::Iterator( const Iterator &rhs )
: m_start( rhs.m_start )
, m_current( rhs.m_current ) {
    // empty
}

Value::Iterator::~Iterator() {
    // empty
}

bool Value::Iterator::hasNext() const {
    if( ddl_nullptr == m_current ) {
        return false;
    }
    return ( ddl_nullptr != m_current->getNext() );
}

Value *Value::Iterator::getNext() {
    if( !hasNext() ) {
        return ddl_nullptr;
    }

    Value *v( m_current->getNext() );
    m_current = v;

    return v;
}

const Value::Iterator Value::Iterator::operator++( int ) {
    if( ddl_nullptr == m_current ) {
        return end;
    }

    m_current = m_current->getNext();
    Iterator inst( m_current );

    return inst;
}

Value::Iterator &Value::Iterator::operator++( ) {
    if( ddl_nullptr == m_current ) {
        return end;
    }

    m_current = m_current->getNext();

    return *this;
}

bool Value::Iterator::operator == ( const Iterator &rhs ) const {
    return ( m_current == rhs.m_current );
}

Value *Value::Iterator::operator->( ) const {
    if(ddl_nullptr == m_current ) {
        return ddl_nullptr;
    }
    return m_current;
}


Value::~Value() {
    freeAllocatedMemory();

    if(m_next!=ddl_nullptr)
        delete m_next;
}


void Value::dump() {
    switch( m_type ) {
        case ddl_none:
            std::cout << "None" << std::endl;
            break;
        case ddl_bool:
            std::cout << getBool() << std::endl;
            break;
        case ddl_int8:
            std::cout << getInt8() << std::endl;
            break;
        case ddl_int16:
            std::cout << getInt16() << std::endl;
            break;
        case ddl_int32:
            std::cout << getInt32() << std::endl;
            break;
        case ddl_int64:
            std::cout << getInt64() << std::endl;
            break;
        case ddl_unsigned_int8:
            std::cout << getUnsignedInt8() << std::endl;
            break;
        case ddl_unsigned_int16:
            std::cout << getUnsignedInt16() << std::endl;
            break;
        case ddl_unsigned_int32:
            std::cout << getUnsignedInt32() << std::endl;
            break;
        case ddl_unsigned_int64:
            std::cout << getUnsignedInt64() << std::endl;
            break;
        case ddl_half:
            std::cout << "Not supported" << std::endl;
            break;
        case ddl_float:
            std::cout << getFloat() << std::endl;
            break;
        case ddl_double:
            std::cout << getDouble() << std::endl;
            break;
        case ddl_string:
            std::cout << getString() << std::endl;
            break;
        case ddl_ref:
            std::cout << "Not supported" << std::endl;
            break;
        default:
            break;
    }
}

size_t Value::size(){
    size_t result=1;
    Value *n=m_next;
    while( n!=ddl_nullptr) {
        result++;
        n=n->m_next;
    }
    return result;
}

END_ODDLPARSER_NS
