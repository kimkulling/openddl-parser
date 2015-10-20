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

#include <openddlparser/OpenDDLExport.h>
#include <openddlparser/DDLNode.h>
#include <openddlparser/Value.h>

BEGIN_ODDLPARSER_NS

class OpenDDLExportMock : public OpenDDLExport {
public:
    OpenDDLExportMock() 
    : OpenDDLExport() {
        // empty
    }
    
    virtual ~OpenDDLExportMock() {
        // empty
    }
    
    virtual bool writePropertiesTester( DDLNode *node, std::string &statement ) {
        return writeProperties( node, statement );
    }

    virtual bool writeValueTester( Value *val, std::string &statement ) {
        return writeValue( val, statement );
    }
};

class OpenDDLExportTest : public testing::Test {
public:
    DDLNode  *m_root;

protected:
    virtual void SetUp() {
        m_root = createNodeHierarchy();
    }

    virtual void TearDown() {
        delete m_root;
        m_root = ddl_nullptr;
    }

    DDLNode *createNodeHierarchy() {
        DDLNode *root = DDLNode::create( "test", "root" );
        for( uint32 i = 0; i < 10; i++ ) {
            DDLNode *node( DDLNode::create( "test", "child" ) );
            node->attachParent( root );
        }
        
        return root;
    }

    Property *createProperties( size_t numProps ) {
        if (0 == numProps) {
            return ddl_nullptr;
        }
    
        Property *prop( ddl_nullptr ), *first( ddl_nullptr ), *prev( ddl_nullptr );
        for ( size_t i = 0; i < numProps; i++) {
            static const size_t Size = 256;
            char buffer[ Size ];
            ::memset( buffer, 0, Size );
            sprintf( buffer, "id.%zu", i );
            Identifier *id = new Identifier( buffer, strlen( buffer ) );
            Value *v = ValueAllocator::allocPrimData( Value::ddl_int32 );
            v->setInt32( i );
            prop = new Property( id );
            prop->m_value = v;
            if (ddl_nullptr == first) {
                first = prop;
            }

            if (ddl_nullptr != prev) {
                prev->m_next = prop;
            }
            prev = prop;
        }

        return first;
    }
};

TEST_F( OpenDDLExportTest, createTest ) {
    bool ok( true );
    try {
        OpenDDLExport myExport;
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( OpenDDLExportTest, handleNodeTest ) {
    OpenDDLExport myExport;
     
    bool success( true );
    success = myExport.handleNode( m_root );
    EXPECT_TRUE( success );
}

TEST_F( OpenDDLExportTest, writeBoolTest ) {
    OpenDDLExportMock myExport;
    Value *v = ValueAllocator::allocPrimData( Value::ddl_bool );
    v->setBool( true );

    std::string statement;
    bool ok( true );
    ok = myExport.writeValueTester( v, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "true", statement );

    v->setBool( false );
    statement.clear();
    ok = myExport.writeValueTester( v, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "false", statement );

    ValueAllocator::releasePrimData( &v );
}

TEST_F( OpenDDLExportTest, writeIntegerTest ) {
    OpenDDLExportMock myExport;
    Value *v = ValueAllocator::allocPrimData( Value::ddl_int8 );
    v->setInt8( 10 );
    bool ok( true );
    std::string statement;
    ok = myExport.writeValueTester( v, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "10", statement );
    ValueAllocator::releasePrimData( &v );

    statement.clear();
    v = ValueAllocator::allocPrimData( Value::ddl_int16 );
    v->setInt16( 655 );
    ok = myExport.writeValueTester( v, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "655", statement );
    ValueAllocator::releasePrimData( &v );

    statement.clear();
    v = ValueAllocator::allocPrimData( Value::ddl_int32 );
    v->setInt32( 65501 );
    ok = myExport.writeValueTester( v, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "65501", statement );
    ValueAllocator::releasePrimData( &v );

    statement.clear();
    v = ValueAllocator::allocPrimData( Value::ddl_int64 );
    v->setInt64( 65502 );
    ok = myExport.writeValueTester( v, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "65502", statement );
    ValueAllocator::releasePrimData( &v );
}

TEST_F( OpenDDLExportTest, writeFloatTest ) {
}

TEST_F( OpenDDLExportTest, writeStringTest ) {
}

TEST_F( OpenDDLExportTest, writePropertiesTest ) {
    OpenDDLExportMock myExporter;
    bool ok( true );
    std::string statement;
    ok = myExporter.writePropertiesTester( ddl_nullptr, statement );
    EXPECT_FALSE( ok );

    Property *prop( createProperties( 1 ) );
    m_root->setProperties( prop );

    ok = myExporter.writePropertiesTester( m_root, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "(id.0 = 0)", statement );

    Property *prop2( createProperties( 2 ) );
    m_root->setProperties( prop2 );
    ok = myExporter.writePropertiesTester( m_root, statement );
    EXPECT_TRUE( ok );
    EXPECT_EQ( "(id.0 = 0, id.1 = 1)", statement );
}

END_ODDLPARSER_NS
