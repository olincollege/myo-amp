# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wpedantic -Werror")


# Add executable. Default name is the project name, version 0.1

add_library(TB6612FNG components/TB6612FNG/TB6612FNG.cpp)
add_library(context components/context/context.cpp)
add_executable(main main.cpp)

pico_set_program_name(main "main")
pico_set_program_version(main "0.3")

# no_flash means the target is to run from RAM
pico_set_binary_type(main no_flash)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

# Add the standard library to the build
target_link_libraries(TB6612FNG
    hardware_pwm
    hardware_gpio
    pico_stdlib
)

target_include_directories(context PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/components
        ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
)

target_link_libraries(context
    TB6612FNG
    pico_stdlib
    hardware_adc
)

target_include_directories(context PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/components
        ${CMAKE_CURRENT_LIST_DIR}/components/context
        ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
)

target_link_libraries(main
        TB6612FNG
        context
        pico_stdlib
        pico_multicore
)

# Add the standard include files to the build
target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/components
        ${CMAKE_CURRENT_LIST_DIR}/components/context
        ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
)

pico_add_extra_outputs(main)

