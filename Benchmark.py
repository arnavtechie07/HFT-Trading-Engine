import engine
import time
import random

def benchmark(orders=int(1e6)):
    print("==========================================")
    print("   HFT Engine Latency Benchmark Suite")
    print("==========================================\n")
    
    print(f"1. Pre-generating {orders:,} random orders...")
    #Building the Orders before starting the timer to isolate C++ performance
    orders = []
    for i in range(orders):
        is_buy=random.choice([True, False])
        #Wide price range to check linked lists
        price=random.randint(9000, 11000) 
        qty=random.randint(10, 100)
        
        #Creating C++ object and holding it in Python memory
        orders.append(engine.Order(i+1,is_buy,qty,price))
        
    print("2. Firing the C++ Memory Pool...")
    book=engine.OrderBook()
    
    print("3. Pushing orders into the matching engine...\n")
    
    # --- CRITICAL TIMING SECTION START ---
    start_time=time.perf_counter_ns()
    
    for order in orders:
        book.add_order(order)
        
    end_time=time.perf_counter_ns()
    # --- CRITICAL TIMING SECTION END ---
    
    #Calculate metrics
    total_ns=end_time-start_time
    total_seconds=total_ns/int(1e9)
    ns_per_order=total_ns/orders
    orders_per_second=orders/total_seconds
    
    #Results
    print("==========================================")
    print("             FINAL RESULTS                ")
    print("==========================================")
    print(f"Total Orders Processed : {orders:,}")
    print(f"Total Time Elapsed     : {total_seconds:.4f} seconds")
    print(f"Throughput             : {orders_per_second:,.0f} orders/second")
    print(f"Average Latency/Order  : {ns_per_order:,.0f} nanoseconds")
    print("==========================================\n")
    
    print("Measures End-to-End latency (Python Loop -> Pybind11 -> C++).")
    print("The pure C++ engine internally operates significantly faster!")

if __name__ == "__main__":
    benchmark(1_000_000)