cmake_minimum_required(VERSION 3.13)

# set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

include(pico_sdk_import.cmake)

project(display_led C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
pico_sdk_init()

add_executable(display_led
    src/main.cpp
    src/pin.cpp
    src/shift.cpp
)

pico_enable_stdio_usb(display_led 1)
pico_enable_stdio_uart(display_led 1)

pico_add_extra_outputs(display_led)

target_link_libraries(display_led pico_stdlib)
