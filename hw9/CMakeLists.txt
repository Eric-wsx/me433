cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW9)

pico_sdk_init()

add_executable(pico_to_zero pico_to_zero.c)

# target_sources(pico_to_zero PRIVATE pico_to_zero.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(pico_to_zero pico_stdlib hardware_uart)

pico_enable_stdio_usb(pico_to_zero 1)
pico_enable_stdio_uart(pico_to_zero 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_to_zero)