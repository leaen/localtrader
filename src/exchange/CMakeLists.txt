project(exchange)

set(EXCHANGE_HEADERS exchange.h client.h order.h)
set(EXCHANGE_SOURCE_FILES exchange.cpp client.cpp order.cpp)

add_library(exchange STATIC ${EXCHANGE_HEADERS} ${EXCHANGE_SOURCE_FILES})
target_include_directories(exchange PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
