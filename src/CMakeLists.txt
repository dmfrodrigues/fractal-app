cmake_minimum_required(VERSION 3.10)

project(fractalapp)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

find_package(wxWidgets COMPONENTS core base REQUIRED)

include_directories(../include)
include(${wxWidgets_USE_FILE})

add_library(fractalapp STATIC CURSOR_CROSS.cpp FractalBitmap.cpp FractalFrame.cpp FractalPanel.cpp HDPrintscreenDialog.cpp InfoPanel.cpp)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(STATUS "GCC detected, adding compile flags")
    set(CPP_COMPILER_OPTIMIZE -ffast-math -O3)
    set(CPP_COMPILER_WARNINGS -Wall -Wunused-result -pedantic-errors -Wextra -Wcast-align -Wcast-qual -Wchar-subscripts
                          -Wcomment -Wconversion -Wdisabled-optimization -Wfloat-equal  -Wformat  -Wformat=2
                          -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wimport -Winit-self -Winvalid-pch
                          -Wmissing-braces -Wmissing-field-initializers -Wmissing-format-attribute -Wmissing-include-dirs
                          -Wmissing-noreturn -Wpacked -Wparentheses  -Wpointer-arith -Wredundant-decls -Wreturn-type
                          -Wsequence-point  -Wshadow -Wsign-compare  -Wstack-protector -Wstrict-aliasing -Wstrict-aliasing=2
                          -Wswitch  -Wswitch-default -Wswitch-enum -Wtrigraphs  -Wuninitialized -Wno-unknown-pragmas 
                          -Wunreachable-code -Wunused -Wunused-function  -Wunused-label  -Wunused-parameter
                          -Wunused-value  -Wunused-variable  -Wvariadic-macros -Wvolatile-register-var  -Wwrite-strings)
    set(CPP_COMPILER_OPTIONS )
    set(CPP_LINKER_OPTIONS )
    if(WIN32)
        message(STATUS "GCC under Microsoft Windows")
        if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
            message(STATUS "Release mode: no console")
            set(CPP_COMPILER_OPTIONS ${CPP_COMPILER_OPTIONS} -mwindows)
            set(CPP_LINKER_OPTIONS ${CPP_LINKER_OPTIONS} -mwindows)
        endif()
    endif()
    target_compile_options(fractalapp PRIVATE -g ${CPP_COMPILER_WARNINGS} ${CPP_COMPILER_OPTIMIZE} ${CPP_COMPILER_OPTIONS})
    target_link_options(fractalapp PRIVATE ${CPP_LINKER_OPTIONS})
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    message(STATUS "MSVC detected, adding compile flags")
    set(CPP_COMPILER_OPTIMIZE /O2 /GL)
    set(CPP_LINKER_OPTIONS /SUBSYSTEM:WINDOWS)
    set(CPP_LINKER_OPTIMIZE /LTCG)
    set(CPP_COMPILER_WARNINGS /W4 /wd4068)
    target_compile_options(fractalapp PRIVATE ${CPP_COMPILER_WARNINGS} ${CPP_COMPILER_OPTIMIZE})
    target_link_options(fractalapp PRIVATE ${CPP_LINKER_OPTIMIZE} ${CPP_LINKER_OPTIONS})
endif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
