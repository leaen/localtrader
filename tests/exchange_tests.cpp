#include "gtest/gtest.h"
#include "exchange.h"

TEST(ExchangeTest, statusIsReady) {
    exchange::Exchange e;
    ASSERT_STREQ("Ready.", e.get_status().c_str());
}
