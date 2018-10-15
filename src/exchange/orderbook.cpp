#include "orderbook.h"
#include <iostream>

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
}

