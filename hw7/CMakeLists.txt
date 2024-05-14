cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW7)

pico_sdk_init()

add_executable(uart uart.c ssd1306.c)

target_sources(uart PRIVATE uart.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(uart pico_stdlib hardware_i2c hardware_uart)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart)