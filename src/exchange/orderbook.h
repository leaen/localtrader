#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <string>
#include <chrono>
#include <vector>

#include "client.h"
#include "order.h"
#include "trade.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

namespace exchange {
    class Orderbook {
        public:
            Orderbook(std::string instrument) : instrument(instrument) {}
            Orderbook(const char* instrument) : instrument(instrument) {}

            std::string get_instrument();

            bool submit_order(Order& o);

            double get_best_bid();
            double get_best_offer();

            Order* get_best_buy();
            Order* get_best_sell();

            std::vector<Trade*>* get_trades() { return &trades; }

            void set_trade_announcements(bool flag) { trade_announcements = flag; }
        private:
            void match_orders(OrderSide side);
            bool is_matched();

            std::string instrument;

            std::vector<Order*> buy_orders;
            std::vector<Order*> sell_orders;

            std::vector<Trade*> trades;

            bool trade_announcements = false;
    };
}

#endif
