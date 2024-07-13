#include "map_orderbook.h"

#include <iomanip>

void map_orderbook::update(const updates& bid_updates, const updates& ask_updates) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for(auto &[price, quantity] : bid_updates)
        quantity ? bids[price] = quantity : bids.erase(price);
    for(auto &[price, quantity] : ask_updates)
        quantity ? asks[price] = quantity : asks.erase(price);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration = end_time - start_time;
    spin_time += duration;
    number_of_updates++;
}

void map_orderbook::display() {
    std::cout << std::left << std::setw(25) << "Bids" << " | " << std::setw(25) << "Asks" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    auto bid_it = bids.rbegin();
    auto ask_it = asks.begin();

    while (bid_it != bids.rend() || ask_it != asks.end()) {
        if (bid_it != bids.rend()) {
            std::cout << std::left << std::setw(12) << (std::to_string(bid_it->first) + " : " + std::to_string(bid_it->second));
            ++bid_it;
        } else {
            std::cout << std::left << std::setw(12) << " ";
        }

        std::cout << "|";

        if (ask_it != asks.end()) {
            std::cout << std::left << std::setw(12) << (std::to_string(ask_it->first) + " : " + std::to_string(ask_it->second));
            ++ask_it;
        } else {
            std::cout << std::left << std::setw(12) << " ";
        }
        std::cout << std::endl;
    }
}