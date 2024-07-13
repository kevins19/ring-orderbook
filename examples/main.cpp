#include "listener.h"
#include "map_orderbook.h"
#include "ring_orderbook.h"
#include <boost/core/demangle.hpp>

double run_websocket_for_orderbook(const std::shared_ptr<orderbook>& ob, const std::string& symbol) {
    listener ws_listener(symbol);
    ws_listener.subscribe(ob);
    std::thread connect_thread([&ws_listener]() { ws_listener.connect(); });

    std::this_thread::sleep_for(std::chrono::seconds(30));

    ws_listener.disconnect();

    if (connect_thread.joinable()) {
        connect_thread.join();
    }

    return ob->average_update_time();
}

int main() {
    auto map_ob = std::make_shared<map_orderbook>();
    auto ring_ob = std::make_shared<ring_orderbook>();

    auto map_future = std::async(std::launch::async, run_websocket_for_orderbook, map_ob, "SOLUSDT");
    auto ring_future = std::async(std::launch::async, run_websocket_for_orderbook, ring_ob, "SOLUSDT");

    double map_avg_time = map_future.get();
    double ring_avg_time = ring_future.get();

    std::cout << "Average update time (ns) for map websocket: " << map_avg_time << std::endl;
    std::cout << "Average update time (ns) for ring orderbook: " << ring_avg_time << std::endl;

    return 0;
}