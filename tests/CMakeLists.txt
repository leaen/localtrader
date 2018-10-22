project(localtrader_tests)

SET(TEST_FILES exchange_tests.cpp client_tests.cpp order_tests.cpp orderbook_tests.cpp trade_tests.cpp)
SET(TEST_LIBRARIES exchange)

# Tests executable
add_executable(tests tests.cpp ${TEST_FILES})
target_link_libraries(tests PRIVATE exchange)
target_link_libraries(tests PRIVATE gtest ${TEST_LIBRARIES})

