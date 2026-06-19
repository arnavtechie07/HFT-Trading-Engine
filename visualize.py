import engine
import matplotlib.pyplot as plt
import random

#C++ Engine
book = engine.OrderBook()

#Dark Mode PLot
plt.style.use('dark_background')
plt.ion() 
fig,ax=plt.subplots(figsize=(12, 7))
fig.canvas.manager.set_window_title('Pro Order Book Depth Chart')

order_id= 1
print("Starting Market Simulation...")

try:
    while True:
        #Limiting the data generation speed for readability
        for _ in range(15): 
            is_buy=random.choice([True, False])
            price=random.randint(9500, 10500) 
            qty=random.randint(10, 100)
            book.add_order(engine.Order(order_id,is_buy,qty,price))
            order_id+=1
            
        bids=book.get_bids()
        asks=book.get_asks()

        ax.clear()

        # --- CUMULATIVE BIDS (Green Staircase) ---
        if bids:
            bid_prices=sorted(bids.keys(), reverse=True) 
            bid_vols=[bids[p] for p in bid_prices]
            
            # Aggregating the volume layer by layer
            cum_bid_vols=[sum(bid_vols[:i+1]) for i in range(len(bid_vols))]
            
            # Plot the filled staircase
            ax.fill_between(bid_prices, cum_bid_vols, color='#00ff00', step='post', alpha=0.3)
            ax.step(bid_prices, cum_bid_vols, color='#00ff00', where='post', linewidth=2, label='Cumulative Bids')

        # --- CUMULATIVE ASKS (Red Staircase) ---
        if asks:
            ask_prices=sorted(asks.keys())
            ask_vols=[asks[p] for p in ask_prices]
            
            cum_ask_vols=[sum(ask_vols[:i+1]) for i in range(len(ask_vols))]
            
            ax.fill_between(ask_prices, cum_ask_vols, color='#ff0000', step='post', alpha=0.3)
            ax.step(ask_prices, cum_ask_vols, color='#ff0000', where='post', linewidth=2, label='Cumulative Asks')

        # --- FORMATTING ---
        ax.set_title("Live Market Depth", fontsize=18, fontweight='bold', color='white')
        ax.set_xlabel("Price ($)", fontsize=12)
        ax.set_ylabel("Cumulative Volume (Shares)", fontsize=12)
        
        # Locking the axes to avoid shaking
        ax.set_xlim(94.00, 106.00) 
        
        ax.grid(color='#333333', linestyle='--', linewidth=0.5)
        ax.legend(loc='upper right', facecolor='#111111', edgecolor='white')

        #Making the Rendering slow
        plt.pause(0.5) 

except KeyboardInterrupt:
    print("\nPro Market Simulation Stopped.")
    plt.ioff()
    plt.show()