cmake_minimum_required(VERSION 3.12)

# The project name
project(localtrader)

# Compiler warning flags
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Set the compiler
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")

add_subdirectory(thirdparty/googletest)
add_subdirectory(src)
add_subdirectory(tests)
