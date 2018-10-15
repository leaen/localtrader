#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <string>
#include <chrono>
#include <optional>

#include "client.h"
#include "order.h"

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
        private:
            void match_orders(OrderSide side);
            bool is_matched();

            std::string instrument;

            std::vector<Order*> buy_orders;
            std::vector<Order*> sell_orders;
    };
}

#endif
