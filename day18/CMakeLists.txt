cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day18 C CXX ASM)

pico_sdk_init()

add_executable(day18 day18.cpp)

pico_set_program_name(day18 "day18")
pico_set_program_version(day18 "0.1")

pico_enable_stdio_uart(day18 1)
pico_enable_stdio_usb(day18 0)

target_link_libraries(day18 pico_stdlib)

pico_add_extra_outputs(day18)

