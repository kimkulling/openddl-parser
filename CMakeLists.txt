CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( OpenDDL-Parser )
SET ( OPENDDL_PARSER_VERSION_MAJOR 0 )
SET ( OPENDDL_PARSER_VERSION_MINOR 1 )
SET ( OPENDDL_PARSER_VERSION_PATCH 0 )
SET ( OPENDDL_PARSER_VERSION ${CPPCORE_VERSION_MAJOR}.${CPPCORE_VERSION_MINOR}.${CPPCORE_VERSION_PATCH} )
SET ( PROJECT_VERSION "${OPENDDL_PARSER_VERSION}" )

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    find_package(Threads)
endif()

add_definitions( -DOPENDDLPARSER_BUILD )
add_definitions( -D_VARIADIC_MAX=10 )

INCLUDE_DIRECTORIES(
	./
	include/
    contrib/gtest-1.7.0/include
    contrib/gtest-1.7.0/
)

link_directories(
	./
)

SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin )

if( WIN32 AND NOT CYGWIN )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )  # Force to always compile with W4
  if( CMAKE_CXX_FLAGS MATCHES "/W[0-4]" )
    string( REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
  else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4" )
  endif()
elseif( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++0x")
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++11")
endif()

SET ( openddl_parser_src
	code/OpenDDLParser.cpp
    include/openddlparser/OpenDDLParser.h
)
 
SOURCE_GROUP( code            FILES ${openddl_parser_src} )

ADD_LIBRARY( openddl_parser SHARED
    ${openddl_parser_src}
)

SET ( GTEST_PATH contrib/gtest-1.7.0 )

SET ( gtest_src
    ${GTEST_PATH}/src/gtest-death-test.cc
    ${GTEST_PATH}/src/gtest-filepath.cc
    ${GTEST_PATH}/src/gtest-internal-inl.h
    ${GTEST_PATH}/src/gtest-port.cc
    ${GTEST_PATH}/src/gtest-printers.cc
    ${GTEST_PATH}/src/gtest-test-part.cc
    ${GTEST_PATH}/src/gtest-typed-test.cc
    ${GTEST_PATH}/src/gtest.cc
    ${GTEST_PATH}/src/gtest_main.cc
)

SET( openddl_parser_test_src  
    test/OpenDDLParserTest.cpp 
)
SOURCE_GROUP( code            FILES ${openddl_parser_test_src} )

ADD_EXECUTABLE( openddl_parser_test
    ${gtest_src}
    ${openddl_parser_test_src}
)

target_link_libraries( openddl_parser_test openddl_parser ${CMAKE_THREAD_LIBS_INIT} )
