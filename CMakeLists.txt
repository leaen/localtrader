cmake_minimum_required(VERSION 3.12)

# The project name
project(localtrader)

# Compiler warning flags
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Boost and pthread
set(CMAKE_EXE_LINKER_FLAGS "-lboost_system -lpthread")

# Set the standard to C++14
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(thirdparty/googletest)
add_subdirectory(thirdparty/websocketpp)
add_subdirectory(src)
add_subdirectory(tests)
