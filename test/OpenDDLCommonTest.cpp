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
#include <openddlparser/Value.h>

BEGIN_ODDLPARSER_NS

static const char TestText[] = "hello";

class OpenDDLCommonTest : public testing::Test {
    Text *m_text;

public:
    void SetUp() override {
        m_text = nullptr;
    }

    void TearDown() override {
        delete m_text;
        m_text = nullptr;
    }

    const char *getTestText() const {
        return &TestText[0];
    }

    size_t getTestTextLen() {
        return strlen(TestText);
    }

    Text *createTestText() {
        m_text = new Text(getTestText(), getTestTextLen());
        return m_text;
    }
};

TEST_F(OpenDDLCommonTest, typeSizesTest) {
    EXPECT_EQ(1U, sizeof(int8));
    EXPECT_EQ(2U, sizeof(int16));
    EXPECT_EQ(4U, sizeof(int32));
    EXPECT_EQ(8U, sizeof(int64));

    EXPECT_EQ(1U, sizeof(uint8));
    EXPECT_EQ(2U, sizeof(uint16));
    EXPECT_EQ(4U, sizeof(uint32));
    EXPECT_EQ(8U, sizeof(uint64));
}

TEST_F(OpenDDLCommonTest, createNameTest) {
    bool ok(true);
    Name *name = nullptr;
    try {
        name = new Name(GlobalName, nullptr);
    } catch (...) {
        ok = false;
        name = nullptr;
    }
    EXPECT_TRUE(ok);
    delete name;
}

TEST_F(OpenDDLCommonTest, createTextTest) {
    bool ok(true);
    try {
        static_cast<void>(createTestText());
    } catch (...) {
        ok = false;
    }
    EXPECT_TRUE(ok);
}

TEST_F(OpenDDLCommonTest, accessTextTest) {
    Text *theText = createTestText();
    ASSERT_TRUE(nullptr != theText);

    EXPECT_EQ(getTestTextLen(), theText->m_len);
    int res(strncmp(getTestText(), theText->m_buffer, getTestTextLen()));
    EXPECT_EQ(0, res);
}

TEST_F(OpenDDLCommonTest, clearTextTest) {
    Text *theText = createTestText();
    ASSERT_TRUE(nullptr != theText);

    theText->clear();
    EXPECT_EQ(0U, theText->m_len);
    EXPECT_TRUE(nullptr == theText->m_buffer);
}

TEST_F(OpenDDLCommonTest, setTextTest) {
    Text *theText = createTestText();
    ASSERT_FALSE(nullptr == theText);

    static const std::string test2 = "Hello, World!";
    theText->set(test2.c_str(), test2.size());

    EXPECT_EQ(test2.size(), theText->m_len);
    const int res(strncmp(test2.c_str(), theText->m_buffer, test2.size()));
    EXPECT_EQ(0, res);
}

TEST_F(OpenDDLCommonTest, CompareIdentifierTest) {
    Text id1("test", 4), id2("test", 4);
    EXPECT_EQ(id1, id2);
}

TEST_F(OpenDDLCommonTest, sizeAdatArrayListTest) {
    DataArrayList *list = new DataArrayList;
    EXPECT_EQ(0U, list->size());
    DataArrayList *listNext = new DataArrayList;
    list->m_next = listNext;
    EXPECT_EQ(1U, list->size());

    delete list;
}

TEST_F(OpenDDLCommonTest, sizeInBytesReferenceTest) {
    Text *id = new Text("test", 4);
    Name *name = new Name(GlobalName, id);
    Reference *ref1(new Reference(1, &name));
    size_t size(ref1->sizeInBytes());
    EXPECT_EQ(4U, size);
    delete ref1;

    Reference *ref2(new Reference(0, nullptr));
    size = ref2->sizeInBytes();
    EXPECT_EQ(0U, size);
    delete ref2;
}

END_ODDLPARSER_NS
