#include <vector>
#include <utility>
#include <chrono>

#pragma once

using updates = std::vector<std::pair<double, double>>;
class orderbook {
protected:
    std::chrono::duration<double, std::nano> spin_time;
    int number_of_updates;
public:
    virtual void update(const updates& bid_updates, const updates& ask_updates) = 0;
    virtual void display() = 0;
    double average_update_time() { return (number_of_updates > 0) ? spin_time.count() / number_of_updates : 0.0; }
};