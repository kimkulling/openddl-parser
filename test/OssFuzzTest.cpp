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
#include <string>
#include "gtest/gtest.h"

#include "UnitTestCommon.h"

char *data = "�$$indexarray";

BEGIN_ODDLPARSER_NS

class OssFuzzTest : public testing::Test {
public: 
    static void readFile(const char *name, std::vector<char> &buffer) {
        std::string fn = std::string(OPENDDL_TEST_DATA "/") + std::string(name);
        FILE *fileStream = ::fopen(fn.c_str(), "rb");
        fseek(fileStream, 0, SEEK_END);
        const size_t size = ftell(fileStream);
        buffer.resize(size);
        ::fread(&buffer[0], size, sizeof(char), fileStream);
        ::fclose(fileStream);
    }
};

TEST_F(OssFuzzTest, fuzz24806_undefinedBahavior) {
    OpenDDLParser myParser;
    myParser.setBuffer(data, 14);
    EXPECT_FALSE(myParser.parse());
}

TEST_F(OssFuzzTest, fuzz24587_undefinedBahavior) {
    std::vector<char> buffer;
    OssFuzzTest::readFile("clusterfuzz-testcase-minimized-assimp_fuzzer-5699047558742016", buffer);
    OpenDDLParser myParser;
    myParser.setBuffer(&buffer[0], buffer.size());
    bool ok = myParser.parse();
    EXPECT_FALSE(ok);
}

TEST_F(OssFuzzTest, fuzz24463_undefinedBahavior) {
    std::vector<char> buffer;
    OssFuzzTest::readFile("clusterfuzz-testcase-minimized-assimp_fuzzer-5161012492500992", buffer);
    OpenDDLParser myParser;
    myParser.setBuffer(&buffer[0], buffer.size());
    bool ok = myParser.parse();
    EXPECT_FALSE(ok);
}
    

END_ODDLPARSER_NS
