# MULTI-PARADIGM PROGRAMMING - PROJECT - SHOP SIMULATION


The project contains three programmes simulating a shop. 
This was to be completed using two programming paradigms - Procedural Programming (POP) and Object-Oriented Programming (OOP).

    - OOP:  Implemented using Python.
    - POP:  Implemented using Python and C.


## Objective:
The programme is a simulation of a shop and processed customer orders. The following objectives should be recreated in POP with C and Python and OOP with Python. 

	- Display shop stock and current cash.
	- Reading and processing a customer order from a CSV file.
	- Processing a live customer order through user input.
	- Throw an error if an order cannot be completed due to stock shortage or insufficient funds.
	- Update shop cash and stock as an order is processed.
	- Identical “user experience” for each implementation.


## Folder Contents: 
    - Inside the main project folder there are 3 subfolders.
        1. c - This folder contains the c implementation of the shop in POP.
        2. python oop - This folder contains the python implementation of the shop in OOP.
        3. python proc - This folder contains the python implementation of the shop in POP.

    - stock.csv contains the stock to create the shop. This is read in by all three implementations.

    - report.pdf which compares the similarities and differences between the procedural and the object-oriented approach.

    - There are serveral CSV files which are test customer order files.
        1. customer.csv - This was provided in the downloaded project folder. It is a valid customer order and contains a product not stocked in the shop.
        2. not_enough.csv - A customer order which cannot be fulfilled due to the customer ordering a quantity of Big Bags greater than the shop stocks.
        3. low_funds.csv - A customer order which cannot be fulfilled due to the customer not having enough cash to purchase their order.
        4. valid_order.csv - A valid customer order which can be fulfilled.   

    * Note: customer.csv and valid_order.csv may only be vaild and fulfilled if processed once depending on updated shop stock quantity.