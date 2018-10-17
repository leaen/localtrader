#include <iostream>

#include "orderbook.h"

namespace exchange {
    std::string Orderbook::get_instrument() {
        return instrument;
    }

    bool Orderbook::submit_order(Order& o) {
        if (o.get_instrument() != instrument) {
            std::cerr << "Order rejected for instrument mismatch with Orderbook.\n";
            return false;
        }

        if (o.is_buy()) {
            buy_orders.push_back(&o);
        } else {
            sell_orders.push_back(&o);
        }

        return true;
    }

    double Orderbook::get_best_bid() {
        double bb = 0.0;

        for (auto o : buy_orders) {
            if (o->is_cancelled()) { continue; }
            bb = std::max(bb, o->get_price());
        }

        return bb;
    }

    double Orderbook::get_best_offer() {
        double bo = std::numeric_limits<double>::max();

        for (auto o : sell_orders) {
            if (o->is_cancelled()) { continue; }
            bo = std::min(bo, o->get_price());
        }

        return bo;
    }

    bool Orderbook::is_matched() {
        /*
         * Returns true when there are matched orders in the book.
         */
        return get_best_bid() > get_best_offer();
    }

    Order* Orderbook::get_best_buy() {
        /*
         * Returns a pointer to the most aggressive buy order in the book.
         */

        Order* best_buy = nullptr;
        for (auto o : buy_orders) {
            if (best_buy == nullptr) {
                best_buy = o;
                continue;
            }

            if (*o < *best_buy) {
                best_buy = o;
            }
        }

        return best_buy;
    }

    Order* Orderbook::get_best_sell() {
        /*
         * Returns a pointer to the most aggressive sell order in the book.
         */

        Order* best_sell = nullptr;
        for (auto o : sell_orders) {
            if (best_sell == nullptr) {
                best_sell = o;
                continue;
            }

            if (*o < *best_sell) {
                best_sell = o;
            }
        }

        return best_sell;
    }
}

