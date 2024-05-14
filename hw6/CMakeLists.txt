cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(HW6)

pico_sdk_init()

add_executable(oled 
    oled.c
    ssd1306.c
)

target_sources(oled PRIVATE oled.c)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(oled pico_stdlib hardware_i2c hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(oled)