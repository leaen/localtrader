#include <chrono>
#include "order.h"

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

    bool Order::operator <(const Order& o) {
        if (side == BUY) {
            if (price < o.price) {
                return true;
            }

            if (price == o.price) {
                if (order_time < o.order_time) {
                    return false;
                } else {
                    return true;
                }
            }

            if (price > o.price) {
                return false;
            }

        } else {
            if (price > o.price) {
                return true;
            }

            if (price == o.price) {
                if (order_time < o.order_time) {
                    return false;
                } else {
                    return true;
                }
            }

            if (price < o.price) {
                return false;
            }
        }

        // Unreachable
        throw 0;
    }

    bool Order::operator >(const Order& o) { return (*this < o); };

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
        return true;
    }
};
