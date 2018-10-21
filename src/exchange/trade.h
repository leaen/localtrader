#ifndef TRADE_H
#define TRADE_H

#include <string>
#include <chrono>

#include "client.h"
#include "order.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

namespace exchange {
    class Trade {
        public:
            Trade(std::string instrument, double price, int size, OrderSide side,
                  Client maker, Client taker);

            int get_size() { return size; }
            double get_price() { return price; }

        private:
            std::string instrument;
            double price;
            int size;
            OrderSide side;
            Client maker;
            Client taker;
            Timestamp trade_time;
    };
}

#endif
