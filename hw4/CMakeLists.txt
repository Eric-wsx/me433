cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW4)

pico_sdk_init()

add_executable(DAC)

target_sources(DAC PRIVATE DAC.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(DAC pico_stdlib hardware_spi)
pico_enable_stdio_usb (DAC 1)
pico_enable_stdio_uart (DAC 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(DAC)