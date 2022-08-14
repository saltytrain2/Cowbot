cmake_minimum_required(VERSION 3.14)
project(chess-engine LANGUAGES CXX)

option(BUILD_GUI "build the gui that comes with this program" ON)

# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MY_COMPILE_FLAGS -Wall -Wextra -Wpedantic -pedantic 
    $<$<CXX_COMPILER_ID:Clang,GNU>:-flto> # enable LTO if Clang or GCC
    $<$<CONFIG:Debug>:-fsanitize=address> # enable sanitizers if in debug-mode
)
set(MY_LINK_FLAGS 
    $<$<CXX_COMPILER_ID:Clang,GNU>:-flto> # enable LTO if Clang or GCC
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,10.0.1>>:-fuse-ld=lld> # clang linker specification available starting 10.0.1
    $<$<CONFIG:Debug>:-fsanitize=address>
)

enable_testing()
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(gui)
