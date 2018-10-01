#include <iostream>
#include "exchange.h"

int main() {
    std::cout << "Hello, world!" << std::endl;

    exchange::Exchange e;
    std::cout << e.get_status() << std::endl;

    return 0;
}
