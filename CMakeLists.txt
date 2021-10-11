CMAKE_MINIMUM_REQUIRED( VERSION 3.1 )
PROJECT( OpenDDL-Parser )
SET ( openddlparser_VERSION_MAJOR 0 )
SET ( openddlparser_VERSION_MINOR 1 )
SET ( openddlparser_VERSION_PATCH 0 )
SET ( openddlparser_VERSION ${openddlparser_VERSION_MAJOR}.${openddlparser_VERSION_MINOR}.${openddlparser_VERSION_PATCH} )
SET ( PROJECT_VERSION "${openddlparser_VERSION}" )

option( DDL_DEBUG_OUTPUT        "Set to ON to use output debug texts"                                         OFF )
option( DDL_STATIC_LIBRARY		"Set to ON to build static libary of OpenDDL Parser."                         ON )
option( COVERALLS               "Generate coveralls data"                                                     OFF )
option( DDL_BUILD_TESTS         "Set to OFF to not build tests by default"                                    ON )
option( DDL_BUILD_PARSER_DEMO   "Set to OFF to opt out building parser demo"                                  ON )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    find_package(Threads)
else()
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()

if ( DDL_STATIC_LIBRARY )
	add_definitions( -DOPENDDL_STATIC_LIBARY )
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING")
    add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
endif()

add_definitions( -DOPENDDLPARSER_BUILD )
add_definitions( -D_VARIADIC_MAX=10 )
add_definitions( -DGTEST_HAS_PTHREAD=0 )
if ( DDL_DEBUG_OUTPUT )
    add_definitions( -DDDL_DEBUG_HEADER_NAME)
endif()

INCLUDE_DIRECTORIES(
    ./
    include/
    contrib/gtest-1.7.0/include
    contrib/gtest-1.7.0/
)

link_directories(
    ${CMAKE_HOME_DIRECTORY}/lib
)

SET( CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake )
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic ${OPENDDL_CXXFLAGS}")
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic ${OPENDDL_CXXFLAGS} -Wwrite-strings")
endif()

if (COVERALLS)
    include(Coveralls)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

# Include the doc component.
FIND_PACKAGE( doxygen )
IF ( DOXYGEN_FOUND )
    CONFIGURE_FILE( doc/openddlparser_doc.in doc/doxygenfile @ONLY )
    ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} doc/doxygenfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM )
ENDIF ( DOXYGEN_FOUND )

SET ( openddlparser_headers
    include/openddlparser/OpenDDLCommon.h
    include/openddlparser/OpenDDLExport.h
    include/openddlparser/OpenDDLParser.h
    include/openddlparser/OpenDDLParserUtils.h
    include/openddlparser/OpenDDLStream.h
    include/openddlparser/DDLNode.h
    include/openddlparser/Value.h
    include/openddlparser/TPoolAllocator.h
)
SET ( openddlparser_src
    code/OpenDDLCommon.cpp
    code/OpenDDLExport.cpp
    code/OpenDDLParser.cpp
    code/OpenDDLStream.cpp
    code/DDLNode.cpp
    code/Value.cpp
    README.md
)

SOURCE_GROUP( code            FILES ${openddlparser_src} )

if ( DDL_STATIC_LIBRARY )
	ADD_LIBRARY( openddlparser STATIC ${openddlparser_src} ${openddlparser_headers})
else()
	ADD_LIBRARY( openddlparser SHARED ${openddlparser_src} ${openddlparser_headers})
endif()

set_target_properties( openddlparser PROPERTIES PUBLIC_HEADER "${openddlparser_headers}")

if (DDL_BUILD_TESTS)
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
    )

    SET( openddlparser_unittest_src
        test/UnitTestCommon.h
        test/DDLNodeTest.cpp
        test/OpenDDLCommonTest.cpp
        test/OpenDDLExportTest.cpp
        test/OpenDDLParserTest.cpp
        test/OpenDDLParserUtilsTest.cpp
        test/OpenDDLStreamTest.cpp
        test/OpenDDLIntegrationTest.cpp
        test/ValueTest.cpp
        test/OpenDDLDefectsTest.cpp
        test/OssFuzzTest.cpp
        test/main.cpp
    )
    add_definitions(-DOPENDDL_TEST_DATA="${CMAKE_CURRENT_LIST_DIR}/test/TestData")

    SOURCE_GROUP( code  FILES ${openddlparser_unittest_src} )
    SOURCE_GROUP( gtest FILES ${gtest_src} )

    ADD_EXECUTABLE( openddlparser_unittest
        ${gtest_src}
        ${openddlparser_unittest_src}
    )

    target_link_libraries( openddlparser_unittest openddlparser ${CMAKE_THREAD_LIBS_INIT} )
endif ()

if (DDL_BUILD_PARSER_DEMO)
    SET( openddlparser_demo_src
        demo/main.cpp
    )

    if (COVERALLS)
        set(COVERAGE_SRCS     ${gtest_src} ${openddlparser_unittest_src} )

        # Create the coveralls target.
        coveralls_setup(
            "${COVERAGE_SRCS}" # The source files.
            ON                 # If we should upload.
            "${PROJECT_SOURCE_DIR}/cmake/") # (Optional) Alternate project cmake module path.
    endif()

    ADD_EXECUTABLE( openddlparser_demo
        ${openddlparser_demo_src}
    )

    target_link_libraries( openddlparser_demo openddlparser )
endif ()

include(GNUInstallDirs)

install(TARGETS openddlparser
        EXPORT  openddlparser-targets
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/openddlparser")

install(EXPORT openddlparser-targets
    FILE openddlparser-config.cmake
    NAMESPACE openddlparser::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/openddlparser")