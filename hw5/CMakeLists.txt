cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW4)

pico_sdk_init()

add_executable(I2C)

target_sources(I2C PRIVATE I2C.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(I2C pico_stdlib hardware_i2c)
pico_enable_stdio_usb (I2C 1)
pico_enable_stdio_uart (I2C 0)
# create map/bin/hex file etc.
pico_add_extra_outputs(I2C)