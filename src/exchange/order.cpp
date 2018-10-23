#include <chrono>
#include <iomanip>
#include <sstream>
#include "order.h"
#include "client.h"

namespace exchange {
    Order::Order(const char* instrument, double price, int size, OrderSide side, Client client)
        : instrument(instrument)
        , price(price)
        , size(size)
        , side(side)
        , client(client) {

        filled_amount = 0;
        status = UNFILLED;
        order_time = std::chrono::high_resolution_clock::now();
    }

    bool Order::operator <(const Order& o) const {
        if (side == BUY) {
            if (price > o.price) {
                return true;
            }

            if (price == o.price) {
                if (order_time < o.order_time) {
                    return true;
                } else {
                    return false;
                }
            }

            if (price < o.price) {
                return false;
            }

        } else if (side == SELL){
            if (price < o.price) {
                return true;
            }

            if (price == o.price) {
                if (order_time < o.order_time) {
                    return true;
                } else {
                    return false;
                }
            }

            if (price > o.price) {
                return false;
            }
        }

        // Unreachable
        throw 0;
    }

    bool Order::operator >(const Order& o) const { return (*this < o); }

    OrderStatus Order::get_status() {
        return status;
    }

    void Order::set_status(OrderStatus new_status) {
        status = new_status;
    }

    void Order::cancel() {
        status = CANCELLED;
    }

    int Order::effective_size() {
        return size - filled_amount;
    }

    bool Order::fill(int fill_size) {
        /* Partially fill an order.

           fill_size: the amount of the partial fill

           Precondition: fill_size >= 0 and fill_size cannot be larger
                         than the current effective size of the order.

           Returns true when the order was successfully partially filled
           and false otherwise.
         */

        if (fill_size <= 0) {
            return false;
        }

        if (fill_size > this->effective_size()) {
            return false;
        }

        size -= fill_size;
        if (size == 0) {
            status = FILLED;
        } else {
            status = PARTIALLY_FILLED;
        }

        return true;
    }

    std::string Order::get_instrument() {
        return instrument;
    }

    std::string Order::serialize(const Order& o) {
        std::stringstream ss;
        ss << 'o' << '|';
        ss << o.instrument << '|';
        ss << std::fixed << std::setprecision(4) << o.price << '|';
        ss << std::setprecision(0) << o.size << '|';
        ss << (o.side == BUY ? "BUY" : "SELL") << '|';
        ss << o.get_client().get_name();

        return ss.str();
    }

    std::pair<Order*, bool> Order::deserialize(const std::string& o_serialized) {
        std::stringstream ss;
        ss << o_serialized;

        // Dummy variable to hold pipe separators
        char sep;

        // Check message type
        char message_type;
        ss >> message_type >> sep;

        // The first character in the string should be an 'o' otherwise
        //     this is not an order or is corrupted in some way
        if (message_type != 'o') { return {nullptr, false}; }

        std::string instrument;
        std::getline(ss, instrument, '|');

        double price;
        ss >> price >> sep;

        int size;
        ss >> size >> sep;

        std::string order_side;
        std::getline(ss, order_side, '|');
        OrderSide side;
        if (order_side == "BUY") {
            side = BUY;
        } else if (order_side == "SELL") {
            side = SELL;
        } else {
            // Unknown order side
            return {nullptr, false};
        }

        std::string client_name;
        std::getline(ss, client_name);

        Client* c = new Client(client_name);
        Order* o = new Order(instrument.c_str(), price, size, side, *c);

        return {o, true};
    }
}

