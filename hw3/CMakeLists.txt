cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(HW3)# project folder name

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

#Created a target named “print”
add_executable(cdc
    cdc.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(cdc pico_stdlib hardware_adc)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(cdc)

pico_enable_stdio_usb(cdc 1) #same target with add_executable 
pico_enable_stdio_uart(cdc 0)