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

            std::string get_instrument() { return instrument; }
            double get_price() { return price; }
            int get_size() { return size; }
            OrderSide get_side() { return side; }
            Client get_maker() const { return maker; }
            Client get_taker() const { return taker; }

            Timestamp get_trade_time() const { return trade_time; }
            void set_trade_time(Timestamp new_time) { trade_time = new_time; }
            long get_trade_time_ms() const;

            static std::string serialize(const Trade &t);

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
