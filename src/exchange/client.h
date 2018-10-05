#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

namespace exchange {
    class Client {
        public:
            Client(std::string name);
            std::vector<const Fill&> get_fills();
            std::vector<Order&> get_orders();
    };
}

#endif
