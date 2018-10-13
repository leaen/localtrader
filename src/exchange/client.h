#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

namespace exchange {
    class Client {
        public:
            Client(std::string name) : name(name) {};
            Client(const char* name) : name(name) {};

            std::string get_name();
        private:
            std::string name;
    };
}

#endif
