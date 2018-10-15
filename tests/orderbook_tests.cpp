#include "gtest/gtest.h"
#include "orderbook.h"

using namespace exchange;

TEST(OrderbookTest, can_submit_order) {
    Client bob("bob");
    Order o("ABC", 50.00, 2, BUY, bob);
    Orderbook ob("ABC");

    bool success = ob.submit_order(o);
    ASSERT_TRUE(success);
}

TEST(OrderbookTest, cant_submit_order_to_wrong_orderbook) {
    Client bob("bob");
    Order o("ABC", 50.00, 2, BUY, bob);
    Orderbook ob("CBA");

    bool success = ob.submit_order(o);
    ASSERT_FALSE(success);
}

TEST(OrderbookTest, can_see_best_bid) {
    Client bob("bob");
    Order o1("ABC", 100.00, 2, BUY, bob);
    Order o2("ABC", 110.00, 2, BUY, bob);
    Orderbook ob("ABC");

    ob.submit_order(o1);
    ASSERT_EQ(100.00, ob.get_best_bid());

    ob.submit_order(o2);
    ASSERT_EQ(110.00, ob.get_best_bid());

    o2.cancel();
    ASSERT_EQ(100.00, ob.get_best_bid());
}

TEST(OrderbookTest, can_see_best_offer) {
    Client bob("bob");
    Order o1("ABC", 110.00, 2, SELL, bob);
    Order o2("ABC", 100.00, 2, SELL, bob);
    Orderbook ob("ABC");

    ob.submit_order(o1);
    ASSERT_EQ(110.00, ob.get_best_offer());

    ob.submit_order(o2);
    ASSERT_EQ(100.00, ob.get_best_offer());

    o2.cancel();
    ASSERT_EQ(110.00, ob.get_best_offer());
}

