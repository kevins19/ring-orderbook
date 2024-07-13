#include <map>
#include <vector>
#include <iostream>
#include <utility>
#include "orderbook.h"

#pragma once

class map_orderbook : public orderbook {
private:
    std::map<double, double> bids;
    std::map<double, double> asks;
public:
    void update(const updates& bid_updates, const updates& ask_updates) override;
    void display() override;
};