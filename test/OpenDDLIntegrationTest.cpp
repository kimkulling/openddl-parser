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

#include <openddlparser/OpenDDLParser.h>

#include "UnitTestCommon.h"
#include <vector>

BEGIN_ODDLPARSER_NS

class OpenDDLIntegrationTest : public testing::Test {
    // empty
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
    ASSERT_FALSE(nullptr == myNode);

    Context *ctx = theParser.getContext();
    ASSERT_FALSE(nullptr == ctx);

    DDLNode::DllNodeList myList = myNode->getChildNodeList();
    ASSERT_EQ( 2U, myList.size() );
    DDLNode *child = myList[ 0 ];
    ASSERT_FALSE(nullptr == child);
    EXPECT_EQ( "Metric", child->getType() );
    Property *prop = child->getProperties();
    ASSERT_FALSE(nullptr == prop);

    const char *data1 = ( const char * ) prop->m_value->m_data;
    int res( ::strncmp( "distance", data1, strlen( "distance" ) ) );
    EXPECT_EQ(Value::ValueType::ddl_string, prop->m_value->m_type);
    EXPECT_EQ( 0, res );

    child = myList[ 1 ];
    ASSERT_FALSE(nullptr == child);
    EXPECT_EQ( "Metric", child->getType() );
    prop = child->getProperties();
    const char *data2 = ( const char * ) prop->m_value->m_data;
    res = ::strncmp( "up", data2, strlen( "up" ) );
    EXPECT_EQ(Value::ValueType::ddl_string, prop->m_value->m_type);
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
    ASSERT_FALSE(nullptr == root);
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
    ASSERT_FALSE(nullptr == root);

    DDLNode::DllNodeList childs = root->getChildNodeList();
    ASSERT_EQ( 1U, childs.size() );

    DDLNode::DllNodeList childChilds = childs[ 0 ]->getChildNodeList();
    ASSERT_EQ( 3U, childChilds.size() );

    DDLNode *currentNode = nullptr;
    currentNode = childChilds[ 0 ];
    EXPECT_EQ( "Name", currentNode->getType() );
    currentNode = childChilds[ 1 ];
    EXPECT_EQ( "ObjectRef", currentNode->getType() );
    currentNode = childChilds[ 2 ];
    EXPECT_EQ( "MaterialRef", currentNode->getType() );
}

static void dataArrayList2StdVector( DataArrayList *dataArrayList, std::vector<float> &container ) {
    if (nullptr == dataArrayList) {
        return;
    }
    Value *val( dataArrayList->m_dataList );
    if (nullptr == val) {
        return;
    }
    while (nullptr != val) {
        container.push_back( val->getFloat() );
        val = val->m_next;
    }
}

TEST_F( OpenDDLIntegrationTest, parseTransformDataTest ) {
    char token[] =
        "Transform\n"
        "{\n"
        "    float[ 16 ]\n"
        "    {\n"
        "        {0x3F800000, 0x00000000, 0x00000000, 0x00000000,		// {1, 0, 0, 0\n"
        "         0x00000000, 0x3F800000, 0x00000000, 0x00000000,		//  0, 1, 0, 0\n"
        "         0x00000000, 0x00000000, 0x3F800000, 0x00000000,		//  0, 0, 1, 0\n"
        "         0xBEF33B00, 0x411804DE, 0x00000000, 0x3F800000}		//  -0.47506, 9.50119, 0, 1}\n"
        "    }\n"
        "}\n";
    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ) );
    result = theParser.parse();
    EXPECT_TRUE( result );

    DDLNode *root( theParser.getRoot() );
    EXPECT_TRUE(nullptr != root);

    const DDLNode::DllNodeList &childs( root->getChildNodeList() );
    EXPECT_EQ( 1U, childs.size() );
    DDLNode *transform( childs[ 0 ] );
    EXPECT_TRUE(nullptr != transform);
    DataArrayList *transformData( transform->getDataArrayList() );
    EXPECT_TRUE(nullptr != transformData);
    EXPECT_EQ( 16U, transformData->m_numItems );

    std::vector<float> container;
    dataArrayList2StdVector( transformData, container );
    EXPECT_FLOAT_EQ( 1.0f, container[ 0 ] );
    EXPECT_FLOAT_EQ( 0.0f, container[ 1 ] );
    EXPECT_FLOAT_EQ( 0.0f, container[ 2 ] );
    EXPECT_FLOAT_EQ( 0.0f, container[ 3 ] );
}

TEST_F( OpenDDLIntegrationTest, exportDataTest ) {
    char token [] =
        "Metric( key = \"distance\" ) { float{ 1 } }\n"
        "Metric( key = \"angle\" ) { float{ 1 } }\n"
        "Metric( key = \"time\" ) { float{ 1 } }\n"
        "Metric( key = \"up\" ) { string{ \"z\" } }\n"
        "\n"
        "GeometryNode $node1\n"
        "{\n"
        "   Name{ string{ \"Box001\" } }\n"
        "   ObjectRef{ ref{ $geometry1 } }\n"
        "   MaterialRef{ ref{ $material1 } }\n"
        "\n"
        "Transform\n"
        "{\n"
        "   float[ 16 ]\n"
        "   {\n"
        "        { 0x3F800000, 0x00000000, 0x00000000, 0x00000000,		// {1, 0, 0, 0\n"
        "          0x00000000, 0x3F800000, 0x00000000, 0x00000000,		//  0, 1, 0, 0\n"
        "          0x00000000, 0x00000000, 0x3F800000, 0x00000000,		//  0, 0, 1, 0\n"
        "          0xBEF33B00, 0x411804DE, 0x00000000, 0x3F800000 }		//  -0.47506, 9.50119, 0, 1}\n"
        "       }\n"
        "   }\n"
        "}\n"
        "\n"
        "GeometryNode $node2\n"
        "{\n"
        "   Name{ string{ \"Box002\" } }\n"
        "   ObjectRef{ ref{ $geometry1 } }\n"
        "   MaterialRef{ ref{ $material1 } }\n"
        "\n"
        "   Transform\n"
        "   {\n"
        "       float[ 16 ]\n"
        "       {\n"
        "           { 0x3F800000, 0x00000000, 0x00000000, 0x00000000,		// {1, 0, 0, 0\n"
        "             0x00000000, 0x3F800000, 0x00000000, 0x00000000,		//  0, 1, 0, 0\n"
        "             0x00000000, 0x00000000, 0x3F800000, 0x00000000,		//  0, 0, 1, 0\n"
        "             0x43041438, 0x411804DE, 0x00000000, 0x3F800000 }		//  132.079, 9.50119, 0, 1}\n"
        "       }\n"
        "   }\n"
        "}\n"
        "\n"
        "GeometryObject $geometry1		// Box001, Box002\n"
        "{ \n"
        "    Mesh( primitive = \"triangles\" )\n"
        "    {\n"
        "        VertexArray( attrib = \"position\" )\n"
        "        {\n"
        "           float[ 3 ]		// 24\n"
        "           { \n"
        "               { 0xC2501375, 0xC24C468A, 0x00000000 },{ 0xC2501375, 0x424C468A, 0x00000000 },{ 0x42501375, 0x424C468A, 0x00000000 },{ 0x42501375, 0xC24C468A, 0x00000000 },{ 0xC2501375, 0xC24C468A, 0x42BA3928 },{ 0x42501375, 0xC24C468A, 0x42BA3928 },{ 0x42501375, 0x424C468A, 0x42BA3928 },{ 0xC2501375, 0x424C468A, 0x42BA3928 }, \n"
        "               { 0xC2501375, 0xC24C468A, 0x00000000 },{ 0x42501375, 0xC24C468A, 0x00000000 },{ 0x42501375, 0xC24C468A, 0x42BA3928 },{ 0xC2501375, 0xC24C468A, 0x42BA3928 },{ 0x42501375, 0xC24C468A, 0x00000000 },{ 0x42501375, 0x424C468A, 0x00000000 },{ 0x42501375, 0x424C468A, 0x42BA3928 },{ 0x42501375, 0xC24C468A, 0x42BA3928 }, \n"
        "               { 0x42501375, 0x424C468A, 0x00000000 },{ 0xC2501375, 0x424C468A, 0x00000000 },{ 0xC2501375, 0x424C468A, 0x42BA3928 },{ 0x42501375, 0x424C468A, 0x42BA3928 },{ 0xC2501375, 0x424C468A, 0x00000000 },{ 0xC2501375, 0xC24C468A, 0x00000000 },{ 0xC2501375, 0xC24C468A, 0x42BA3928 },{ 0xC2501375, 0x424C468A, 0x42BA3928 }\n"
        "           }\n"
        "        }\n"
        "\n"
        "       VertexArray( attrib = \"normal\" )\n"
        "       {\n"
        "           float[ 3 ]		// 24\n"
        "           { \n"
        "               { 0x00000000, 0x00000000, 0xBF800000 },{ 0x00000000, 0x00000000, 0xBF800000 },{ 0x00000000, 0x00000000, 0xBF800000 },{ 0x00000000, 0x00000000, 0xBF800000 },{ 0x00000000, 0x00000000, 0x3F800000 },{ 0x00000000, 0x00000000, 0x3F800000 },{ 0x00000000, 0x00000000, 0x3F800000 },{ 0x00000000, 0x00000000, 0x3F800000 }, \n"
        "               { 0x00000000, 0xBF800000, 0x00000000 },{ 0x00000000, 0xBF800000, 0x00000000 },{ 0x00000000, 0xBF800000, 0x00000000 },{ 0x80000000, 0xBF800000, 0x00000000 },{ 0x3F800000, 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000, 0x00000000 }, \n"
        "               { 0x00000000, 0x3F800000, 0x00000000 },{ 0x00000000, 0x3F800000, 0x00000000 },{ 0x00000000, 0x3F800000, 0x00000000 },{ 0x80000000, 0x3F800000, 0x00000000 },{ 0xBF800000, 0x00000000, 0x00000000 },{ 0xBF800000, 0x00000000, 0x00000000 },{ 0xBF800000, 0x00000000, 0x00000000 },{ 0xBF800000, 0x00000000, 0x00000000 }\n"
        "           }\n"
        "       }\n"
        "\n"
        "       VertexArray( attrib = \"texcoord\" )\n"
        "       {\n"
        "           float[ 2 ]		// 24\n"
        "           { \n"
        "               { 0x3F800000, 0x00000000 },{ 0x3F800000, 0x3F800000 },{ 0x00000000, 0x3F800000 },{ 0x00000000, 0x00000000 },{ 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000 },{ 0x3F800000, 0x3F800000 },{ 0x00000000, 0x3F800000 }, \n"
        "               { 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000 },{ 0x3F800000, 0x3F800000 },{ 0x00000000, 0x3F800000 },{ 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000 },{ 0x3F800000, 0x3F800000 },{ 0x00000000, 0x3F800000 }, \n"
        "               { 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000 },{ 0x3F800000, 0x3F800000 },{ 0x00000000, 0x3F800000 },{ 0x00000000, 0x00000000 },{ 0x3F800000, 0x00000000 },{ 0x3F800000, 0x3F800000 },{ 0x00000000, 0x3F800000 }\n"
        "           }\n"
        "       }\n"
        "\n"
        "       IndexArray\n"
        "       {\n"
        "           unsigned_int32[ 3 ]		// 12\n"
        "           { \n"
        "               { 0, 1, 2 },{ 2, 3, 0 },{ 4, 5, 6 },{ 6, 7, 4 },{ 8, 9, 10 },{ 10, 11, 8 },{ 12, 13, 14 },{ 14, 15, 12 },{ 16, 17, 18 },{ 18, 19, 16 },{ 20, 21, 22 },{ 22, 23, 20 }\n"
        "           }\n"
        "       }\n"
        "   }\n"
        "}\n"
        "\n"
        "Material $material1\n"
        "{\n"
        "   Name{ string{ \"03 - Default\" } }\n"
        "\n"
        "   Color( attrib = \"diffuse\" ) { float[ 3 ]{ { 0.588235, 0.588235, 0.588235 } } }\n"
        "   Texture( attrib = \"diffuse\" )\n"
        "   {\n"
        "       string{ \"texture/Concrete.tga\" }\n"
        "   }\n"
        "}\n";

    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ) );
    result = theParser.parse();
    EXPECT_TRUE( result );

    result = theParser.exportContext( theParser.getContext(), "" );
    EXPECT_TRUE( result );
}

END_ODDLPARSER_NS
