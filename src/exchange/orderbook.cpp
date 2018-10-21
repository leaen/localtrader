#include <algorithm>
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

        match_orders(o.get_side());

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
        return get_best_bid() >= get_best_offer();
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

    void Orderbook::match_orders(OrderSide side) {
        std::vector<Trade*> new_trades;
        while (is_matched()) {
            Order* bb = get_best_buy();
            Order* bs = get_best_sell();

            int trade_size = std::min(bb->effective_size(), bs->effective_size());

            // Price occurs at the maker order price, so if the new order
            //     was a buy, then the trade occurs at the price of the sell order
            //     and vice versa if the taker is a sell order.
            double trade_price = (side == BUY) ? bs->get_price() : bb->get_price();

            // Maker is the order on the book and taker is the client of the new order.
            Client maker = (side == BUY) ? bs->get_client() : bb->get_client();
            Client taker = (side == BUY) ? bb->get_client() : bs->get_client();

            Trade new_t = Trade(instrument, trade_price, trade_size, side,
                                maker, taker);

            // Register the fill on each order
            bb->fill(trade_size);
            bs->fill(trade_size);

            // If orders are filled remove them from the book
            if (bb->get_status() == FILLED) {
                buy_orders.erase(
                    std::find(buy_orders.begin(), buy_orders.end(), bb), buy_orders.end()
                );
            }

            if (bs->get_status() == FILLED) {
                sell_orders.erase(
                    std::find(sell_orders.begin(), sell_orders.end(), bs), sell_orders.end()
                );
            }

            // Record the new trade
            trades.push_back(&new_t);
        }
    }
}

