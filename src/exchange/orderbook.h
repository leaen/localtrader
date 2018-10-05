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
            Orderbook(std::string instrument);
            void submit_order(Order& o);
            std::optional<double> get_best_bid();
            std::optional<double> get_best_offer();

        private:
            void match_orders(OrderSide side);
            bool is_matched();
    };
}

#endif
