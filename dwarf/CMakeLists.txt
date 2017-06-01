cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(DwarfLoader)

set(CMAKE_CXX_STANDARD 11)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/DwarfReader.cpp
        ${PROJECT_SOURCE_DIR}/Type.cpp
        ${PROJECT_SOURCE_DIR}/Context.cpp
        ${PROJECT_SOURCE_DIR}/DwarfHelper.cpp
        ${PROJECT_SOURCE_DIR}/Filter.cpp
        ${PROJECT_SOURCE_DIR}/Duplicate.cpp)

#------------------------------ json11 library ----------------------------------------------------#
ExternalProject_Add(json11
        GIT_REPOSITORY "https://github.com/dropbox/json11.git"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        INSTALL_COMMAND ""
        BUILD_COMMAND PATH=$ENV{PATH} make
        SOURCE_DIR "${CMAKE_SOURCE_DIR}/3rdparty/json11"
        BINARY_DIR "${CMAKE_SOURCE_DIR}/3rdparty/json11"
        CMAKE_ARGS -DJSON11_BUILD_TESTS=off
        TEST_COMMAND "")

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/json11)
link_directories(${CMAKE_SOURCE_DIR}/3rdparty/json11)
#find_library(json11_LIBRARY NAMES json11 PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json11)
#--------------------------------------------------------------------------------------------------#

add_definitions(-DBIGARRAY_MULTIPLIER=1 -DTARGET_IA32E -DHOST_IA32E -DTARGET_LINUX)
include_directories(${PIN_INCLUDE_DIRS})
include_directories("${PROJECT_INCLUDE_DIR}")
link_directories (${PIN_LIBRARY_DIRS})
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PUBLIC -fno-omit-frame-pointer)
add_dependencies(${PROJECT_NAME} libdwarf)
target_link_libraries(${PROJECT_NAME} dwarf elf z pin pindwarf pinvm {CMAKE_THREAD_LIBS_INIT})

#add_subdirectory(test)
