project(localtrader_prog)

add_subdirectory(exchange)

# Exchange server executable
add_executable(server server.cpp)
target_link_libraries(server PRIVATE exchange)
