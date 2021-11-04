#include <string.h>
#include <stdbool.h>
#include <CUnit/Basic.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "business.h"

// typedef struct entry entry_t;
// typedef struct hash_table ioopm_hash_table_t;

// struct entry
// {
//   elem_t key;       // holds the key
//   elem_t value;   // holds the value
//   entry_t *next; // points to the next entry (possibly NULL)
// };

// struct hash_table
// {
//   size_t size;
//   size_t capacity;
//   float load_factor;
//   entry_t *buckets;
//   ioopm_hash_function hash_func;
//   ioopm_eq_function compare_key;
//   ioopm_eq_function compare_val;
// };

struct merch 
{
    int id;
    char *name;
    size_t price;
    char *description;
    ioopm_list_t *location;
    size_t total_stock;
    size_t unused_stock;
    bool lock;
};


struct shelf
{
    char *shelf;
    char *item_in_shelf;
    size_t stock;
};

struct warehouse
{
    ioopm_hash_table_t *items;
    ioopm_hash_table_t *shelves;
    ioopm_list_t *carts;
    size_t item_count;
    size_t cart_count;
};

struct cart
{
    ioopm_hash_table_t *items;
    int id;
};


// begin test

int init_suite(void)
{
  return 0;
}

int clean_suite(void)
{
  return 0;
}

void test_add_merch()
{
  char *name = "test name";
  char *desc = "test desc";
  size_t price = 13;
  elem_t merch;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();

  result = ioopm_hash_table_is_empty(warehouse->items);
  CU_ASSERT_TRUE(result);
  result = ioopm_hash_table_is_empty(warehouse->shelves);
  CU_ASSERT_TRUE(result);

  ioopm_add_merch(warehouse, name, desc, price);
  result = ioopm_hash_table_is_empty(warehouse->items);
  CU_ASSERT_FALSE(result);

  result = NULL;
  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem("test name"), &merch); //id på första tillagda merch -> 1
  CU_ASSERT_TRUE(result);

  ioopm_warehouse_destroy(warehouse);
}


void test_remove_merch()
{
  char *name = "test name";
  char *name2 = "name";
  char *desc = "test desc";
  size_t price = 13;
  elem_t merch;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, name, desc, price);
  ioopm_add_merch(warehouse, name2, desc, price);

  size_t ht_size = ioopm_hash_table_size(warehouse->items);
  CU_ASSERT_EQUAL(ht_size, 2);

  ioopm_remove_merch(warehouse, name);

  ht_size = ioopm_hash_table_size(warehouse->items);
  CU_ASSERT_EQUAL(ht_size, 1);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem("test name"), &merch);
  CU_ASSERT_FALSE(result);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem("name"), &merch);
  CU_ASSERT_TRUE(result);

  result = ioopm_hash_table_is_empty(warehouse->items);
  CU_ASSERT_FALSE(result);

  ioopm_remove_merch(warehouse, name2);

  result = ioopm_hash_table_is_empty(warehouse->items);
  CU_ASSERT_TRUE(result);

  ioopm_warehouse_destroy(warehouse);
}

void test_edit_merch()
{
  char *name = "test name";
  char *name_edit = "name";
  char *desc = "test desc";
  char *desc_edit = "edited desc";
  merch_t *merch;
  elem_t merch_ptr;
  size_t price = 13;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, name, desc, price);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name), &merch_ptr);
  merch = merch_ptr.func_point;
  CU_ASSERT_FALSE(strcmp(merch->description, desc_edit) == 0);
  CU_ASSERT_TRUE(strcmp(merch->description, desc) == 0);

  ioopm_edit_merch(warehouse, name, name_edit, desc_edit, price);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name), &merch_ptr);
  CU_ASSERT_FALSE(result);
  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name_edit), &merch_ptr); //ska vara true, key har inte uppdaterats, value rätt men key fel
  CU_ASSERT_TRUE(result);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name_edit), &merch_ptr);
  merch = merch_ptr.func_point;
  CU_ASSERT_FALSE(strcmp(merch->description, desc) == 0);
  CU_ASSERT_TRUE(strcmp(merch->description, desc_edit) == 0);
  CU_ASSERT_FALSE(strcmp(merch->name, name) == 0);
  CU_ASSERT_TRUE(strcmp(merch->name, name_edit) == 0);

  ioopm_edit_merch(warehouse, name_edit, name, desc, price);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name), &merch_ptr);
  merch = merch_ptr.func_point;
  CU_ASSERT_FALSE(strcmp(merch->description, desc_edit) == 0);
  CU_ASSERT_TRUE(strcmp(merch->description, desc) == 0);
  CU_ASSERT_FALSE(strcmp(merch->name, name_edit) == 0);
  CU_ASSERT_TRUE(strcmp(merch->name, name) == 0);

  ioopm_warehouse_destroy(warehouse);
}


void test_replenish()
{
  warehouse_t *warehouse = ioopm_warehouse_create();

  ioopm_add_merch(warehouse, "first item", "test desc", 10);
  ioopm_add_merch(warehouse, "second merchandise", "test desc", 40);
  
  ioopm_replenish_stock(warehouse, "first item", "B45", 20);
  ioopm_replenish_stock(warehouse, "first item", "B45", 10);
  ioopm_replenish_stock(warehouse, "second merchandise", "A12", 50);

  elem_t merch1_elem, merch2_elem, shelf1_elem, shelf2_elem, shelf3_elem;

  ioopm_hash_table_lookup(warehouse->items, ptr_elem("first item"), &merch1_elem);
  ioopm_hash_table_lookup(warehouse->items, ptr_elem("second merchandise"), &merch2_elem);

  merch_t *item1 = merch1_elem.func_point;
  merch_t *item2 = merch2_elem.func_point;
  ioopm_list_t *shelves_item1 = item1->location;
  ioopm_list_t *shelves_item2 = item2->location;
  elem_t shelf_item1 = ioopm_linked_list_get(shelves_item1, int_elem(0));
  elem_t shelf_item2 = ioopm_linked_list_get(shelves_item2, int_elem(0));
  char *shelf1 = shelf_item1.func_point;
  char *shelf2 = shelf_item2.func_point;
  size_t quantity1 = item1->total_stock;
  size_t quantity2 = item2->total_stock;

  CU_ASSERT_TRUE(strcmp(shelf1, "B45") == 0);
  CU_ASSERT_TRUE(strcmp(shelf2 , "A12") == 0);
  CU_ASSERT_EQUAL(quantity1, 30);
  CU_ASSERT_EQUAL(quantity2, 50);

  ioopm_replenish_stock(warehouse, "second merchandise", "Q90", 50);
  shelf_item2 = ioopm_linked_list_get(shelves_item2, int_elem(0));
  quantity2 = item2->total_stock;
  CU_ASSERT_EQUAL(quantity2, 100);

  ioopm_hash_table_lookup(warehouse->shelves, ptr_elem("B45"), &shelf1_elem);
  ioopm_hash_table_lookup(warehouse->shelves, ptr_elem("A12"), &shelf2_elem);
  ioopm_hash_table_lookup(warehouse->shelves, ptr_elem("Q90"), &shelf3_elem);

  shelf_t *shelf_1 = shelf1_elem.func_point;
  shelf_t *shelf_2 = shelf2_elem.func_point;
  shelf_t *shelf_3 = shelf3_elem.func_point;

  CU_ASSERT_TRUE(strcmp(shelf_1->shelf, "B45") == 0);
  CU_ASSERT_TRUE(strcmp(shelf_2->shelf, "A12") == 0);
  CU_ASSERT_TRUE(strcmp(shelf_3->shelf, "Q90") == 0);
  CU_ASSERT_EQUAL(shelf_1->stock, 30);
  CU_ASSERT_EQUAL(shelf_2->stock, 50);
  CU_ASSERT_EQUAL(shelf_3->stock, 50);
  CU_ASSERT_TRUE(strcmp(shelf_1->item_in_shelf, "first item") == 0);
  CU_ASSERT_TRUE(strcmp(shelf_2->item_in_shelf, "second merchandise") == 0);
  CU_ASSERT_TRUE(strcmp(shelf_3->item_in_shelf, "second merchandise") == 0);

  ioopm_warehouse_destroy(warehouse);
}


void test_add_to_cart()
{
  elem_t merch;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, "test name", "test desc", 13);
  ioopm_add_merch(warehouse, "test name2", "test desc", 13);

  cart_t *cart = ioopm_cart_create(warehouse);
  cart_t *cart2 = ioopm_cart_create(warehouse);

  ioopm_replenish_stock(warehouse, "test name", "b12", 15);
  ioopm_replenish_stock(warehouse, "test name2", "b13", 11);
  ioopm_add_to_cart(warehouse, cart, "test name", 12);
  ioopm_add_to_cart(warehouse, cart2, "test name", 2);
  ioopm_add_to_cart(warehouse, cart2, "test name2", 11);

  result = ioopm_hash_table_is_empty(cart->items);
  CU_ASSERT_FALSE(result);

  result = ioopm_hash_table_lookup(cart->items, ptr_elem("test name"), &merch);
  CU_ASSERT_TRUE(result);
  result = ioopm_hash_table_lookup(cart2->items, ptr_elem("test name"), &merch);
  CU_ASSERT_TRUE(result);
  result = ioopm_hash_table_lookup(cart2->items, ptr_elem("test name2"), &merch);
  CU_ASSERT_TRUE(result);

  result = ioopm_hash_table_lookup(cart->items, ptr_elem("test not present in cart"), &merch);
  CU_ASSERT_FALSE(result);

  ioopm_warehouse_destroy(warehouse);
}


void test_remove_from_cart()
{
  elem_t merch;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, "bord", "trä", 13);
  ioopm_add_merch(warehouse, "stol", "trä", 13);

  cart_t *cart = ioopm_cart_create(warehouse);

  ioopm_replenish_stock(warehouse, "bord", "b12", 15);
  ioopm_replenish_stock(warehouse, "stol", "b13", 11);
  ioopm_add_to_cart(warehouse, cart, "bord", 12);
  ioopm_add_to_cart(warehouse, cart, "stol", 11);

  result = ioopm_hash_table_lookup(cart->items, ptr_elem("bord"), &merch);
  CU_ASSERT_TRUE(result);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("stol"), &merch);
  CU_ASSERT_TRUE(result);

  ioopm_remove_from_cart(warehouse, cart, "bord", 12);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("bord"), &merch);
  CU_ASSERT_FALSE(result);

  ioopm_remove_from_cart(warehouse, cart, "stol", 9);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("stol"), &merch);
  CU_ASSERT_TRUE(result);

  result = ioopm_hash_table_is_empty(cart->items);
  CU_ASSERT_FALSE(result);

  ioopm_remove_from_cart(warehouse, cart, "stol", 2);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("stol"), &merch);
  CU_ASSERT_FALSE(result);  
  
  ioopm_warehouse_destroy(warehouse);
}


void test_cart_cost()
{
  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, "bord", "trä", 13);
  ioopm_add_merch(warehouse, "stol", "trä", 13);

  cart_t *cart = ioopm_cart_create(warehouse);

  ioopm_replenish_stock(warehouse, "bord", "b12", 15);
  ioopm_replenish_stock(warehouse, "stol", "b13", 11);
  ioopm_add_to_cart(warehouse, cart, "bord", 12);
  ioopm_add_to_cart(warehouse, cart, "stol", 11);

  size_t total_price = ioopm_calc_cost_cart(warehouse, cart);
  CU_ASSERT_EQUAL(total_price, 299);

  ioopm_warehouse_destroy(warehouse);
}

void test_cart_checkout()
{
  elem_t merch;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, "bord", "trä", 13);
  ioopm_add_merch(warehouse, "stol", "trä", 13);

  cart_t *cart = ioopm_cart_create(warehouse);

  ioopm_replenish_stock(warehouse, "bord", "b12", 15);
  ioopm_replenish_stock(warehouse, "stol", "b15", 11);
  ioopm_add_to_cart(warehouse, cart, "bord", 12);
  ioopm_add_to_cart(warehouse, cart, "stol", 11);

  ioopm_checkout_cart(warehouse, cart);

  ioopm_hash_table_lookup(warehouse->items, ptr_elem("bord"), &merch);
  merch_t *merch_info = merch.func_point;
  size_t merch_total_stock = merch_info->total_stock;
  CU_ASSERT_EQUAL(merch_total_stock, 3);

  ioopm_list_t *shelves = merch_info->location;

  char *shelf_name = ioopm_linked_list_get(shelves, int_elem(0)).func_point; //b12
  elem_t shelf_elem;
  ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(shelf_name), &shelf_elem);


  shelf_t *current_shelf = shelf_elem.func_point;
  int shelf_stock = current_shelf->stock;
  CU_ASSERT_EQUAL(shelf_stock, 3);

  size_t size = ioopm_linked_list_size(shelves);
  CU_ASSERT_EQUAL(size, 1);

  ioopm_hash_table_lookup(warehouse->items, ptr_elem("stol"), &merch);
  merch_info = (merch_t *)merch.func_point;
  shelves = merch_info->location;
  result = ioopm_linked_list_contains(shelves, ptr_elem("b12"));
  CU_ASSERT_FALSE(result);
  
  size = ioopm_linked_list_size(shelves);
  CU_ASSERT_EQUAL(size, 0);

  ioopm_remove_cart(warehouse->carts, 1);
  // size_t carts_quantity = ioopm_linked_list_size(warehouse->carts);
  // CU_ASSERT_EQUAL(carts_quantity, 0);

  ioopm_warehouse_destroy(warehouse);

}

// end test

int main()
{
  CU_pSuite store_test_suite1 = NULL;

  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  store_test_suite1 = CU_add_suite("Store Suite", init_suite, clean_suite);
  if (NULL == store_test_suite1)
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  if 
    ((NULL == CU_add_test(store_test_suite1, "test ioopm_add_merch", test_add_merch))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_remove_merch", test_remove_merch))|| 
  
    (NULL == CU_add_test(store_test_suite1, "test ioopm_edit_merch", test_edit_merch))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_replenish_", test_replenish))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_add_to_cart", test_add_to_cart))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_remove_from_cart", test_remove_from_cart))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_calc_cart_cost", test_cart_cost))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_cart_checkout", test_cart_checkout)))

    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}