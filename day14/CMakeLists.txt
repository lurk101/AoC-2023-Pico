cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day14 C CXX ASM)

pico_sdk_init()

add_executable(day14 day14.cpp)

pico_set_program_name(day14 "day14")
pico_set_program_version(day14 "0.1")

pico_enable_stdio_uart(day14 1)
pico_enable_stdio_usb(day14 0)

target_link_libraries(day14 pico_stdlib)

pico_add_extra_outputs(day14)

