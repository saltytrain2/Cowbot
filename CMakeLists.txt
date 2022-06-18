cmake_minimum_required(VERSION 3.14)
project(chess-engine LANGUAGES CXX)

# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MY_COMPILE_FLAGS -Wall -Wextra -Wpedantic $<$<CXX_COMPILER_ID:Clang>:-flto> $<$<CONFIG:Debug>:-fsanitize=address>)
set(MY_LINK_FLAGS $<$<CXX_COMPILER_ID:Clang>:-flto> $<$<CONFIG:Debug>:-fsanitize=address>)

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)