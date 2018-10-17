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

TEST(OrderbookTest, can_get_best_buy) {
    Client bob("bob");
    Order o1("ABC", 100.00, 2, BUY, bob);
    Order o2("ABC", 110.00, 2, BUY, bob);
    Order o3("ABC", 110.00, 2, BUY, bob);
    Orderbook ob("ABC");

    ASSERT_EQ(nullptr, ob.get_best_buy());

    ob.submit_order(o1);
    ob.submit_order(o2);
    ob.submit_order(o3);

    ASSERT_EQ(&o2, ob.get_best_buy());

    Order o4("ABC", 130.00, 2, BUY, bob);
    Order o5("ABC", 120.00, 2, BUY, bob);
    Order o6("ABC", 130.00, 2, BUY, bob);

    ob.submit_order(o4);
    ob.submit_order(o5);
    ob.submit_order(o6);

    ASSERT_EQ(&o4, ob.get_best_buy());
}

TEST(OrderbookTest, can_get_best_sell) {
    Client bob("bob");
    Order o1("ABC", 130.00, 2, SELL, bob);
    Order o2("ABC", 120.00, 2, SELL, bob);
    Order o3("ABC", 120.00, 2, SELL, bob);
    Orderbook ob("ABC");

    ASSERT_EQ(nullptr, ob.get_best_sell());

    ob.submit_order(o1);
    ob.submit_order(o2);
    ob.submit_order(o3);

    ASSERT_EQ(&o2, ob.get_best_sell());

    Order o4("ABC", 100.00, 2, SELL, bob);
    Order o5("ABC", 110.00, 2, SELL, bob);
    Order o6("ABC", 100.00, 2, SELL, bob);

    ob.submit_order(o4);
    ob.submit_order(o5);
    ob.submit_order(o6);

    ASSERT_EQ(&o4, ob.get_best_sell());
}
