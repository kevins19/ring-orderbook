#include <map>
#include <vector>
#include <array>
#include <iostream>
#include <utility>
#include "orderbook.h"

#pragma once

class ring_orderbook : public orderbook {
private:
    enum Direction { Buy=-1, Sell=1 };
    struct book {
        static constexpr int sz = 128;
        std::array<double, sz> prices;
        std::array<double, sz> quantities;
        Direction direction;
        int head;
        int active_values;

        book(Direction d) : direction(d), head{0}, active_values{0} {} 
        void update(const double& price, const double& quantity);
        inline int get_index(const double& price);
    }; 
    book bids{Buy};     // ordered in decreasing order
    book asks{Sell};    // ordered in increasing order
public:
    void update(const updates& bid_updates, const updates& ask_updates) override;
    void display() override;
};