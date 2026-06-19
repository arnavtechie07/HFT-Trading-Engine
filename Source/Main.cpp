#include <iostream>
#include "../include/Order.h"
#include "../include/OrderBook.h" 

int main() {
    OrderBook book;

    cout<<"--- 1. Placing Orders ---\n";
    book.addOrder({101, true, 10, 9900});  
    book.addOrder({102, true, 50, 9900});  
    book.addOrder({103, true, 100, 9800}); 
    book.printBook();

    cout<<"\n--- 2. Canceling Order 101 (Front of the line at $99) ---\n";
    book.cancelOrder(101);

    cout<<"\n--- 3. Updated Book Structure ---\n";
    book.printBook(); 
    
    return 0;
}