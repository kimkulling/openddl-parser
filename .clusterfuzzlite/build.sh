#!/bin/bash -eu
# Build project
mkdir build
cd build
cmake ../
make

# Copy all fuzzer executables to $OUT/
$CXX $CFLAGS $LIB_FUZZING_ENGINE -stdlib=libc++ \
  $SRC/openddl-parser/.clusterfuzzlite/parser_fuzzer.cpp \
  -o $OUT/parser_fuzzer \
  -I$SRC/openddl-parser/include \
  $SRC/openddl-parser/build/lib/libopenddlparser.a
