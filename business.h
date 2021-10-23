#pragma once
#include "hash_table.h"

/**
 * @file business.h
 * @author David Kidane & Felix Agnerdahl
 * @date --
 * @brief Back-end for a shopping website.
 * The shopping website uses a warehouse to store merchendise in shelves and a cart (both hash tables) which 
 * are used alongside one another to allow for management of stock and merchendise aswell as the purchasing of these.
 * The warehouse uses shelves to store merchendise, the backend user may manipulate these as they see fit by change ex. name and quantity etc.
 * @see https://uppsala.instructure.com/courses/47154/assignments/75644
 */

/// @brief The merchendise which is made up of a name, price, description, stock and location (shelf). The merchendise in itself is an element in a hashtable and its location is part of a list, \
the name of the merchendise serves as its key.
typedef struct merch merch_t;

/// @brief The shelf is the storage space for a merchendise and is itself a link in a linked list.
typedef struct shelf shelf_t;

/// @brief Adds a new item to the warehouse
/// @param warehouse The warehouse which is being modified by adding merchandise
/// @param merch The merchandise added to the warehouse
/// @return The newly added merchandise
void ioopm_add_merch(ioopm_hash_table_t *warehouse, merch_t merch);

/// @brief Lists all items in the warehouse
/// @param warehouse The warehouse containing current 
/// @return prints 20 items at a time, the user can continue to print the next 20 items
void ioopm_list_merch(ioopm_hash_table_t *warehouse);

/// @brief Completely removes an item from the warehouse
/// @param warehouse The warehouse which is being modified by removing the merch
/// @param merch The merchandise to be removed
/// @return The removed merchandise
void ioopm_remove_merch(ioopm_hash_table_t *warehouse, merch_t merch);

/// @brief Allows changes to items. Name, description and price. But the name is not allowed to be one that already exists
/// @param warehouse The warehouse in which the merchandise is stored
/// @param merch The merch which you want to edit
/// @param name The new name of the merchandise. (MUST NOT BE ONE THAT ALREADY EXISTS)
/// @param new_desc The new description of the merchandise
/// @param new_price The new price of the merchanise
/// @return The edited merchandise
merch_t ioopm_edit_merch(ioopm_hash_table_t warehouse, merch_t merch, char *new_name, char *new_desc, size_t new_price);

/// @brief Lists where the item is stored as well as the quantity of it in each storage location
/// @param warehouse The warehouse where the merchandise is stored
/// @param merch The merch we wish to know the storage locations of (shelf)
void ioopm_show_stock(ioopm_hash_table_t warehouse, merch_t merch);

/// @brief Increases the stock of an item by atleast 1
/// @param warehouse The warehouse in which the merch is stored
/// @param merch The merch which you want to replenish the stock of
/// @param quantity The amount of which you wish to increase the stock with
void ioopm_replenish_stock(ioopm_hash_table_t warehouse, merch_t merch, size_t quantity);

/// @brief Creates a shopping cart
/// @param cart The shopping cart being created
/// @return An empty shopping cart
ioopm_hash_table_t ioopm_create_cart(ioopm_hash_table_t cart);

/// @brief Removes a shopping cart
/// @param cart Shopping cart to be removed
void ioopm_remove_cart(ioopm_hash_table_t cart);

/// @brief Adds a quantity of merchandise to the cart
/// @param cart The cart which one wants to add merchandise to
/// @param merch The merchandise to be added
/// @param quantity The requested quantity of that merchandise
void ioopm_add_to_cart(ioopm_hash_table_t cart, merch_t merch, size_t quantity);

/// @brief //TODO
/// @param //TODO
/// @param //TODO
/// @param //TODO
void ioopm_remove_from_cart(ioopm_hash_table_t cart, merch_t merch, size_t quantity);

/// @brief Calculate the cost of all merchandise in a cart
/// @param cart The cart 
/// @return the cost of the shopping cart
int ioopm_calc_cart_cost(ioopm_hash_table_t cart);

/// @brief Confirms a purchase and changes the stock of all items purchased. Also removes the shopping cart
/// @param warehouse The warehouse which contains all items and their current stock
/// @param cart The cart which is being checked out and which therefore modifies the warehouse according to its purchased contents
/// @return 
void ioopm_checkout_cart(ioopm_hash_table_t warehouse, ioopm_hash_table_t cart);

/// @brief Quits the program.
void ioopm_quit(); 


/*
/// @brief OPTIONAL Undos an action.
/// @param 
/// @return
void ioopm_undo();
*/
