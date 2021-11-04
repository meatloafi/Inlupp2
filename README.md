Webstore Backend with Terminal Storefront
========

Fully functional webstore with carts, shelves, merch and a warehouse. All these work together to allow for a storefront where you can manage
your merchendise and carts (add, edit & remove) aswell as add the merchendise to different shelves and carts. With this flexible webstore you
have a good base on which to build your own store!


1: Add merchendise
2: List items in store
3: Remove merchendise
4: Edit merchendise
5: Show stock
6: Replenish
7: Create cart
8: Remove cart
9: Add to cart
10: Remove from cart
11: List carts and their contents
12: Calculate Cost
13: Checkout
0: QUIT


Features
--------

- Creates a warehouse where you can
  * Add/remove merchendise 
  * List the merchendise in the store
  * Edit the name, price and description of the merchendise
  * Replenish the stock of the merchendise in a desired shelf

-Cart functionality in warehouse where you can
  * Create/remove a cart
  * Choose a merchendise to add to/remove from your cart
  * Calculate the cost of all the merchendisein your
  * Checkout the cart

-Testing capabilities using CUnit

	
Requirements
------------
- CUnit
- make

Installation
------------
1. Navigate to where you have fetched the git repo in the terminal
ex. cd /mnt/c/plugg/inlupp2

2. Run the following (seperated by " "): "make ./store" & "make ./test"


4. You are good to go!



Usage
------------

1. For running the store : "./store"

2. For running the tests : "./test"

3. For memory testing of store with inputs run the following : "make ./memtest-store"

4. For memory testing of stores tests : "make ./memtest-test"
