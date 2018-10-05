#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "client.h"

namespace exchange {
    enum OrderStatus {
        ACTIVE,
        FILLED,
        PARTIALLY_FILLED,
        CANCELLED
    };

    enum OrderSide {
        BUY,
        SELL
    };

    class Order {
        public:
            Order(std::string instrument, double price, int size, OrderSide side, Client client);
            bool operator <(const Order& o);
            void set_status(OrderStatus s);
            OrderStatus get_status();
            void cancel();
            void fill(int fill_size);
            static std::string serialize(const Order& o);
            static Order deserialize(std::string serialized_o);
        friend:
            ostream &operator<<(ostream &output, const Order& o);
    };
}

#endif
