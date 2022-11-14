#include <stdio.h>
#include <string.h>
#include <stdlib.h>


///// CREATING THE STRUCTS //////

// Stuct for product
struct Product {
	char* name;
	double price;
};

// Struct for product stock
struct ProductStock {
	struct Product product;
	int quantity;
};

// Struct for the shop
struct Shop {
	double cash;
	struct ProductStock stock[20];
	int index;
};

// Struct for the customer
struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index;
};



//// PRINT FUNCTIONS ////


// Printing the product name and price
void printProduct(struct Product p)
{
	printf("PRODUCT NAME: %s \nPRODUCT PRICE: %.2f\n", p.name, p.price);
	printf("-------------\n");
}


// Printing the customer name and budget
void printCustomer(struct Customer c)
{
	printf("CUSTOMER NAME: %s \nCUSTOMER BUDGET: %.2f\n", c.name, c.budget);
	printf("-------------\n");
    // Loop through customer array
	for(int i = 0; i < c.index; i++)
	{
        // Getting the customers shopping list and from list getting the indivdual product and name and quanity
		printProduct(c.shoppingList[i].product);
		printf("%s ORDERS %d OF ABOVE PRODUCT\n", c.name, c.shoppingList[i].quantity);
        // The cost is the quanity on the shopping list * by the product  price
		double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price; 
		printf("The cost to %s will be €%.2f\n", c.name, cost);
	}
}


///// CREATING THE SHOP ///////


struct Shop createAndStockShop()
{
    // Struct to represent the shop
	struct Shop shop = { 200 };
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;


    // Opening the file and reading it ('r')
    fp = fopen("stock.csv", "r");
    // If file doesn't exist exit the programme
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // Reading line one by one until it get to the end
    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s IS A LINE", line);

        // Using string tokenizer
        // Getting the name of the product by breaking apart at the comma. Hold in the name variable
		char *n = strtok(line, ",");
        // Getting the price. NULL to keep working on the same string to get the vaule at the next comma split.
		char *p = strtok(NULL, ",");
        // Getting the quanity. 
		char *q = strtok(NULL, ",");

        // Converting quanity from string to an interger (this made new memeory malloc to rectify)
		int quantity = atoi(q);
        // Converting price from string to a double/float (this made new memeory malloc to rectify)
		double price = atof(p);

        // Creating second character varaible because the name is getting overwritten
        // Allocate memory for new vaiable  to the size of characters
		char *name = malloc(sizeof(char) * 50); 
		strcpy(name, n); // Takes from source (n), makes a copy of the data and puts into the new memory location (malloc)

        // Put the above data into a struct to represent the product
		struct Product product = { name, price };
        // The data from above to represent the product (struct above) and its quanity
		struct ProductStock stockItem = { product, quantity };

        // Adding these structs (arrays) to the shop
		shop.stock[shop.index++] = stockItem;
		// printf("NAME OF PRODUCT %s PRICE %.2f QUANTITY %d\n", name, price, quantity);
    }
	
    // Return a shop at the end of createAndStockShop() method
	return shop;
}



// Print method for the shop
void printShop(struct Shop s)
{
    // Print the amount of money in the shop
	printf("Shop has %.2f in cash\n", s.cash);
    // Loop through the stocked products in the shop
	for (int i = 0; i < s.index; i++)
	{   
        // Reusing method to print the products in the shop
		printProduct(s.stock[i].product);
        // Print the quanity of the product
		printf("The shop has %d of the above\n", s.stock[i].quantity);
	}
}





int main(void) 
{
	// struct Customer dominic = { "Dominic", 100.0 };
	// struct Product coke = { "Can Coke", 1.10 };
	// struct Product bread = { "Bread", 0.7 };

	// // printProduct(coke);
	
	// struct ProductStock cokeStock = { coke, 20 };
	// struct ProductStock breadStock = { bread, 2 };
	
    // Adding an item to a customers list
	// dominic.shoppingList[dominic.index++] = cokeStock;
	// dominic.shoppingList[dominic.index++] = breadStock;

	// printCustomer(dominic);
	

    // Passing the returned shop to the method
	struct Shop shop = createAndStockShop();
	printShop(shop);
	
// printf("The shop has %d of the product %s\n", cokeStock.quantity, cokeStock.product.name);
	
    return 0;
}