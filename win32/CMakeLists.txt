cmake_minimum_required(VERSION 3.20)
project(dtach_win32 C)

# Native Windows (MSVC) build of dtach. Sources stay at the repository root,
# patched under #ifdef _WIN32; the POSIX-on-Win32 layer lives in compat/.
#   cmake -S win32 -B win32/build -A x64 && cmake --build win32/build --config Release

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(NOT MSVC)
    message(FATAL_ERROR "win32/ targets MSVC (cl.exe); on Unix use the upstream Makefile.")
endif()

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(COMMON_FLAGS /W3 /utf-8 /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_DEPRECATE)

# posix_compat: OBJECT library, never STATIC: compat/src/init.c registers a
# CRT initializer that nothing references by name; a .lib would drop it.
file(GLOB COMPAT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/compat/src/*.c)
add_library(posix_compat OBJECT ${COMPAT_SOURCES})
target_compile_options(posix_compat PRIVATE ${COMMON_FLAGS})
target_include_directories(posix_compat PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/compat/include)
target_link_libraries(posix_compat PUBLIC ws2_32 advapi32)

function(posix_port target)
    target_link_libraries(${target} PRIVATE posix_compat)
    target_include_directories(${target} BEFORE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/compat/include)
    target_compile_options(${target} PRIVATE ${COMMON_FLAGS}
        /FI${CMAKE_CURRENT_SOURCE_DIR}/compat/include/compat.h)
    target_sources(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/compat/utf8.manifest)
endfunction()

add_executable(dtach ${SRC}/main.c ${SRC}/master.c ${SRC}/attach.c)
target_include_directories(dtach PRIVATE ${SRC} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
posix_port(dtach)

# Test driver: a scripted ConPTY "terminal" (see tests/run-tests.ps1).
add_executable(ptyrun tests/ptyrun.c)
target_compile_options(ptyrun PRIVATE /W3 /utf-8 /D_CRT_SECURE_NO_WARNINGS)
