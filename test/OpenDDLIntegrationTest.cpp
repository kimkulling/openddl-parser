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

BEGIN_ODDLPARSER_NS

static char *findEnd( char *in, size_t &len ) {
    len = strlen( in );
    char *end( &in[ len ] + 1 );

    return end;
}

class OpenDDLIntegrationTest : public testing::Test {
};

TEST_F( OpenDDLIntegrationTest, parseMetricTest ) {
    size_t len( 0 );
    char token[] = "Metric( key = \"distance\" ) { float{ 1 } }";
    char *end( findEnd( token, len ) );

    bool result( false );
    OpenDDLParser theParser;
    theParser.setBuffer( token, strlen( token ), false );
    //result = theParser.parse();
    //EXPECT_TRUE( result );
}

END_ODDLPARSER_NS
