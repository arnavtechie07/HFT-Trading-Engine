import engine #C++ MODULE!

print("Successfully loaded the C++ engine!")

#Starting C++ Engine
book = engine.OrderBook()

#Adding some C++ orders using Python
#Order(id, is_buy, quantity, price)
print("\n--- Placing Initial Orders ---")
book.add_order(engine.Order(101, True, 100, 9900))  
book.add_order(engine.Order(102, False, 50, 10100)) 

#Print resting book
book.print_book()

print("\n--- Incoming Aggressive Buyer! ---")

book.add_order(engine.Order(103, True, 75, 10500)) 

#Print updated book
book.print_book()