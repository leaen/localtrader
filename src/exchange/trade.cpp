#include <chrono>
#include <sstream>
#include <iomanip>
#include "trade.h"

namespace exchange {
    Trade::Trade(std::string instrument, double price, int size, OrderSide side,
          Client maker, Client taker)
        : instrument(instrument)
        , price(price)
        , size(size)
        , side(side)
        , maker(maker)
        , taker(taker) {
            trade_time = std::chrono::high_resolution_clock::now();
    }

    long Trade::get_trade_time_ms() const {
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(get_trade_time());
        auto epoch = now_ms.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
        long trade_time_ms = value.count();

        return trade_time_ms;
    }

    std::string Trade::serialize(const Trade &t) {
        std::stringstream ss;
        ss << 't' << '|';
        ss << t.instrument << '|';
        ss << std::fixed << std::setprecision(4) << t.price << '|';
        ss << std::setprecision(0) << t.size << '|';
        ss << (t.side == BUY ? "BUY" : "SELL") << '|';
        ss << t.get_maker().get_name() << '|';
        ss << t.get_taker().get_name() << '|';
        ss << t.get_trade_time_ms();

        return ss.str();
    }
}
