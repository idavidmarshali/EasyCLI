cmake_minimum_required(VERSION 3.22)
project(EasyCLI C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-O3 -Wall -Werror -Wpedantic -Wextra)
message("EasyCLI by DavidMarshal (https://github.com/idavidmarshali)")
include_directories(include)
add_library(EasyCLIShared SHARED src/EasyCLI.c)
add_library(EasyCLIStatic STATIC src/EasyCLI.c)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(EasyCLITest test/test.c)
    target_link_libraries(EasyCLITest EasyCLIStatic)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")