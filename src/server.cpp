#include <set>
#include <mutex>

#include <chrono>
#include <thread>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

const int PORT = 9000;

class broadcast_server {
public:
    broadcast_server() : i(0) {
        m_server.init_asio();

        m_server.set_open_handler(bind(&broadcast_server::on_open,this,::_1));
        m_server.set_close_handler(bind(&broadcast_server::on_close,this,::_1));
        m_server.set_message_handler(bind(&broadcast_server::on_message,this,::_1,::_2));
    }

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        {
            std::lock_guard<std::mutex> lock(mu);
            i++;
        }

        // Send a private ACK back to the sender of the message
        m_server.send(hdl, std::string("ACK"), websocketpp::frame::opcode::text);

        // Broadcast a message to all connections
        std::stringstream m_ss;
        m_ss << "The new number is " << i;

        for (auto it : m_connections) {
            m_server.send(it, m_ss.str(), websocketpp::frame::opcode::text);
        }
    }

    void run(uint16_t port) {
        m_server.listen(port);
        m_server.start_accept();
        m_server.run();
    }
private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

    server m_server;
    con_list m_connections;

    int i;
    std::mutex mu;
};

int main() {
    broadcast_server server;
    std::cout << "Started server running on port " << PORT << std::endl;
    server.run(PORT);
}
