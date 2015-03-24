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

#include <openddlparser/OpenDDLParser.h>

#include "UnitTestCommon.h"

BEGIN_ODDLPARSER_NS

class OpenDDLIntegrationTest : public testing::Test {
};

TEST_F( OpenDDLIntegrationTest, parseMetricTest ) {
    char token[] = "Metric( key = \"distance\" ) { float{ 1 } }\n"
                   "Metric( key = \"up\" ) { float{ 1 } }\n";
    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ) );
    result = theParser.parse();
    EXPECT_TRUE( result );

    DDLNode *myNode = theParser.getRoot();
    ASSERT_NE( nullptr, myNode );

    Context *ctx = theParser.getContext();
    ASSERT_NE( nullptr, ctx );

    DDLNode::DllNodeList myList = myNode->getChildNodeList();
    ASSERT_EQ( 2, myList.size() );
    DDLNode *child = myList[ 0 ];
    ASSERT_NE( nullptr, child );
    EXPECT_EQ( "Metric", child->getType() );
    Property *prop = child->getProperties();
    ASSERT_NE( nullptr, prop );

    const char *data1 = ( const char *) prop->m_primData->m_data;
    int res( ::strncmp( "distance", data1, strlen( "distance" ) ) );
    EXPECT_EQ( Value::ddl_string, prop->m_primData->m_type );
    EXPECT_EQ( 0, res );

    child = myList[ 1 ];
    ASSERT_NE( nullptr, child );
    EXPECT_EQ( "Metric", child->getType() );
    prop = child->getProperties();
    const char *data2 = ( const char * ) prop->m_primData->m_data;
    res = ::strncmp( "up", data2, strlen( "up" ) );
    EXPECT_EQ( Value::ddl_string, prop->m_primData->m_type );
    EXPECT_EQ( 0, res );
}

TEST_F( OpenDDLIntegrationTest, parseEmbeddedStructureTest ) {
    char token[] =
        "GeometryNode $node1\n"
        "{\n"
        "    string\n"
        "    {\n"
        "        \"test\"\n"
        "    }\n"
        "}";

    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ) );
    result = theParser.parse();
    EXPECT_TRUE( result );

    DDLNode *root( theParser.getRoot() );
    ASSERT_NE( nullptr, root );
}

TEST_F( OpenDDLIntegrationTest, parseEmbeddedStructureWithRefTest ) {
    char token[] =
        "GeometryNode $node1\n"
        "{\n"
        "    Name{ string{ \"Box001\" } }\n"
        "    ObjectRef{ ref{ $geometry1 } }\n"
        "    MaterialRef{ ref{ $material1 } }\n"
        "}";

    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ) );
    result = theParser.parse();
    EXPECT_TRUE( result );

    DDLNode *root( theParser.getRoot() );
    ASSERT_NE( nullptr, root );

    DDLNode::DllNodeList childs = root->getChildNodeList();
    ASSERT_EQ( 1, childs.size() );

    DDLNode::DllNodeList childChilds = childs[ 0 ]->getChildNodeList();
    ASSERT_EQ( 3, childChilds.size() );
    EXPECT_EQ( "Name", childChilds[ 0 ]->getType() );
    EXPECT_EQ( "ObjectRef", childChilds[ 1 ]->getType() );
    EXPECT_EQ( "MaterialRef", childChilds[ 2 ]->getType() );
}

TEST_F( OpenDDLIntegrationTest, parseTransformDataTest ) {
    char token[] =
        "    Transform\n"
        "    {\n"
        "        float[ 16 ]\n"
        "        {\n"
        "            {0x3F800000, 0x00000000, 0x00000000, 0x00000000,		// {1, 0, 0, 0\n"
        "             0x00000000, 0x3F800000, 0x00000000, 0x00000000,		//  0, 1, 0, 0\n"
        "             0x00000000, 0x00000000, 0x3F800000, 0x00000000,		//  0, 0, 1, 0\n"
        "             0xBEF33B00, 0x411804DE, 0x00000000, 0x3F800000}		//  -0.47506, 9.50119, 0, 1}\n"
        "        }\n"
        "    }\n";
    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ) );
    result = theParser.parse();
    EXPECT_TRUE( result );
}

END_ODDLPARSER_NS
