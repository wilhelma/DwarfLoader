cmake_minimum_required(VERSION 3.2)
include(ExternalProject)

project(DwarfLoader)

set (CMAKE_CXX_STANDARD 11)

set (PROJECT_INCLUDE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/dwarf/include)
set (PROJECT_SOURCE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/dwarf/src)
set (ARCHITECTURE_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/architecture/include)
set (ARCHITECTURE_SOURCE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/architecture/src)
set (AST_DIR                    ${CMAKE_CURRENT_SOURCE_DIR}/ast)

set(SOURCE_FILES
        src/main.cpp
        ${PROJECT_SOURCE_DIR}/DwarfReader.cpp
        ${PROJECT_SOURCE_DIR}/Type.cpp
        ${PROJECT_SOURCE_DIR}/Context.cpp
        ${PROJECT_SOURCE_DIR}/DwarfHelper.cpp
        ${PROJECT_SOURCE_DIR}/Filter.cpp
        ${PROJECT_SOURCE_DIR}/Duplicate.cpp
        ${ARCHITECTURE_SOURCE_DIR}/OrOperatorRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/AndOperatorRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/SetOperatorRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/ArchBuilder.cpp
        ${ARCHITECTURE_SOURCE_DIR}/ArchRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/ClassRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/NamespaceRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/NotOperatorRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/FunctionRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/VariableRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/RegexFileRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/RegexNameRule.cpp
        ${ARCHITECTURE_SOURCE_DIR}/RegexRule.cpp
        ${AST_DIR}/visitor/ASTVisitor.cpp
        ${AST_DIR}/visitor/VisitorContext.cpp
        ${AST_DIR}/ast-from-json/CreateAstFromJson.cpp
        ${AST_DIR}/nodes/Program.cpp
        ${AST_DIR}/nodes/DefinitionExpression.cpp
        ${AST_DIR}/nodes/Component.cpp
        ${AST_DIR}/nodes/AndExpression.cpp
        ${AST_DIR}/nodes/OrExpression.cpp
        ${AST_DIR}/nodes/NotExpression.cpp
        ${AST_DIR}/nodes/AtomExpression.cpp
        ${AST_DIR}/nodes/SetExpression.cpp
        ${AST_DIR}/nodes/Expression.cpp)

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
#--------------------------------------------------------------------------------------------------#

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${ARCHITECTURE_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PUBLIC -fno-omit-frame-pointer)
target_link_libraries(${PROJECT_NAME} dwarf elf ${CMAKE_SOURCE_DIR}/3rdparty/json11/libjson11.a)

