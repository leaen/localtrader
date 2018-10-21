#include <chrono>
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
}
