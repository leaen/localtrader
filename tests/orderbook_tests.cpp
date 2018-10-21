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

TEST(OrderbookTest, trades_occur) {
    Client bob("bob");
    Client alice("alice");
    Order o1("ABC", 100.00, 10, BUY, bob);
    Order o2("ABC", 100.00, 5, SELL, alice);
    Orderbook ob("ABC");

    // Initially there should be no trades recorded
    ASSERT_EQ(0, ob.get_trades()->size());

    ob.submit_order(o1);
    ob.submit_order(o2);

    // One trade of 5 units should have occurred at a price of 100.00
    //     upon submitting the sell order
    ASSERT_EQ(1, ob.get_trades()->size());
    ASSERT_EQ(5, (*ob.get_trades())[0]->get_size());
    ASSERT_EQ(100.00, (*ob.get_trades())[0]->get_price());

    // The buy should have 5 units left and the sell should be filled
    ASSERT_EQ(PARTIALLY_FILLED, o1.get_status());
    ASSERT_EQ(5, o1.effective_size());
    ASSERT_EQ(FILLED, o2.get_status());

    // Submitting another sell order should trigger another trade of size 5
    //     since the buy order was for 10 units
    Order o3("ABC", 100.00, 5, SELL, alice);
    ob.submit_order(o3);

    // This should be counted as a second separate trade
    ASSERT_EQ(2, ob.get_trades()->size());
    ASSERT_EQ(5, (*ob.get_trades())[1]->get_size());
    ASSERT_EQ(100.0, (*ob.get_trades())[1]->get_price());

    // Since the buy is fully matched, another sell should not trigger a trade
    Order o4("ABC", 100.00, 5, SELL, alice);
    ob.submit_order(o4);

    // No new trades should occur
    ASSERT_EQ(2, ob.get_trades()->size());
}

TEST(OrderbookTest, cant_match_cancelled_order) {
    Client bob("bob");
    Client alice("alice");
    Order o1("ABC", 100.00, 10, BUY, bob);
    Order o2("ABC", 100.00, 5, SELL, alice);
    Orderbook ob("ABC");

    // Submit then cancel the buy order
    ob.submit_order(o1);
    o1.cancel();

    // Submit the sell order
    ob.submit_order(o2);

    // No trades should occur
    ASSERT_EQ(0, ob.get_trades()->size());
}

TEST(OrderbookTest, cant_match_filled_orders) {
    Client bob("bob");
    Client alice("alice");
    Order o1("ABC", 100.00, 5, BUY, bob);
    Order o2("ABC", 100.00, 5, SELL, alice);
    Order o3("ABC", 100.00, 5, SELL, alice);
    Orderbook ob("ABC");

    ob.submit_order(o1);
    ob.submit_order(o2);

    // Buy and first sell should have matched
    ASSERT_EQ(1, ob.get_trades()->size());
    ASSERT_EQ(FILLED, o1.get_status());
    ASSERT_EQ(FILLED, o2.get_status());

    ob.submit_order(o3);

    // No new trades should occur since the buy order
    // was fully filled
    ASSERT_EQ(1, ob.get_trades()->size());
    ASSERT_EQ(UNFILLED, o3.get_status());
}
