CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

set(CMAKE_C_COMPILER "gcc")
set(PROJECT_NAME pairwise-msd)

# Status information
message("Using: ${CMAKE_C_COMPILER} with compiler ID ${CMAKE_C_COMPILER_ID} and compiler version ${CMAKE_C_COMPILER_VERSION}")

file(GLOB_RECURSE SOURCE_FILES "src/*.c")

include_directories(include/)

# Generic flags for GCC compiler
list(APPEND CMAKE_C_FLAGS "-std=c99 -O3 -g -Wall -Wpedantic -Wextra -fopenmp -ffast-math -lm")

#Generic flags for ICC compiler
#list(APPEND CMAKE_C_FLAGS "-qopenmp -g -fp-model fast=2 -std=c99 -O2 -fma -align -finline-functions -no-prec-div -ipo -DOPEMP_SPEC -qopt-streaming-stores=never")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
