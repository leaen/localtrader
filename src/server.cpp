#include <set>
#include <mutex>

#include <chrono>
#include <thread>

#include <iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include "order.h"
#include "orderbook.h"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

const int PORT = 9000;

class broadcast_server {
public:
    broadcast_server() : i(0), ob("ABC") {
        m_server.init_asio();

        m_server.set_open_handler(bind(&broadcast_server::on_open,this,::_1));
        m_server.set_close_handler(bind(&broadcast_server::on_close,this,::_1));
        m_server.set_message_handler(bind(&broadcast_server::on_message,this,::_1,::_2));

        auto channels = websocketpp::log::alevel::connect | websocketpp::log::alevel::disconnect;

        m_server.clear_access_channels(websocketpp::log::alevel::all);
        m_server.set_access_channels(channels);

        ob.set_trade_announcements(true);
    }

    void on_open(connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(connection_hdl hdl) {
        m_connections.erase(hdl);
    }

    void on_message(connection_hdl hdl, server::message_ptr msg) {
        if (msg->get_payload() == "bb") {
            double best_bid = ob.get_best_bid();

            std::stringstream m_ss;
            m_ss << "bb|" << std::fixed << std::setprecision(4) << best_bid;

            m_server.send(hdl, m_ss.str(), websocketpp::frame::opcode::text);
            return;
        } else if (msg->get_payload() == "bo") {
            double best_offer = ob.get_best_offer();

            std::stringstream m_ss;
            m_ss << "bo|" << std::fixed << std::setprecision(4) << best_offer;

            m_server.send(hdl, m_ss.str(), websocketpp::frame::opcode::text);
            return;
        } else if (msg->get_payload() == "bbbo") {
            double best_bid = ob.get_best_bid();
            double best_offer = ob.get_best_offer();
            long long current_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

            std::stringstream m_ss;
            m_ss << "bbbo" << std::fixed << std::setprecision(4)
                 << '|' << best_bid
                 << '|' << best_offer
                 << '|' << current_ms;

            m_server.send(hdl, m_ss.str(), websocketpp::frame::opcode::text);
            return;
        }

        exchange::Order* o;
        bool success;

        std::tie(o, success) = exchange::Order::deserialize(msg->get_payload());

        if (!success) {
            std::cout << "Failed to decode order " << (msg->get_payload()) << std::endl;
            return;
        }

        {
            ob.submit_order(*o);
            std::lock_guard<std::mutex> lock(mu);
            i++;
        }

        // Send a private ACK back to the sender of the message
        m_server.send(hdl, std::string("ACK"), websocketpp::frame::opcode::text);

        // Broadcast a message to all connections
        std::stringstream m_ss;
        m_ss << "The new number is " << i;

        for (auto it : m_connections) {
            try {
                m_server.send(it, m_ss.str(), websocketpp::frame::opcode::text);
            } catch (websocketpp::exception) {
                std::cerr << "Failed to send sequence update message" << std::endl;
            }
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

    exchange::Orderbook ob;
};

int main() {
    broadcast_server server;
    std::cout << "Started server running on port " << PORT << std::endl;
    server.run(PORT);
}
