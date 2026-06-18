#pragma once
#include<vector>
#include"Order.h"
#include<stdexcept>
using namespace std;

class OrderPool{
private:
    vector<Order>pool; //Pre allocated chunk of Memory
    vector<size_t>freeIndices; //Track of empty slots

public:
    //Runs once the engine starts
    OrderPool(size_t capacity=1e4){
        //Pre allocating the memory from OS all at once
        pool.resize(capacity);
        freeIndices.reserve(capacity);

        //Initially every single index is free
        //Pushing them backwards so index 0 at Top
        for(size_t i=capacity;i>0;i--){
            freeIndices.push_back(i-1);
        }
    }

    //TO Grab a free slot
    size_t allocate(){
        if(freeIndices.empty()){
            throw runtime_error("Memory Pool Exhausted!");
        }
        size_t index=freeIndices.back();
        freeIndices.pop_back();
        return index;
    }

    //Return a slot to the pool
    void deallocate(size_t index){
        freeIndices.push_back(index);
    }

    //To read/write the actual order of pool
    Order& getOrder(size_t index){
        return pool[index];
    }
};