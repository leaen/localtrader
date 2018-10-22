#include <chrono>
#include "gtest/gtest.h"
#include "trade.h"

using namespace exchange;

TEST(TradeTest, can_serialize_trades) {
    Client bob("bob");
    Client alice("alice");

    // Create first timestamp
    long t1_ts_ms = 1540176957288;
    std::chrono::milliseconds dur_t1(t1_ts_ms);
    std::chrono::time_point<std::chrono::high_resolution_clock> t1_ts(dur_t1);

    Trade t1(std::string("ABC"), 100.00, 50, BUY, bob, alice);
    t1.set_trade_time(t1_ts);

    std::string t1_serialized = Trade::serialize(t1);

    // Check first trade
    std::string expected1 = "t|ABC|100.0000|50|BUY|bob|alice|1540176957288";
    ASSERT_STREQ(expected1.c_str(), t1_serialized.c_str());

    // Create second timestamp
    long t2_ts_ms = 1540176957288;
    std::chrono::milliseconds dur_t2(t2_ts_ms);
    std::chrono::time_point<std::chrono::high_resolution_clock> t2_ts(dur_t2);

    Trade t2(std::string("CBA"), 12.333, 432, SELL, alice, bob);
    t2.set_trade_time(t2_ts);
    std::string t2_serialized = Trade::serialize(t2);

    // Check second trade
    std::string expected2 = "t|CBA|12.3330|432|SELL|alice|bob|1540176957288";
    ASSERT_STREQ(expected2.c_str(), t2_serialized.c_str());
}

