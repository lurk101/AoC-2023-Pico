cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(day07 C CXX ASM)

pico_sdk_init()

add_executable(day07 day07.cpp)

pico_set_program_name(day07 "day07")
pico_set_program_version(day07 "0.1")

pico_enable_stdio_uart(day07 1)
pico_enable_stdio_usb(day07 0)

target_link_libraries(day07 pico_stdlib)

pico_add_extra_outputs(day07)

