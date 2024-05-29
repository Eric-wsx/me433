cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW16)

pico_sdk_init()

add_executable(MOTOR_control MOTOR_control.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(MOTOR_control pico_stdlib hardware_uart)

# create map/bin/hex file etc.
pico_add_extra_outputs(MOTOR_control)212