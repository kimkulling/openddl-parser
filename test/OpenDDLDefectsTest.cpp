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

#include <openddlparser/OpenDDLCommon.h>
#include <openddlparser/OpenDDLParser.h>
#include <openddlparser/Value.h>
#include "UnitTestCommon.h"

BEGIN_ODDLPARSER_NS

class OpenDDLDefectsTest : public testing::Test {
    // empty
};

TEST_F( OpenDDLDefectsTest, Issue20_WrongColorNodeParsing ) {
    char token[] = "{0.588235, 0.588235, 0.588235}\n";
    size_t len( 0 );
    char *end = findEnd( token, len );

    Value *data = nullptr;
    Reference *refs(nullptr);
    size_t numRefs( 0 ), numValues( 0 );
    Value::ValueType type(Value::ValueType::ddl_none);
    char *in = OpenDDLParser::parseDataList( token, end, type, &data, numValues, &refs, numRefs );
    ASSERT_FALSE(nullptr == in);
    ASSERT_FALSE(nullptr == data);
    ASSERT_EQ( 3U, numValues );
    delete data;
    delete refs;
}

TEST_F( OpenDDLDefectsTest, assimp_issues_665 ) {
    char token [] = {
        "GeometryNode $node2\n"
        "{\n"
        "    Name{ string{ \"Plane\" } }\n"
        "    ObjectRef{ ref{ $geometry1 } }\n"
        "    MaterialRef( index = 0 ) { ref{ $material1 } }\n"
        "    Transform\n"
        "    {\n"
        "        float[ 16 ]\n"
        "        {\n"
        "            { 1.0, 0.0, 0.0, 0.0,\n"
        "              0.0, 1.0, 0.0, 0.0,\n"
        "              0.0, 0.0, 1.0, 0.0,\n"
        "              0.0, 0.0, 0.0, 1.0 }\n"
        "        }\n"
        "    }\n"
        "}\n"
    };

    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );
}

TEST_F( OpenDDLDefectsTest, no_whitespace ) {
    char token [] = {
        "GeometryNode$node2"
        "{"
        "Name{string{\"Plane\"}}"
        "ObjectRef{ref{$geometry1}}"
        "MaterialRef(index=0){ref{$material1}}"
        "Transform"
        "{"
        "float[16]"
        "{"
        "{1.0,0.0,0.0,0.0,"
        "0.0,1.0,0.0,0.0,"
        "0.0,0.0,1.0,0.0,"
        "0.0,0.0,0.0,1.0}"
        "}"
        "}"
        "}"
    };

    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );
    DDLNode *root = myParser.getRoot();
    DDLNode::DllNodeList childs = root->getChildNodeList();
    EXPECT_STREQ( "node2", childs[0]->getName().c_str());
    EXPECT_STREQ( "GeometryNode", childs[0]->getType().c_str());
}

TEST_F( OpenDDLDefectsTest, strings_with_the_comment_syntax_in_them_breaks_the_parser ) {
    char token[] = {
        "Material $material1\n"
        "{\n"
        "    Name { string { \"defaultMat\" } }\n"
        "\n"
        "    Color( attrib = \"diffuse\" ) { float[ 3 ] {{0.6400000190734865, 0.6400000190734865, 0.6400000190734865}} }\n"
        "    Color( attrib = \"specular\" ) { float[ 3 ] {{0.5, 0.5, 0.5}} }\n"
        "    Param( attrib = \"specular_power\" ) { float { 50 } }\n"
        "\n"
        "    Texture( attrib = \"diffuse\" )\n"
        "    {\n"
        "        string { \"//f/kittylow.bmp\" }\n"
        "    }\n"
        "}\n"
    };

    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );

}

TEST_F( OpenDDLDefectsTest, parse_light_object_issue38 ) {
    char token[] = {
        "LightObject $light1( type = \"point\" ) // Lamp\n"
        "{\n"
        "    Color( attrib = \"light\" ) { float[ 3 ]{ { 1.0, 1.0, 1.0 } } }\n"
        "\n"
        "    Atten( curve = \"inverse_square\" )\n"
        "    {\n"
        "        Param( attrib = \"scale\" ) { float{ 5.47722400800463 } }\n"
        "    }\n"
        "}\n"
    };
    
    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );
    DDLNode *root = myParser.getRoot();
    EXPECT_TRUE(nullptr != root);

}

TEST_F( OpenDDLDefectsTest, parse_hexa_float_issue ) {
    char token[] = {
        "VertexArray( attrib = \"position\" )\n"
        "{\n"
        "   float[ 3 ]		// 24\n"
        "   {\n"
        "        { 0xC2501375, 0xC24C468A, 0x00000000 },{ 0xC2501375, 0x424C468A, 0x00000000 },{ 0x42501375, 0x424C468A, 0x00000000 },{ 0x42501375, 0xC24C468A, 0x00000000 },{ 0xC2501375, 0xC24C468A, 0x42BA3928 },{ 0x42501375, 0xC24C468A, 0x42BA3928 },{ 0x42501375, 0x424C468A, 0x42BA3928 },{ 0xC2501375, 0x424C468A, 0x42BA3928 },\n"
        "        { 0xC2501375, 0xC24C468A, 0x00000000 },{ 0x42501375, 0xC24C468A, 0x00000000 },{ 0x42501375, 0xC24C468A, 0x42BA3928 },{ 0xC2501375, 0xC24C468A, 0x42BA3928 },{ 0x42501375, 0xC24C468A, 0x00000000 },{ 0x42501375, 0x424C468A, 0x00000000 },{ 0x42501375, 0x424C468A, 0x42BA3928 },{ 0x42501375, 0xC24C468A, 0x42BA3928 },\n"
        "        { 0x42501375, 0x424C468A, 0x00000000 },{ 0xC2501375, 0x424C468A, 0x00000000 },{ 0xC2501375, 0x424C468A, 0x42BA3928 },{ 0x42501375, 0x424C468A, 0x42BA3928 },{ 0xC2501375, 0x424C468A, 0x00000000 },{ 0xC2501375, 0xC24C468A, 0x00000000 },{ 0xC2501375, 0xC24C468A, 0x42BA3928 },{ 0xC2501375, 0x424C468A, 0x42BA3928 }\n"
        "   }\n"
        "}\n"
    };
    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );
    DDLNode *root = myParser.getRoot();
    EXPECT_TRUE(nullptr != root);
    DDLNode::DllNodeList childs = root->getChildNodeList();
    EXPECT_EQ( 1U, childs.size() );
}

TEST_F( OpenDDLDefectsTest, invalid_size_dataarraylist_issue_41 ) {
    DataArrayList *list = new DataArrayList;
    EXPECT_EQ( 0U, list->size() );
    
    list->m_dataList = new Value(Value::ValueType::ddl_bool);
    EXPECT_EQ( 1U, list->size() );
    delete list;
}

TEST_F( OpenDDLDefectsTest, invalid_handling_of_embedded_comments ) {
    char token[] = {
        "GeometryObject /*mesh*/ $geometry1 {\n"
        "    Mesh (primitive = \"triangle_strip\") {\n"
        "        VertexArray (attrib = \"position\") {\n" 
        "            float[3] {\n"
        "                {0.0, 1.0, 3.0},\n"
        "                {-1.0, 2.0, 2.0},\n" 
        "                {3.0, 3.0, 1.0}\n"
        "            }\n"
        "        }\n"
        "    }\n"
        "}\n"
    };	
    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );
}

TEST_F( OpenDDLDefectsTest, invalid_handling_of_unsigned_int_16 ) {
    char token[] = {
        "Metric( key = \"up\" ) { string{ \"y\" } }\n"
        "\n"
        "GeometryObject /*mesh*/{\n"
        "   Mesh( primitive = \"triangle_strip\" ) {\n"
        "       VertexArray( attrib = \"position\" ) {\n"
        "           float[ 3 ]{\n"
        "               { 0.0, 1.0, 3.0 },{ -1.0, 2.0, 2.0 },{ 3.0, 3.0, 1.0 }\n"
        "           }\n"
        "       }\n"
        "       VertexArray( attrib = \"normal\" ) {\n"
        "           float[ 3 ]{\n"
        "               { 0.0, 1.0, 0.0 },{ -1.0, 0.0, 0.0 },{ 0.0, 0.0, 1.0 }\n"
        "           }\n"
        "       }\n"
        "       VertexArray( attrib = \"texcoord\" ) {\n"
        "           float[ 2 ]{\n"
        "               { 0.5, 0.5 },{ 0.5, 1.0 },{ 1.0, 1.0 }\n"
        "           }\n"
        "       }\n"
        "       VertexArray( attrib = \"texcoord\" ) {\n"
        "           float[ 2 ]{\n"
        "               { 0.5, 1.0 },{ 1.0, 0.5 },{ 0.5, 0.5 }\n"
        "           }\n"
        "       }\n"
        "       VertexArray( attrib = \"color\" ) { float{} }\n"
        "   }\n"
        "}\n"
        "\n"
        "GeometryObject /*meshIndexed*/{\n"
        "   Mesh( primitive = \"triangles\" ) {\n"
        "       VertexArray( attrib = \"position\" ) {\n"
        "           float[ 3 ]{\n"
        "               { 0.0, 1.0, 3.0 },{ -1.0, 2.0, 2.0 },{ 3.0, 3.0, 1.0 },{ 5.0, 7.0, 0.5 }\n"
        "           }\n"
        "       }\n"
        "\n"
        "   IndexArray{ unsigned_int16[ 3 ]{\n"
        "       { 2, 0, 1 },{ 1, 2, 3 }\n"
        "   }\n" 
        "}\n"
        "}\n"
        "}\n"
        "\n"
        "GeometryObject /*meshEnlargeShrink*/{\n"
        "Mesh{\n"
        "VertexArray( attrib = \"position\" ) {\n"
        "float[ 4 ]{\n"
        "    { 0.0, 1.0, 3.0, 0.5 },{ -1.0, 2.0, 2.0, 1.0 },{ 3.0, 3.0, 1.0, 1.0 }\n"
        "}\n"
        "}\n"
        "VertexArray( attrib = \"normal\" ) {\n"
        "float[ 2 ]{\n"
        "    { 0.0, 1.0 },{ -1.0, 0.0 },{ 1.0, 0.0 }\n"
        "}\n"
        "}\n"
        "VertexArray( attrib = \"texcoord\" ) {\n"
        "    float[ 1 ]{\n"
        "        { 0.5 },{ 0.0 },{ 1.0 }\n"
        "    }\n"
        "}\n"
        "}\n"
        "}\n" 
    };
    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    //EXPECT_TRUE( ok );
}

TEST_F( OpenDDLDefectsTest, invalid_normalizing ) {
    char token[] = {
        "Metric( key = \"up\" ) { string{ \"y\" } }\n"
        "\n"
        "GeometryObject /*mesh*/{\n"
        "   Mesh( primitive = \"triangle_strip\" ) {\n"
        "       VertexArray( attrib = \"position\" ) {\n"
        "           float[ 3 ]{\n"
        "               { 0.0, 1.0, 3.0 },{ -1.0, 2.0, 2.0 },{ 3.0, 3.0, 1.0 }\n"
        "           }\n"
        "       }\n"
        "   }\n"
        "}\n"
    };
     
    OpenDDLParser myParser;
    myParser.setBuffer( token, strlen( token ) );
    const bool ok( myParser.parse() );
    EXPECT_TRUE( ok );
}
    
END_ODDLPARSER_NS
