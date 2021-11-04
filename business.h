#pragma once
#include "hash_table.h"

/**
 * @file business.h
 * @author David Kidane & Felix Agnerdahl
 * @date --
 * @brief Back-end for a shopping website.
 * The shopping website uses a warehouse to store merchandise in shelves and a cart (both hash tables) which 
 * are used alongside one another to allow for management of stock and merchandise aswell as the purchasing of these.
 * The warehouse uses shelves to store merchandise, the backend user may manipulate these as they see fit by change ex. name and quantity etc.
 * @see https://uppsala.instructure.com/courses/47154/assignments/75644
 */

/// @brief The merchandise which is made up of a name, price, description, stock and location (shelf). 
/// The merchandise in itself is an element in a hashtable and its location is part of a list, 
/// the name of the merchandise serves as its key.
typedef struct merch merch_t;

typedef struct warehouse warehouse_t;

typedef struct cart cart_t;

/// @brief The shelf is the storage space for a merchandise and is itself a link in a linked list.
typedef struct shelf shelf_t;

#define make_spacing  printf("\n----------------------------------------------------------\n")

merch_t *create_merch(int id, char *name, char *desc, size_t price);

warehouse_t *ioopm_warehouse_create();

void ioopm_warehouse_destroy();

cart_t *ioopm_cart_create(warehouse_t *warehouse);

shelf_t *create_shelf(char *shelf_name);

void key_array_destroy(char **keys, size_t size);

/// @brief Adds a new item to the warehouse
/// @param warehouse The warehouse which is being modified by adding merchandise
/// @param merch The merchandise added to the warehouse
/// @return The newly added merchandise
bool ioopm_add_merch(warehouse_t *warehouse, char *name_sd, char *desc_sd, size_t price);

/// @brief Lists all items in the warehouse
/// @param warehouse The warehouse containing current 
/// @return prints 20 items at a time, the user can continue to print the next 20 items
void ioopm_list_merch(warehouse_t *warehouse);

merch_t *merch_get(warehouse_t *warehouse, char *merch_name);

/// @brief Completely removes an item from the warehouse
/// @param warehouse The warehouse which is being modified by removing the merch
/// @param merch The merchandise to be removed
/// @return The removed merchandise
bool ioopm_remove_merch(warehouse_t *warehouse, char *merch_name);

/// @brief Allows changes to items. Name, description and price. But the name is not allowed to be one that already exists
/// @param warehouse The warehouse in which the merchandise is stored
/// @param merch The merch which you want to edit
/// @param name The new name of the merchandise. (MUST NOT BE ONE THAT ALREADY EXISTS)
/// @param new_desc The new description of the merchandise
/// @param new_price The new price of the merchanise
/// @return The edited merchandise TODO:
bool ioopm_edit_merch(warehouse_t *warehouse, char *merch, char *new_name, char *new_desc, size_t new_price);

/// @brief Lists where the item is stored as well as the quantity of it in each storage location
/// @param warehouse The warehouse where the merchandise is stored
/// @param merch The merch we wish to know the storage locations of (shelf)
bool ioopm_show_stock(warehouse_t *warehouse, char *merch);

/// @brief Increases the stock of an item by atleast 1
/// @param warehouse The warehouse in which the merch is stored
/// @param merch The merch which you want to replenish the stock of
/// @param quantity The amount of which you wish to increase the stock with
bool ioopm_replenish_stock(warehouse_t *warehouse, char *merch_name, char *shelf_name, int quantity);

/// @brief Creates a shopping cart
/// @param cart The shopping cart being created
/// @return An empty shopping cart
ioopm_hash_table_t ioopm_create_cart(ioopm_hash_table_t *cart);

/// @brief Removes a shopping cart
/// @param cart Shopping cart to be removed
// void ioopm_remove_cart(warehouse_t *warehouse, cart_t *cart);
bool ioopm_remove_cart(warehouse_t *warehouse, int cart_id);

bool get_cart(warehouse_t *warehouse, int cart_id, cart_t **cart);

/// @brief Adds a quantity of merchandise to the cart
/// @param cart The cart which one wants to add merchandise to
/// @param merch The merchandise to be added
/// @param quantity The requested quantity of that merchandise
bool ioopm_add_to_cart(warehouse_t *warehouse, cart_t *cart, char *merch, size_t quantity);

/// @brief Removes zero or more of a specific merch from a cart
/// @param cart The cart that merchandise is to be removed from
/// @param merch The merch to be removed
/// @param quantity The amount of the merchandise to be removed
bool ioopm_remove_from_cart(warehouse_t *warehouse, cart_t *cart, char *merch_name, size_t quantity);

/// @brief Calculate the cost of all merchandise in a cart
/// @param cart The cart 
/// @return the cost of the shopping cart
size_t ioopm_calc_cost_cart(warehouse_t *warehouse, cart_t *cart);

/// @brief Confirms a purchase and changes the stock of all items purchased. Also removes the shopping cart
/// @param warehouse The warehouse which contains all items and their current stock
/// @param cart The cart which is being checked out and which therefore modifies the warehouse according to its purchased contents
/// @return 
void ioopm_checkout_cart(warehouse_t *warehouse, cart_t *cart);

/// @brief Quits the program.
void ioopm_quit(); 

bool ioopm_list_carts(warehouse_t *warehouse);
/*
/// @brief OPTIONAL Undos an action.
/// @param 
/// @return
void ioopm_undo();
*/
