CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( OpenDDL-Parser )
SET ( OPENDDL_PARSER_VERSION_MAJOR 0 )
SET ( OPENDDL_PARSER_VERSION_MINOR 1 )
SET ( OPENDDL_PARSER_VERSION_PATCH 0 )
SET ( OPENDDL_PARSER_VERSION ${CPPCORE_VERSION_MAJOR}.${CPPCORE_VERSION_MINOR}.${CPPCORE_VERSION_PATCH} )
SET ( PROJECT_VERSION "${OPENDDL_PARSER_VERSION}" )

option( DDL_CHECK_CODE_COVERAGE "Set to ON to track code coverage in unit tests ( only supported for gcc ). " OFF )
option( DDL_USE_CPP11           "Set to ON to use C++11 features." OFF )

if ( DDL_USE_CPP11 )
else()
    add_definitions( -DOPENDDL_NO_USE_CPP11 )
endif()

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    find_package(Threads)
else()
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

add_definitions( -DOPENDDLPARSER_BUILD )
add_definitions( -D_VARIADIC_MAX=10 )
add_definitions( -DGTEST_HAS_PTHREAD=0 )

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
  if ( DDL_CHECK_CODE_COVERAGE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage -Wall -Wno-long-long -pedantic -std=c++0x")
  else( DDL_CHECK_CODE_COVERAGE )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++0x")
  endif( DDL_CHECK_CODE_COVERAGE )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++11 -Wwrite-strings")
endif()

# Include the doc component.
ADD_SUBDIRECTORY( doc )
FIND_PACKAGE( doxygen )
IF ( DOXYGEN_FOUND )
    CONFIGURE_FILE( doc/openddlparser_doc.in doc/doxygenfile @ONLY )
    ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} doc/doxygenfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM )
ENDIF ( DOXYGEN_FOUND )

SET ( openddl_parser_src
    code/OpenDDLCommon.cpp
    code/OpenDDLExport.cpp
    code/OpenDDLParser.cpp
    code/DDLNode.cpp
    code/Value.cpp
    include/openddlparser/OpenDDLCommon.h
    include/openddlparser/OpenDDLExport.h
    include/openddlparser/OpenDDLParser.h
    include/openddlparser/OpenDDLParserUtils.h
    include/openddlparser/DDLNode.h
    include/openddlparser/Value.h
    README.md
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

SET( openddl_parser_unittest_src  
    test/UnitTestCommon.h
    test/DDLNodeTest.cpp
    test/OpenDDLCommonTest.cpp
    test/OpenDDLExportTest.cpp
    test/OpenDDLParserTest.cpp
    test/OpenDDLParserUtilsTest.cpp
    test/OpenDDLIntegrationTest.cpp
    test/ValueTest.cpp
    test/OpenDDLDefectsTest.cpp
)

SOURCE_GROUP( code  FILES ${openddl_parser_unittest_src} )
SOURCE_GROUP( gtest FILES ${gtest_src} )

ADD_EXECUTABLE( openddl_parser_unittest 
    ${gtest_src}
    ${openddl_parser_unittest_src}
)

target_link_libraries( openddl_parser_unittest openddl_parser ${CMAKE_THREAD_LIBS_INIT} )

SET( openddl_parser_demo_src  
    demo/main.cpp 
)

ADD_EXECUTABLE( openddl_parser_demo
    ${openddl_parser_demo_src}
) 

target_link_libraries( openddl_parser_demo openddl_parser )
