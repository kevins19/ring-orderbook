#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include "orderbook.h"

#pragma once

using client = websocketpp::client<websocketpp::config::asio_tls_client>;

class listener {
public:
    listener(const std::string& s) : symbol(s) {}
    void connect();
    void subscribe(const std::shared_ptr<orderbook>& o);
    void disconnect();

private:
    std::string symbol;
    client ws_client;
    std::shared_ptr<orderbook> ob;
    websocketpp::connection_hdl hdl;
};