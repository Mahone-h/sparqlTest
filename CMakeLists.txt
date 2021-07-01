cmake_minimum_required(VERSION 3.15.3)
project(sparqlTest)

set(CMAKE_CXX_STANDARD 14)


#set(antlr4-Parser-include
#        ${PROJECT_SOURCE_DIR}/sparql/SparqlLexer.cpp
#        ${PROJECT_SOURCE_DIR}/sparql/SparqlParser.cpp
#        ${PROJECT_SOURCE_DIR}/sparql/SparqlListener.cpp
#        ${PROJECT_SOURCE_DIR}/sparql/SparqlVisitor.cpp
#        )
#foreach(src_file ${antlr4-Parser-include})
#    set_source_files_properties(
#            ${src_file}
#            PROPERTIES
#            GENERATED TRUE
#    )
#endforeach(src_file ${antlr4-Parser-include})
#
#add_custom_target(GenerateParser DEPENDS ${antlr4-Parser-include})
#以上表明antlr4-Parser-include 已经生成 https://cmake.org/cmake/help/latest/prop_sf/GENERATED.html

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/misc
        ${PROJECT_SOURCE_DIR}/include/atn
        ${PROJECT_SOURCE_DIR}/include/dfa
        ${PROJECT_SOURCE_DIR}/include/tree
        ${PROJECT_SOURCE_DIR}/include/support
        ${PROJECT_SOURCE_DIR}/include/utf8
        ${PROJECT_SOURCE_DIR}/sparql
)

file(GLOB_RECURSE sparqlTest-SRC
        ${PROJECT_SOURCE_DIR}/test/*.cpp
        ${PROJECT_SOURCE_DIR}/bean/*.cpp
        ${PROJECT_SOURCE_DIR}/sparql/*.cpp
        ${PROJECT_SOURCE_DIR}/parser/*.cpp
)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(sparqlTest main.cpp ${sparqlTest-SRC})
#add_dependencies(graphPage GenerateParser)
target_link_libraries(sparqlTest antlr4)
