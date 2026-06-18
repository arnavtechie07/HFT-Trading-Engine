#pragma once

#include <cstdint> // access to uint64_t

struct Order {
    uint64_t orderId;  // Unique ID for every order
    bool isBuy;        // true = Buy (Bid), false = Sell (Ask)
    uint64_t quantity; // Number of shares/contracts
    uint64_t price;    // Price multiplied by 100 to avoid floats
};
