cmake_minimum_required(VERSION 3.14)

project(NTGraph CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MFC_FLAG 2)

add_definitions(-D_AFXDLL -D_MBCS -D_WIN32_WINNT=0x0A00)
   
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEF:../src/NTGraph.def")

add_library(${CMAKE_PROJECT_NAME} SHARED
                                  src/AnnoPpg.cpp
                                  src/AnnoPpg.hpp
                                  src/ColourPicker.cpp
                                  src/ColourPicker.hpp
                                  src/ColourPopup.cpp
                                  src/ColourPopup.hpp
                                  src/CursorPpg.cpp
                                  src/CursorPpg.hpp
                                  src/ElementPpg.cpp
                                  src/ElementPpg.hpp
                                  src/FormatPpg.cpp
                                  src/FormatPpg.hpp
                                  src/NTGraph.cpp
                                  src/NTGraph.hpp
                                  src/NTGraph.rc
                                  src/NTGraphCtl.cpp
                                  src/NTGraphCtl.hpp
                                  src/NTGraphPpg.cpp
                                  src/NTGraphPpg.hpp
                                  src/resource.h
                                  src/StdAfx.cpp
                                  src/StdAfx.h)

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


