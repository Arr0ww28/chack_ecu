cmake_minimum_required(VERSION 3.10)

project(ecu_sim C)

# Use C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Add compiler warnings
add_compile_options(-Wall -Wextra -pedantic)

# Include directory
include_directories(include)

# Source files
add_executable(ecu_sim
    src/main.c
    src/input.c
    src/mode.c
    src/control.c
    src/fault.c
    src/state.c
    src/log.c
)
