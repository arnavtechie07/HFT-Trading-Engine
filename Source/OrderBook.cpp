#include<iostream>
#include<algorithm>
#include "../include/OrderBook.h"
using namespace std;
//For Adding an Order
void OrderBook::addOrder(const Order& order){
    Order cur=order;
    
    if(cur.isBuy){
        //Matching the Bid with the corresponding Best Ask
        while(!asks.empty()&&cur.quantity>0){
            auto bestask=asks.begin();

            if(cur.price< bestask->first){
                break;
            }
            list<size_t>&queue=bestask->second;
            while(!queue.empty()&&cur.quantity>0){
                //Looking at the first seller in the line(FIFO)
                size_t& index=queue.front();
                Order& restOrder=pool.getOrder(index);//Accessing the actual order

                //Minimum of Ask and available quantity to sell
                uint64_t tradesize=min(restOrder.quantity,cur.quantity);
                cout<<"Trade Executed:"<<tradesize<<"shares @ $"<<(bestask->first/100.0)<<"\n";
                cur.quantity-=tradesize;
                restOrder.quantity-=tradesize;

                if(restOrder.quantity==0){
                    orderTracker.erase(restOrder.orderId);
                    queue.erase(queue.begin());
                    pool.deallocate(index);//Memory back to the pool
                }
            }
            if(queue.empty()){
                asks.erase(bestask);
            }  
        }
        if(cur.quantity>0){
            //Allocate the pool 
            size_t newindex=pool.allocate();
            pool.getOrder(newindex)=cur;//Copy our remaining Order in the pool
            bids[cur.price].push_back(newindex);

            //Iterator pointing to current order pushed at the last
            auto it=prev(bids[cur.price].end());
            // Logging it in the orderTracker
            orderTracker[cur.orderId]={true,cur.price,it};
        }
    }
    else{
        //Matching the sellers with the corresponding Best Bid

        while(!bids.empty()&&cur.quantity>0){
            auto bestbid=bids.begin();

            if(cur.price<bestbid->first){
                break;
            }
            
            list<size_t>&queue=bestbid->second;
            while(!queue.empty()&&cur.quantity>0){
                //Looking at the first buyer in the line(FIFO)
                size_t index=queue.front();
                Order& restOrder=pool.getOrder(index);
                //Minimum of the available quantity to sell and ask
                uint64_t tradesize=min(cur.quantity,restOrder.quantity);
                cout<<"Trade Executed:"<<tradesize<<"shares @ $"<<(bestbid->first/100.0)<<"\n";
                cur.quantity-=tradesize;
                restOrder.quantity-=tradesize;

                if(restOrder.quantity==0){
                    orderTracker.erase(restOrder.orderId);
                    queue.erase(queue.begin());
                    pool.deallocate(index);
                }
            }
            if(queue.empty()){
                bids.erase(bestbid);
            }
        }
        if(cur.quantity>0){
            size_t newindex=pool.allocate();
            pool.getOrder(newindex)=cur;
            asks[cur.price].push_back(newindex);
            
            //Iterator pointing to current order pushed at the last
            auto it=prev(asks[cur.price].end());
            //Logging it in the order Tracker
            orderTracker[cur.orderId]={false,cur.price,it};
        }
    }
}
//For Cancelling an order
bool OrderBook::cancelOrder(uint64_t orderId){
    //To check if the order Exists
    auto it=orderTracker.find(orderId);
    if(it==orderTracker.end()){
        cout<<"Cancel Failed: Order ID "<<orderId<<" not found.\n";
        return false;
    }
    //Location Details from our tracker
    BookEntry entry=it->second;
    size_t poolIndex;//Grabbing the index to deallocate 

    //Go to the correct location of that order and delete it
    if(entry.isBuy){
        poolIndex=*(entry.position);//Exracting the index from the iterater
        bids[entry.price].erase(entry.position);
        if(bids[entry.price].size()==0){
            bids.erase(entry.price);
        }
    }
    else{
        poolIndex=*(entry.position);
        asks[entry.price].erase(entry.position);
        if(asks[entry.price].size()==0){
            asks.erase(entry.price);
        }
    }
    //Erasing the order from the tracker itself
    orderTracker.erase(it);

    //Deallocating the index to the pool
    pool.deallocate(poolIndex);

    cout<<"Order Cancelled: ID "<<orderId<<"\n";
    return true;
}

//For Printing the current OrderBook
void OrderBook::printBook() {
    cout<<"\n========== ORDER BOOK ==========\n";
    cout<<"--- Asks (Sellers) ---\n";

    for(auto it=asks.rbegin();it!=asks.rend();++it){//Reverse iteartor to print highest asks first
        uint64_t totalquantity=0;
        for(size_t index:it->second){
            totalquantity+= pool.getOrder(index).quantity;//Add all the shares at this price
        }
        cout<<"Price: $"<<(it->first/100.0)<<" | Total Vol: "<<totalquantity<<"\n";
    }
    cout<<"--------------------------------\n";

    cout<<"--- BIDS (Buyers) ---\n";
    for(auto it=bids.rbegin();it!=bids.rend();++it) {
        uint64_t totalquantity=0;
        for(size_t index:it->second) {
            totalquantity+=pool.getOrder(index).quantity;
        }
        cout<<"Price: $"<<(it->first/100.0)<<" | Total Vol: "<<totalquantity<<"\n";
    }
    cout<<"================================\n\n";
}

//To get the total Volume at a certain price
map<double,uint64_t>OrderBook::getAsks(){
    map<double,uint64_t>askDepth;
    for(const auto&[price,queue]:asks){
        uint64_t total=0;
        for(size_t index:queue){
            total+=pool.getOrder(index).quantity;
        }
        askDepth[price/100.0]=total;
    }
    return askDepth;
}
map<double,uint64_t>OrderBook::getBids(){
    map<double,uint64_t>bidDepth;
    for (const auto&[price,queue]:bids) {
        uint64_t total=0;
        for (size_t index:queue) {
            total+=pool.getOrder(index).quantity;
        }
        bidDepth[price/100.0]=total;
    }
    return bidDepth;
}