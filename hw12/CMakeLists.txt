cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW12)

pico_sdk_init()

add_executable(servo servo.c)

target_sources(servo PRIVATE servo.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(servo pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(servo)