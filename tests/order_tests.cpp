#include "gtest/gtest.h"
#include "order.h"

using namespace exchange;

TEST(OrderTest, unfilled_by_default) {
    Client bob("bob");
    Order o("ABC", 50.00, 2, BUY, bob);
    ASSERT_EQ(UNFILLED, o.get_status());
}

TEST(OrderTest, can_change_order_status) {
    Client bob("bob");
    Order o("ABC", 50.00, 2, BUY, bob);

    o.set_status(PARTIALLY_FILLED);
    ASSERT_EQ(PARTIALLY_FILLED, o.get_status());

    o.set_status(CANCELLED);
    ASSERT_EQ(CANCELLED, o.get_status());
}

TEST(OrderTest, price_order_respected) {
    Client bob("bob");
    Order buy_low("ABC", 50.00, 2, BUY, bob);
    Order buy_high("ABC", 55.00, 2, BUY, bob);

    // Orders to buy with higher prices are more aggresive than orders
    //   with lower prices and should sort larger as a result.
    ASSERT_TRUE(buy_low < buy_high);

    Order sell_low("ABC", 50.00, 2, SELL, bob);
    Order sell_high("ABC", 55.00, 2, SELL, bob);

    // Orders to sell with lower prices are more aggresive than orders
    //   with higher prices and should sort larger as a result.
    ASSERT_TRUE(sell_high < sell_low);
}

TEST(OrderTest, time_order_respected) {
    Client bob("bob");
    Order buy_early("ABC", 50.00, 2, BUY, bob);
    Order buy_later("ABC", 50.00, 2, BUY, bob);

    // Early orders are more aggresive than late orders and should sort
    //   larger as a result.
    ASSERT_TRUE(buy_later < buy_early);

    Order sell_early("ABC", 50.00, 2, SELL, bob);
    Order sell_later("ABC", 50.00, 2, SELL, bob);

    ASSERT_TRUE(sell_later < sell_early);
}

TEST(OrderTest, can_cancel_order) {
    Client bob("bob");
    Order o("ABC", 100.00, 2, BUY, bob);
    o.cancel();

    ASSERT_EQ(CANCELLED, o.get_status());
}

TEST(OrderTest, can_change_filled_amount) {
    Client bob("bob");
    Order o("ABC", 100.00, 10, BUY, bob);

    ASSERT_EQ(10, o.effective_size());

    ASSERT_TRUE(o.fill(5));
    ASSERT_EQ(5, o.effective_size());

    ASSERT_TRUE(o.fill(2));
    ASSERT_EQ(3, o.effective_size());

    ASSERT_TRUE(o.fill(3));
    ASSERT_EQ(0, o.effective_size());
}

TEST(OrderTest, cant_fill_past_order_size) {
    Client bob("bob");
    Order o("ABC", 100.00, 10, BUY, bob);

    ASSERT_FALSE(o.fill(11));
}

TEST(OrderTest, cant_fill_negative_amount) {
    Client bob("bob");
    Order o("ABC", 100.00, 10, BUY, bob);

    ASSERT_FALSE(o.fill(-2));
}

