cmake_minimum_required(VERSION 3.20)

project(aoc2025 VERSION 1.0.0 LANGUAGES CXX)

# Enable compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set debug build as default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Add common compiler flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-vla-cxx-extension")

# Add compiler flags for debug builds
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Collect source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Check if we have sources
if(NOT SOURCES)
    message(FATAL_ERROR "No .cpp source files found in ${CMAKE_SOURCE_DIR}")
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compile commands: ${CMAKE_EXPORT_COMPILE_COMMANDS}")
