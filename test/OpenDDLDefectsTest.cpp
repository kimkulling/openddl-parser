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

#include <openddlparser/OpenDDLCommon.h>
#include <openddlparser/OpenDDLParser.h>
#include "UnitTestCommon.h"

BEGIN_ODDLPARSER_NS

class OpenDDLDefectsTest : public testing::Test {
};

TEST_F( OpenDDLDefectsTest, Issue20_WrongColorNodeParsing ) {
    char token[] = "{0.588235, 0.588235, 0.588235}\n";
    size_t len( 0 );
    char *end = findEnd( token, len );

    Value *data( ddl_nullptr );
    Reference *refs( ddl_nullptr );
    size_t numRefs( 0 ), numValues( 0 );
    Value::ValueType type( Value::ddl_none );
    char *in = OpenDDLParser::parseDataList( token, end, type, &data, numValues, &refs, numRefs );
    ASSERT_FALSE( ddl_nullptr == in );
    ASSERT_FALSE( ddl_nullptr == data );
    ASSERT_EQ( 3, numValues );
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

END_ODDLPARSER_NS
