cmake_minimum_required(VERSION 3.14)

project(NTGraph CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MFC_FLAG 2)

add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_AFXDLL -D_MBCS -D_WIN32_WINNT=0x0A00)
   
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEF:${CMAKE_CURRENT_SOURCE_DIR}/src/NTGraph.def")

file(GLOB_RECURSE SRC
    "src/*.cpp"
    "src/*.h"
    "src/*.hpp"
    "src/*.odl"
)

file(GLOB_RECURSE RES
    "src/res/*"
    "src/*.rc"
)

add_library(${CMAKE_PROJECT_NAME} MODULE ${SRC})
target_sources(${PROJECT_NAME} PRIVATE ${RES})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".ocx")                        
set_source_files_properties(src/NTGraph.rc PROPERTIES LANGUAGE RC)
source_group("Resource Files" FILES ${RES})

set(MIDL_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NTGraph.tlb)
set(MIDL_FILE ${CMAKE_CURRENT_LIST_DIR}/src/NTGraph.odl)
add_custom_command(
    OUTPUT ${MIDL_OUTPUT}
    COMMAND midl ${MIDL_FILE} /tlb ${MIDL_OUTPUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${MIDL_FILE}
    MAIN_DEPENDENCY ${MIDL_FILE}
    VERBATIM
)

target_sources(${PROJECT_NAME} PRIVATE ${MIDL_FILE})


