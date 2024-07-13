#include "listener.h"
#include <format> 
#include <nlohmann/json.hpp>


using json = nlohmann::json;

void listener::connect() {
    ws_client.init_asio();

    ws_client.clear_access_channels(websocketpp::log::alevel::all);
    ws_client.clear_error_channels(websocketpp::log::elevel::all);

    ws_client.set_tls_init_handler([](websocketpp::connection_hdl) {
        auto ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12);
        try {
            ctx->set_options(boost::asio::ssl::context::default_workarounds |
                             boost::asio::ssl::context::no_sslv2 |
                             boost::asio::ssl::context::no_sslv3 |
                             boost::asio::ssl::context::single_dh_use);
        } catch (std::exception &e) {
            std::cout << "Error in context pointer: " << e.what() << std::endl;
        }
        return ctx;
    });

    ws_client.set_open_handler([&](websocketpp::connection_hdl hdl) {
        std::cout << "Connected to Bybit WebSocket" << std::endl;
        std::string subscribe_msg = std::format(R"({{"op": "subscribe", "args": ["orderbook.50.{}"]}})", symbol);
        this->hdl = hdl; 
        ws_client.send(hdl, subscribe_msg, websocketpp::frame::opcode::text);
    });

    ws_client.set_message_handler([&](websocketpp::connection_hdl, client::message_ptr msg) {
        if(!ob) 
            std::cout << "Listener has not been given an orderbook yet." << std::endl;
        else {
            json market_incremental = json::parse(msg->get_payload());
            std::vector<std::pair<double, double>> bid_updates;
            std::vector<std::pair<double, double>> ask_updates;
            for (const auto& bid : market_incremental["data"]["b"]) 
                bid_updates.emplace_back(std::stod(bid[0].get<std::string>()), std::stod(bid[1].get<std::string>()));
            for (const auto& ask : market_incremental["data"]["a"]) 
                ask_updates.emplace_back(std::stod(ask[0].get<std::string>()), std::stod(ask[1].get<std::string>()));
            ob -> update(bid_updates, ask_updates);
        }
    });

    ws_client.set_close_handler([](websocketpp::connection_hdl) {
        std::cout << "Connection closed" << std::endl;
    });

    ws_client.set_fail_handler([](websocketpp::connection_hdl) {
        std::cout << "Connection failed" << std::endl;
    });

    std::string uri = "wss://stream.bybit.com/v5/public/spot";
    websocketpp::lib::error_code ec;
    client::connection_ptr con = ws_client.get_connection(uri, ec);
    if (ec) {
        std::cout << "Could not create connection because: " << ec.message() << std::endl;
        return;
    }
    ws_client.connect(con);
    // run on detached thread to disconnect later
    std::thread([this]() { ws_client.run(); }).detach();
};

void listener::subscribe(const std::shared_ptr<orderbook>& o) {
    ob = o;
}

void listener::disconnect() {
    websocketpp::lib::error_code ec;
    ws_client.close(hdl, websocketpp::close::status::normal, "Normal closure", ec);
    if (ec) {
        std::cout << "Error closing connection: " << ec.message() << std::endl;
    }
    ws_client.stop();
}