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

#include <openddlparser/DDLNode.h>
#include <openddlparser/OpenDDLParser.h>

#include "UnitTestCommon.h"

BEGIN_ODDLPARSER_NS

class DDLNodeTest : public testing::Test {
};

TEST_F( DDLNodeTest, createDDLNodeTest ) {
    DDLNode *myNode( nullptr );
    bool success( true );
    try {
        myNode = DDLNode::create( "test", "" );
    } catch( ... ) {
        myNode = nullptr;
        success = false;
    }
    EXPECT_NE( nullptr, myNode );
    EXPECT_TRUE( success );
}

TEST_F( DDLNodeTest, accessTypeTest ) {
    static const std::string type1 = "type";
    static const std::string name1 = "test";
    DDLNode *myNode = DDLNode::create( type1, name1 );
    EXPECT_EQ( type1, myNode->getType() );
    EXPECT_EQ( name1, myNode->getName() );

    static const std::string type2 = "type2";
    myNode->setType( type2 );
    EXPECT_EQ( type2, myNode->getType() );
}

TEST_F( DDLNodeTest, accessNameTest ) {
    static const std::string type1 = "type";
    static const std::string name1 = "test";
    DDLNode *myNode = DDLNode::create( type1, name1 );
    EXPECT_EQ( type1, myNode->getType() );
    EXPECT_EQ( name1, myNode->getName() );

    static const std::string name2 = "test";
    myNode->setName( name2 );
    EXPECT_EQ( name2, myNode->getName() );
}

TEST_F( DDLNodeTest, accessParentTest ) {
    static const std::string parent = "test";
    static const std::string parentName = "testparent_name";
    DDLNode *parentNode = DDLNode::create( parent, "parentName" );
    static const std::string name1 = "test1";
    static const std::string childType = "child";
    DDLNode *myNode = DDLNode::create( childType, name1, parentNode );
    EXPECT_EQ( parentNode, myNode->getParent() );
    EXPECT_EQ( 1, parentNode->getChildNodeList().size() );

    DDLNode *myNodeWithoutParent = DDLNode::create( childType, name1 );
    EXPECT_EQ( nullptr, myNodeWithoutParent->getParent() );

    myNodeWithoutParent->attachParent( parentNode );
    EXPECT_EQ( parentNode, myNodeWithoutParent->getParent() );
    DDLNode::DllNodeList myChilds = parentNode->getChildNodeList();
    EXPECT_EQ( 2, myChilds.size() );

    // check if the child node is not the parent node ( bug )
    EXPECT_EQ( name1, myChilds[ 0 ]->getName() );
    EXPECT_EQ( name1, myChilds[ 1 ]->getName() );
}

TEST_F( DDLNodeTest, accessPropertiesDDLNodeTest ) {
    DDLNode *node = DDLNode::create( "test", "testName" );
    
    EXPECT_EQ( nullptr, node->getProperties() );
    Identifier *id = new Identifier( "test", 4 );
    Property *first = new Property( id );
    node->setProperties( first );
    EXPECT_EQ( first, node->getProperties() );
}

TEST_F( DDLNodeTest, hasPropertyTest ) {
    DDLNode *node = DDLNode::create( "test", "testName" );
    bool found( false );
    found = node->hasProperty( "test" );
    EXPECT_FALSE( found );

    Identifier *id = new Identifier( "test", 4 );
    Property *first = new Property( id );
    node->setProperties( first );
    found = node->hasProperty( "test" );
    EXPECT_TRUE( found );
}

TEST_F( DDLNodeTest, hasPropertiesTest ) {
    DDLNode *node = DDLNode::create( "test", "testName" );
    EXPECT_FALSE(node->hasProperties() );

    Identifier *id = new Identifier( "test", 4 );
    Property *first = new Property( id );
    node->setProperties( first );
    EXPECT_TRUE( node->hasProperties() );

}

TEST_F( DDLNodeTest, findPropertyByNameTest ) {
    DDLNode *node = DDLNode::create( "test", "testName" );
    Property *prop( ddl_nullptr );
    prop = node->findPropertyByName( "test" );
    EXPECT_EQ( ddl_nullptr, prop );

    Identifier *id = new Identifier( "test", 4 );
    Property *first = new Property( id );
    node->setProperties( first );
    prop = node->findPropertyByName( "test" );
    EXPECT_EQ( first, prop );
}

TEST_F( DDLNodeTest, accessValueTest ) {
    DDLNode *myNode = DDLNode::create( "test", "name" );
    EXPECT_EQ( nullptr, myNode->getValue() );

    Value *myValue( new Value( Value::ddl_bool ) );
    myNode->setValue( myValue );
    EXPECT_EQ( myValue, myNode->getValue() );
}

TEST_F( DDLNodeTest, accessDataArrayListTest ) {
    DDLNode *myNode = DDLNode::create( "test", "name" );
    EXPECT_EQ( nullptr, myNode->getDataArrayList() );

    DataArrayList *dtArrayList( new DataArrayList );
    myNode->setDataArrayList( dtArrayList );
    EXPECT_EQ( dtArrayList, myNode->getDataArrayList() );
}

TEST_F( DDLNodeTest, accessReferencesTest ) {
    DDLNode *myNode = DDLNode::create( "test", "name" );
    EXPECT_EQ( nullptr, myNode->getReferences() );

    Reference *ref = new Reference;
    myNode->setReferences( ref );
    EXPECT_EQ( ref, myNode->getReferences() );
}

END_ODDLPARSER_NS
