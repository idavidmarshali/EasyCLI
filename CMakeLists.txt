cmake_minimum_required(VERSION 3.22)
project(EasyCLI C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-O3 -Wall -Werror -Wpedantic -Wextra)
message("EasyCLI by DavidMarshal (https://github.com/idavidmarshali)")
include_directories(include)
add_library(EasyCLIShared SHARED src/EasyCLI.c)
add_library(EasyCLIStatic STATIC src/EasyCLI.c)
