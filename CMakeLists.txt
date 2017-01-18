cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(DwarfLoader)

set(CMAKE_CXX_STANDARD 11)

include_directories(/home/wilhelma/tools/libdwarf/libdwarf)
link_directories(/home/wilhelma/tools/libdwarf/libdwarf)

set(SOURCE_FILES src/main.cpp
                 entities/SoftwareEntity.h
                 entities/Class.h
                 entities/Variable.h
                 entities/Image.h
                 entities/Namespace.h
                 include/DwarfReader.h
                 tag/TagGeneric.h
                 tag/TagMember.impl
                 src/DwarfReader.cpp
                 include/Context.h
                 include/Type.h src/Type.cpp
                 include/DwarfException.h
                 guards/FileGuard.h
                 guards/DbgGuard.h
                 src/Context.cpp
                 include/DwarfHelper.cpp
                 include/DwarfHelper.h
                 architecture/ArchBuilder.h
                 architecture/ArchRule.h
                 architecture/NamespaceRule.cpp
                 architecture/NamespaceRule.h
                 architecture/ClassRule.cpp
                 architecture/ClassRule.h
                 architecture/ArchBuilder.cpp)
add_executable(DwarfLoader ${SOURCE_FILES})

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
find_library(json11_LIBRARY NAMES json11 PATHS ${CMAKE_SOURCE_DIR}/3rdparty/json11)
#--------------------------------------------------------------------------------------------------#

target_compile_options(DwarfLoader PUBLIC -fno-omit-frame-pointer -Wterminate)

target_link_libraries(DwarfLoader dwarf elf z ${json11_LIBRARY})