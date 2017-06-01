cmake_minimum_required(VERSION 3.6)

project(runUnitTests)

#-Google Test--------------------------------------------------------------------------------------#
enable_testing()
find_package(GTest REQUIRED)
find_package (Threads)
include_directories(${GTEST_INCLUDE_DIRS})
#--------------------------------------------------------------------------------------------------#

#-libDwarf-----------------------------------------------------------------------------------------#
include_directories(/home/wilhelma/tools/libdwarf/libdwarf)
link_directories(/home/wilhelma/tools/libdwarf/libdwarf)
#--------------------------------------------------------------------------------------------------#

#-json11-------------------------------------------------------------------------------------------#
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/json11)
find_library(json11_LIBRARY NAMES json11 PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json11)
#--------------------------------------------------------------------------------------------------#

set(test_SRCS ../../architecture/test/ArchBuilderTest.cpp
        ../../architecture/test/Utility.h
        ../../architecture/test/Utility.cpp
        ../../architecture/src/ArchBuilder.cpp)

add_executable(runUnitTests gtest.cpp ${test_SRCS})

target_link_libraries(runUnitTests ${GTEST_BOTH_LIBRARIES} pthread dwarf elf z ${json11_LIBRARY})
