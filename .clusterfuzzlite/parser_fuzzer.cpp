#include <fuzzer/FuzzedDataProvider.h>
#include <string>

#include <openddlparser/OpenDDLParser.h>
#include <iostream>

USE_ODDLPARSER_NS;

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    OpenDDLParser theParser;
    theParser.setBuffer((const char *)data, size);
    theParser.parse();

    return 0;
}