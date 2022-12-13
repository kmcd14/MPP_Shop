#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



/////////////////////////////////////////////////////////////////


// Inculding the getline function to read a string of text 
int my_getline(char **lineptr, size_t *n, FILE *stream) {
	static char line[256];
	char *ptr;
	unsigned int len;


   if (lineptr == NULL || n == NULL) {
      errno = EINVAL;
      return -1;
   }

   if (ferror (stream))
      return -1;

   if (feof(stream))
      return -1;
     
   fgets(line,256,stream);

   ptr = strchr(line,'\n');   
   if (ptr)
      *ptr = '\0';

   len = strlen(line);
   
   if ((len+1) < 256) {
      ptr = realloc(*lineptr, 256);
      if (ptr == NULL)
         return(-1);
      *lineptr = ptr;
      *n = 256;
   }

   strcpy(*lineptr,line); 
   return(len);
}



//////////////////////////////////////////////////////////////////

// Equivalent to python OOP classes and dataclasses

// Product struct
struct Product {
	char* name;
	double price;
};


// ProductStock struct
struct ProductStock {
	struct Product product;
	int quantity;
};


// Shop struct
struct Shop {
	double cash;
	struct ProductStock stock[20];
	int index; 
};


// Customer stock
struct Customer {
	char* name;
	double budget;
	struct ProductStock shoppingList[10];
	int index; // current index of shoppingList
};




//////////////////////////////////////////////////////////////////


// PRINT FUNCTIONS //


// Function to print product name and price
void printProduct(struct Product p)
{
	printf("%s\t\t %.2f\n", p.name, p.price);
}



// Function to print customer
void printCustomer(struct Customer c) {	

	double total = 0.0; // Variable to track customer order total

	printf("##########################################################\n");
    printf("\nCustomer: %s\nBudget: %.2f", c.name, c.budget);
    printf("\n***************************************************");
	
	// Print the shopping list.
    printf("\n\n Product\t\t Price\t Qty\t Unit Total\n");
    printf("___________________________________________________\n");

	// Calculate unit cost (quantity x price)
	for (int i = 0; i < c.index; i++) {
		
		// Variable to track unit cost for each item
		double cost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
	
		printf("\n %s", c.shoppingList[i].product.name);
		printf("\t\t %.2f \t  %d \t %.2f", c.shoppingList[i].product.price, c.shoppingList[i].quantity, cost);
		printf("\n---------------------------------------------------\n\n");
		
		total += cost; // Update total cost		
	}

	printf("\n Total\t\t\t\t\t %.2f", total);
    printf("\n--------------------------------------------------\n");
	
}



// Print function to display the shop 
void printShop(struct Shop s) {

	printf("##########################################################\n");
	printf("\t\tGENERAL STORE OVERVIEW\t\t\n");
	printf("##########################################################\n");
    printf("\nShop Cash: %.2f\n", s.cash);
    printf("*********************************************\n");
	printf("\t\tCatalogue");
	printf("\n*********************************************");
    printf("\n Product\t\tPrice\t\tQty");
    printf("\n_____________________________________________\n");

	// Loop through each stock item and print name, price and qty
	for (int i = 0; i < s.index; i++) {

		printf("\n%s", s.stock[i].product.name);
        printf("\t\t%.2f\t\t%d", s.stock[i].product.price, s.stock[i].quantity);
        printf("\n----------------------------------------------\n");
	}
}



// Function to print main shop menu
void menuOptions() {
	
	printf("##########################################################\n");
	printf("\t\t\tGENERAL STORE\t\t\n");
	printf("##########################################################\n");
	
	printf("\n\nWelcome to the general store. How can we help you today?\n\n");
    printf("\t-[1] View shop\n");
    printf("\t-[2] Place an order\n");
    printf("\t-[3] Import Order from a file\n");
    printf("\t-[0] Exit\n");	
}



//////////////////////////////////////////////////////////////////



// Function to stock the shop by reading in the stock.csv file
struct Shop createAndStockShop() {

    FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	char *filename = "../stock.csv"; 

	// Try to open and read the csv
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "cannot open %s: \n", filename);
		exit(EXIT_FAILURE);
	}
	
	
    read = my_getline(&line, &len, fp);
	double initialCash = atof(line);
	struct Shop shop = {initialCash};


	while ((read = my_getline(&line, &len, fp)) != -1) {
		char *n = strtok(line, ","); // Get name
		char *p = strtok(NULL, ","); // Get price
		char *q = strtok(NULL, ","); // Get quantity

		int quantity = atoi(q); // Convert to integer
		double price = atof(p); // Convert to double

		char *name = malloc(sizeof(char) * 50); // Allocate memory for product name (returns pointer)
		strcpy(name, n); // copy n into name

		// Create the Product and ProductStock structs
		struct Product product = {name, price};
		struct ProductStock stockItem = {product, quantity};

		shop.stock[shop.index++] = stockItem; // Increment index for next item
	}

	return shop; // return a shop struct
}




// Function to check if product exists
struct Product getProduct(struct Shop s, char *customerProduct) {

	struct Product p;
	// Loop over shop index to see if the customer item is in the product list 
	for (int i = 0; i < s.index; i++) {
		// strcasecmp removes case sensitivity
		if (strcasecmp(s.stock[i].product.name, customerProduct) == 0) {
			p = s.stock[i].product;
		}
	}
	return p; // Returns Product struct
};




// Function to read a customer's order from a csv file
struct Customer customerOrder(struct Shop s) {	

	FILE * fp;
    char filepath[250];
	char * line = NULL;
	size_t len = 0;
	size_t read;

    printf("Enter csv filepath: ");
	scanf("%s", &filepath);

	// Try to open and read csv file
	fp = fopen(filepath,"r");
    if (fp == NULL) {
        printf("Error %s\n", strerror(errno));
        exit(100);
    }

	else {

        my_getline(&line, &len, fp);

        char *cn = strtok(line,","); // Get customer name
        char *cb = strtok(NULL,","); // Get customer budget

        char *customerName = malloc(sizeof(char)*50); // Allocate memory for customer name (returns pointer)
        strcpy(customerName, cn); // copy cn into customerName
        double customerBudget = atof(cb); // Convert to double

        // Create Customer struct
        struct Customer customer = {customerName, customerBudget};

        while ((read = my_getline(&line, &len, fp)) != -1) {
		
            char *p = strtok(line, ","); // Get customer product name
            char *q = strtok(NULL,","); // Get customer product quantity
            
            int customerQuantity = atoi(q); // Convert to integer

            char *customerItem = malloc(sizeof(char)*50); // Allocate memory for customer product name (returns pointer)
            strcpy(customerItem, p); // copy p into customerItem


			// Create Product and ProductStock struts with customer order
            struct Product product = {customerItem, getProduct(s,customerItem).price};
            struct ProductStock listItem = {product, customerQuantity};

            customer.shoppingList[customer.index++] = listItem;	// Increment index for next item
	    }
	    
        return customer; // Return Customer struct
    }

};



// Function to check if product exists. Returns the product struct
char* productName(struct Shop* s, char *customerProduct) {

	for (int j = 0; j < s->index; j++) {
        // strcasecmp removes any case sensitivity
		if (strcasecmp(s->stock[j].product.name, customerProduct) == 0) {
        	return s->stock[j].product.name;   // Return product if it exist in the shop 
        }         
    }
	return NULL; // Return NULL if customer product not in shop stock
}




// Function to check stock quantity 
int getQuantity(struct Shop* s, char* customerProduct) {
   // Loop through shop index and get its quantity
   for (int i = 0; i < s->index; i++) {
		// strcasecmp removes case sensitivity
    	if(strcasecmp(s->stock[i].product.name, customerProduct) == 0) {
        	int q = s->stock[i].quantity;
			return q; // Return quantity if product exists in shop		
      }
   }
	return 0; // Return 0 if not in stock
};




// Function to process customer order
void processOrder(struct Shop* s, struct Customer* c) {	

    // Variable to track whether to checkout a customers order
    char checkout = printf("\n\nCheckout %s's order? (y/n): ", c->name); 
	scanf("%s", &checkout);


    if ((checkout == 'y') || (checkout == 'Y')) {

        printf("\n -------------------------------------------------\n");
        printf("\n Checkout complete!\n");
       

        // Loop over customers shopping list
        for (int i = 0; i < c->index; i++) {

            char *customerItem = malloc(sizeof(char) * 50); // Variable for product name in shoppingList
            strcpy(customerItem, c->shoppingList[i].product.name); // Copy product name into customerItem

            int customerQuantity = c->shoppingList[i].quantity; // Variable to track customer order quantity
            double productPrice = c->shoppingList[i].product.price; // Variable to track product price
			double orderTotal = customerQuantity * productPrice; // Variable to track unit total price


            char* shopProduct = productName(s,customerItem); // Check if product exists in shop
            int shopQuantity = getQuantity(s,shopProduct); // Check shop has enough stock 
            
            // Process order // 

			// If product doesn't exist in shop ignore it
			if (shopProduct == NULL) {
				NULL;
			}
            // If customer orders less than or equal to the the shop has in stock  
            else if (shopQuantity - customerQuantity >= 0) {		
				// and the order total is less than or equal the customers budget 
                if (orderTotal <= c->budget) {  
					// Update the shop stock 
                    for (int j = 0; j < s->index; j++) {
                        if (s->stock[j].product.name == shopProduct) {
                                s->stock[j].quantity = shopQuantity - customerQuantity;      
                            }
                        }

                    // Update the shop and customers cash        
                    s->cash += orderTotal;
                    c->budget -= orderTotal;
                
                }
                // If the order total is more than the customers budget
                else {
                    printf("\n***************************************************************\n");
                    printf("\nCannot complete order: insufficient Funds");
                    printf("\n***************************************************************\n"); 
                }
            }
            // If customer orders more than is stocked in the shop  
            else {
                    printf("\n***************************************************************\n");
                    printf("\n !!! Order incomplete: due to %s stock shortage !!!\n", customerItem );	
                    printf("\n***************************************************************\n");  
                }
            }

        return;
    }   
}



// Function to get customer name, budget and shopping list (live order)
struct Customer liveOrder(struct Shop s) {

	struct Customer liveCustomer; // Variable to hold liveCustomer

	// Get customer name from input
	printf("Welcome to the general store. What's your name? ");
	char *customerName = malloc(sizeof(char) * 50);
	scanf("%s", customerName);
	liveCustomer.name = customerName; // Assign to liveCustomer.name
	
	// Get customer budget from input
	printf("Hi %s, what's your budget: ", customerName);
	double customerBudget;
	scanf("%lf", &customerBudget);
	liveCustomer.budget = customerBudget;  // Assign to liveCustomer.budget


    printf("\nPerfect. Please choose from our product list:\n");
    printf("*********************************************\n");
    printf("\t\tCatalogue");
	printf("\n_____________________________________________\n");
    printf("\n Product\t\tPrice\n");
    printf("*********************************************\n");
	
	// Loop through shop stock and print product name and price
	for (int i = 0; i < s.index; i++) {    
        printf("%s\t\t%.2f\n---------------------------------------------\n", s.stock[i].product.name, s.stock[i].product.price);	
    }

	char addItem;  // Varaible to track if customer wishes to add additional products

	// Continue the while loop until the customer is finished adding items by pressing 'n' to adding additional items
	while (strcasecmp(&addItem, "n") != 0) {

		// Get product name from input
		printf("\nWhat product would you like? ");
		char* customerItem = malloc(sizeof(char)*50);
		scanf("\n%[^\n]%*c", customerItem); // Regular expression that reads product name without last "\n" character

		// Get product order quantity from input
		printf("How many? ");
		int customerQuantity;
		scanf("%d", &customerQuantity);
		printf("\n");


		// Create Product and ProductStock from customers inputs and appends to customer shopping list
		struct Product productChoice = {customerItem, getProduct(s, customerItem).price};
		struct ProductStock productQuantity = {productChoice, customerQuantity};

		// Append product to customer shoppingList and increment index for next item
		liveCustomer.shoppingList[liveCustomer.index] = productQuantity;	
		liveCustomer.index++; 

		// Get customers shoppingList total(quantity x price)
        getProduct(s, customerItem);
        double total = customerQuantity * getProduct(s, customerItem).price;
		

		// Continue the while loop until the customer is finished adding items
		printf("Do you need anything else? (y/n): ");
		printf("\n");
		scanf("%s", &addItem);
	}
	
	return liveCustomer; // Return struct
}




// Function for the main menu
void displayMenu(struct Shop shop) {
	
    menuOptions(); // Show main shop menu


    printf("\nPlease select an option: ");
    char choice; // Variable to store users choice from menu
    scanf("%s", &choice);
	
    
    if (choice == '1') { // Display shop // 
			
		printShop(shop); // Print shop
        displayMenu(shop); // Return to main menu
	}
        
    else if (choice == '2') { // Place a live order //

		struct Customer liveCustomer = liveOrder(shop); // Create a live customer	
		printCustomer(liveCustomer); // Print customer
		processOrder(&shop, &liveCustomer); // Process customer order, pass in customer and shop 
        printf("\nThe balance on %s's account: EUR %.2f\n",liveCustomer.name, liveCustomer.budget); // Print customers new balance
        displayMenu(shop); // Return to main menu
	} 
		
	else if (choice == '3') { // Read order from a csv file //
		
		struct Customer customer = customerOrder(shop); // Create customer from csv file
		printCustomer(customer); // Print customer
		processOrder(&shop, &customer); // Process customer order
        printf("\nThe balance on %s's account: %.2f\n",customer.name, customer.budget); // Print customers new balance
        displayMenu(shop); // Return to main menu  
	} 

	else if (choice == '0') { // Exit programme //
			
		printf("Thanks for shopping at the general store, goodbye!"); 
		exit(0);
	} 
        
    else {
          
        displayMenu(shop); // Any other key is pressed show main menu
	}
}


//////////////////////////////////////////////////////////////////


// Main method for when the programme executes 
int main(void) {

	struct Shop shop = createAndStockShop(); // Stock the shop
	displayMenu(shop); // Show the display menu
    return 0;
}
