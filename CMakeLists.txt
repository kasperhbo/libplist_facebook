#
# Copyright (c) 2015-present, Facebook, Inc.
# All rights reserved.
#
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.
#

cmake_minimum_required(VERSION 3.18)
project(plist)

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_TESTING "Build unit tests" OFF)

add_library(plist
            Sources/ObjectType.cpp
            Sources/Array.cpp
            Sources/Boolean.cpp
            Sources/Data.cpp
            Sources/Date.cpp
            Sources/Dictionary.cpp
            Sources/Integer.cpp
            Sources/Null.cpp
            Sources/Object.cpp
            Sources/Real.cpp
            Sources/String.cpp
            Sources/UID.cpp
            #
            Sources/Base64.cpp
            Sources/rfc4648.c
            Sources/ISODate.cpp
            Sources/UnixTime.cpp
            #
            Sources/Keys/Unpack.cpp
            #
            Sources/Format/Encoding.cpp
            Sources/Format/unicode.c
            #
            Sources/Format/BaseXMLParser.cpp
            Sources/Format/XMLParser.cpp
            Sources/Format/XMLWriter.cpp
            Sources/Format/XML.cpp
            #
            Sources/Format/SimpleXMLParser.cpp
            Sources/Format/SimpleXML.cpp
            #
            Sources/Format/ABPContext.cpp
            Sources/Format/ABPReader.cpp
            Sources/Format/ABPWriter.cpp
            Sources/Format/Binary.cpp
            #
            Sources/Format/ASCIIPListLexer.cpp
            Sources/Format/ASCIIParser.cpp
            Sources/Format/ASCIIWriter.cpp
            Sources/Format/ASCII.cpp
            #
            Sources/Format/JSONParser.cpp
            Sources/Format/JSONWriter.cpp
            Sources/Format/JSON.cpp
            #
            Sources/Format/Any.cpp
            )
target_compile_options(plist BEFORE PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-std=c++11>")

if ("${CMAKE_SYSTEM_NAME}" MATCHES "Windows")
  if ("${CMAKE_CXX_PLATFORM_ID}" STREQUAL "MinGW")
    # MinGW is missing the library for XmlLite.
    target_link_libraries(plist PRIVATE ole32)
  else ()
    target_link_libraries(plist PRIVATE xmllite Ole32)
  endif ()
else ()
  if ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    find_path(LIBXML2_INCLUDE_DIR NAMES "xmlversion.h" PATH_SUFFIXES "libxml" REQUIRED)
    find_library(LIBXML2_LIBRARIES NAMES xml2 REQUIRED)
  else ()
    find_package(LibXml2 REQUIRED)
  endif()
  if ("${CMAKE_SYSTEM_NAME}" MATCHES "OpenBSD")
    target_include_directories(plist PRIVATE "${LIBXML2_INCLUDE_DIRS}")
  else ()
    target_include_directories(plist PRIVATE "${LIBXML2_INCLUDE_DIR}")
  endif ()
  target_compile_definitions(plist PRIVATE "${LIBXML2_DEFINITIONS}")
  target_link_libraries(plist PRIVATE ${LIBXML2_LIBRARIES})
endif ()

target_include_directories(plist PUBLIC "${CMAKE_SOURCE_DIR}/Headers")
target_include_directories(plist PRIVATE "${CMAKE_SOURCE_DIR}/PrivateHeaders")
install(TARGETS plist DESTINATION lib)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/Headers/plist" DESTINATION include)

add_library(util STATIC
            Util/Absolute.cpp
            Util/DefaultFilesystem.cpp
            Util/Filesystem.cpp
            Util/FSUtil.cpp
            Util/Options.cpp
            Util/Permissions.cpp
            Util/Relative.cpp
            Util/Unix.cpp
            Util/Windows.cpp
            )
target_compile_options(util BEFORE PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-std=c++17>")

add_executable(plutil Tools/plutil.cpp)
target_compile_options(plutil BEFORE PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-std=c++17>")
target_link_libraries(plutil PRIVATE plist util)
install(TARGETS plutil DESTINATION bin)

add_executable(PlistBuddy Tools/PlistBuddy.cpp)
target_compile_options(PlistBuddy BEFORE PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-std=c++17>")
target_link_libraries(PlistBuddy PRIVATE plist util)
install(TARGETS PlistBuddy DESTINATION bin)

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  # TODO
else ()
  set(LINENOISE_ROOT "${CMAKE_SOURCE_DIR}/ThirdParty/linenoise")
  set(LINENOISE_SOURCE "${LINENOISE_ROOT}/linenoise.c")
  if (EXISTS "${LINENOISE_SOURCE}")
    add_library(linenoise STATIC "${LINENOISE_SOURCE}")
    target_include_directories(linenoise PUBLIC "${LINENOISE_ROOT}")
    if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
      target_compile_definitions(linenoise PRIVATE _GNU_SOURCE)
    endif ()

    target_link_libraries(PlistBuddy PRIVATE linenoise)
    target_compile_definitions(PlistBuddy PRIVATE HAVE_LINENOISE)
  endif ()
endif ()

if (BUILD_TESTING)
  set(INSTALL_GTEST OFF)
  add_subdirectory(ThirdParty/googletest)

  function (ADD_UNIT_GTEST LIBRARY NAME SOURCES)
    set(TARGET_NAME "test_${LIBRARY}_${NAME}")
    add_executable("${TARGET_NAME}" ${SOURCES})
    target_link_libraries("${TARGET_NAME}" PRIVATE "${LIBRARY}" gtest gtest_main)
    target_include_directories("${TARGET_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/ThirdParty/googletest/googletest/include")
    add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
  endfunction ()

  ADD_UNIT_GTEST(plist Boolean Tests/test_Boolean.cpp)
  ADD_UNIT_GTEST(plist Real Tests/test_Real.cpp)
  ADD_UNIT_GTEST(plist String Tests/test_String.cpp)
  ADD_UNIT_GTEST(plist Encoding Tests/Format/test_Encoding.cpp)
  ADD_UNIT_GTEST(plist ASCII Tests/Format/test_ASCII.cpp)
  ADD_UNIT_GTEST(plist Binary Tests/Format/test_Binary.cpp)
  ADD_UNIT_GTEST(plist JSON Tests/Format/test_JSON.cpp)
  ADD_UNIT_GTEST(plist XML Tests/Format/test_XML.cpp)
endif ()
