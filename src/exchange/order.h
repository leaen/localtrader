#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>
#include "client.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;

namespace exchange {
    enum OrderStatus {
        UNFILLED,
        PARTIALLY_FILLED,
        FILLED,
        CANCELLED
    };

    enum OrderSide {
        BUY,
        SELL
    };

    class Order {
        public:
            Order(const char* instrument, double price, int size, OrderSide side, Client client);

            std::string get_instrument();

            bool operator <(const Order& o);
            bool operator >(const Order& o);

            void set_status(OrderStatus new_status);
            OrderStatus get_status();
            void cancel();
            bool is_cancelled() { return status == CANCELLED; }

            int effective_size();
            bool fill(int fill_size);

            bool is_buy() { return side == BUY; }
            bool is_sell() { return side == SELL; }

            double get_price() { return price; }
        private:
            std::string instrument;
            double price;

            int size;
            int filled_amount;

            OrderSide side;
            OrderStatus status;

            Client client;

            Timestamp order_time;
    };
}

#endif
