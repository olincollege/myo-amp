# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
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

set(PICO_DEOPTIMIZED_DEBUG 1)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Add executable. Default name is the project name, version 0.1

add_library(TB6612FNG components/TB6612FNG/TB6612FNG.cpp)
add_library(DRV8871 components/DRV8871/DRV8871.cpp)
add_library(context components/context/context.cpp)
add_executable(main main.cpp)

# Set C++ standard for all targets
set_target_properties(TB6612FNG PROPERTIES CXX_STANDARD 17)
set_target_properties(DRV8871 PROPERTIES CXX_STANDARD 17)
set_target_properties(context PROPERTIES CXX_STANDARD 17)
set_target_properties(main PROPERTIES CXX_STANDARD 17)

pico_set_program_name(main "main")
pico_set_program_version(main "0.3")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(main 0)
pico_enable_stdio_usb(main 1)

# Add the standard library to the build

target_include_directories(TB6612FNG PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/components
    ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
)

target_link_libraries(TB6612FNG
    hardware_pwm
    hardware_gpio
    pico_stdlib
)

target_include_directories(DRV8871 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/components
    ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
    ${CMAKE_CURRENT_LIST_DIR}/components/DRV8871
)

target_linK_libraries(DRV8871
    hardware_pwm
    hardware_gpio
    pico_stdlib
    TB6612FNG
)

target_include_directories(context PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/components
    ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
    ${CMAKE_CURRENT_LIST_DIR}/components/DRV8871
)

target_link_libraries(context
    TB6612FNG
    DRV8871
    pico_stdlib
    hardware_adc
)

target_link_options(context PRIVATE
    -specs=sync-dmb.specs
)


target_link_libraries(main
    TB6612FNG
    context
    pico_stdlib
    pico_multicore
    hardware_gpio
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/components
    ${CMAKE_CURRENT_LIST_DIR}/components/context
    ${CMAKE_CURRENT_LIST_DIR}/components/TB6612FNG
)

target_link_options(main PRIVATE
    -specs=sync-dmb.specs
)

pico_add_extra_outputs(main)

