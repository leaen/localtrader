project(exchange)

add_library(exchange STATIC exchange.cpp exchange.h)
target_include_directories(exchange PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
