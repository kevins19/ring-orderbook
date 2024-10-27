#include "ring_orderbook.h"
#include <cmath>
#include <iomanip>

void ring_orderbook::book::update(const double& price, const double& quantity) {
    if(active_values) {
        auto index_to_update = get_index(price);
        if(index_to_update == -1)
            return;
        active_values -= quantities[index_to_update] ? 1 : 0;
        prices[index_to_update] = price;
        quantities[index_to_update] = quantity;
        active_values += quantity ? 1 : 0;
        if(quantity && price * direction < prices[head] * direction)
            head = index_to_update;
        if(!quantity && head == index_to_update && active_values) {
            while(quantities[head] == 0)
                head = (head + 1) % sz;
        }
    } else {
        head = 0;
        active_values = quantity ? 1 : 0;
        prices[0] = price;
        quantities[0] = quantity;
    }
}
int ring_orderbook::book::get_index(const double& price) {
    int diff = std::round(direction * (price - prices[head]) * 100);
    if(abs(diff) > sz) 
        return -1;
    else 
        return (diff + head + sz) & (sz - 1);
}

void ring_orderbook::update(const updates& bid_updates, const updates& ask_updates) {
    auto start_time = std::chrono::high_resolution_clock::now();
    for(auto &[price, quantity] : bid_updates)
        bids.update(price, quantity);
    for(auto &[price, quantity] : ask_updates)
        asks.update(price, quantity);
        auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> duration = end_time - start_time;
    spin_time += duration;
    number_of_updates++;
}

void ring_orderbook::display() {
    std::cout << std::left << std::setw(25) << "Bids" << " | " << std::setw(25) << "Asks" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    for (int i = 0; i < asks.sz; i++) {
        std::cout << std::setw(12) << bids.prices[i] << " : " << std::setw(12) << bids.quantities[i];
        std::cout<<"|";
        std::cout << std::setw(12) << asks.prices[i] << " : " << std::setw(12) << asks.quantities[i];
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}
