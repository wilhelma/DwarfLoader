cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(DwarfLoader)

add_subdirectory(test)

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
                 src/DwarfHelper.cpp
                 include/DwarfHelper.h
                 architecture/ArchBuilder.h
                 architecture/ArchRule.h
                 architecture/NamespaceRule.cpp
                 architecture/NamespaceRule.h
                 architecture/ClassRule.cpp
                 architecture/ClassRule.h
                 architecture/RegexRule.cpp
                 architecture/RegexRule.h
                 architecture/RegexNameRule.h
                 architecture/RegexNameRule.cpp
                 architecture/RegexFileRule.h
                 architecture/RegexFileRule.cpp
                 architecture/ArchBuilder.cpp
                 include/Filter.h src/Filter.cpp
                 include/Duplicate.h
                 src/Duplicate.cpp
                 include/CuFileManager.h
        architecture/ArchRule.cpp)

#------------------------------ json11 library ----------------------------------------------------#
ExternalProject_Add(json11lib
        GIT_REPOSITORY "https://github.com/dropbox/json11.git"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        INSTALL_COMMAND ""
        BUILD_COMMAND PATH=$ENV{PATH} make
	SOURCE_DIR "${CMAKE_BINARY_DIR}/3rdparty/json11"
	BUILD_IN_SOURCE 1 
        CMAKE_ARGS -DJSON11_BUILD_TESTS=off
        TEST_COMMAND "")

link_directories(${CMAKE_BINARY_DIR}/3rdparty/json11)
include_directories(${CMAKE_BINARY_DIR}/3rdparty/json11)
#--------------------------------------------------------------------------------------------------#

add_executable(DwarfLoader ${SOURCE_FILES})
add_dependencies(DwarfLoader json11lib)
target_compile_options(DwarfLoader PUBLIC -fno-omit-frame-pointer)
target_link_libraries(DwarfLoader dwarf elf z json11 ${CMAKE_THREAD_LIBS_INIT})

