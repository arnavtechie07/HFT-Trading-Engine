#pragma once
#include<map>
#include<list>
#include "Order.h" // Including the Struct created
#include"OrderPool.h"
using namespace std;

// To Remember where an order lives
struct BookEntry{
    bool isBuy;
    uint64_t price;
    list<size_t>::iterator position;//C++ iterator ->pointer
};

class OrderBook{
private:
    //Memory pool inside the exchange
    OrderPool pool;
    // Bids: Buy Orders, Best Bid first.
    map<uint64_t,list<size_t>,greater<uint64_t>>bids;

    //Asks: Sell Orders, Best Ask First.
    map<uint64_t,list<size_t>>asks;

    //Tracks the order Id ->BookEntry.
    map<uint64_t,BookEntry>orderTracker;

public:
    //To add new order to the OrderBook
    void addOrder(const Order& order);

    //To Cancel an Order
    bool cancelOrder(uint64_t order_id);

    //To print the current status of the OrderBook
    void printBook();
    map<double,uint64_t>getBids();
    map<double,uint64_t>getAsks();
};
