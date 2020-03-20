cmake_minimum_required(VERSION 3.14)

project(NTGraph CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MFC_FLAG 2)

add_definitions(-D_AFXDLL -D_MBCS -D_WIN32_WINNT=0x0A00)
   
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEF:../src/NTGraph.def")

add_library(${CMAKE_PROJECT_NAME} SHARED
                                  src/AnnoPpg.cpp
                                  src/ColourPicker.cpp
                                  src/ColourPopup.cpp
                                  src/CursorPpg.cpp
                                  src/ElementPpg.cpp
                                  src/FormatPpg.cpp
                                  src/NTGraph.cpp
                                  src/NTGraphCtl.cpp
                                  src/NTGraphPpg.cpp
                                  src/StdAfx.cpp
                                  src/NTGraph.rc)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".ocx")                        
set_source_files_properties(src/NTGraph.rc PROPERTIES LANGUAGE RC)

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

target_sources(${PROJECT_NAME} PRIVATE ${MIDL_FILE} ${MIDL_OUTPUT})
