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

  // ioopm_linked_list_destroy(((merch_t *)merch.func_point)->location);
  // free(((merch_t *)merch.func_point)->location);
  ioopm_warehouse_destroy(warehouse);
}


// void test_ht_to_list() //kanske lite dum, ska komma på nåt sätt att testa list_merch
// {                      //rätt keys finns i listorna men vi behöver få ut value som har infon om merch
//   char *name = "test name";
//   char *name2 = "name";
//   char *desc = "test desc";
//   size_t price = 13;
//   bool result = NULL;

//   warehouse_t *warehouse = ioopm_warehouse_create();

//   result = ioopm_hash_table_is_empty(warehouse->items);
//   CU_ASSERT_TRUE(result);

//   ioopm_add_merch(warehouse, name, desc, price);
//   result = ioopm_hash_table_is_empty(warehouse->items);
//   CU_ASSERT_FALSE(result);

//   ioopm_add_merch(warehouse, name2, desc, price);
//   size_t ht_size = ioopm_hash_table_size(warehouse->items);
//   CU_ASSERT_EQUAL(ht_size, 2);

//   ioopm_list_t *items = ioopm_hash_table_keys(warehouse->items);
//   size_t list_size = ioopm_linked_list_size(items);
//   CU_ASSERT_EQUAL(ht_size, list_size);

//   ioopm_hash_table_lookup(warehouse->items, int_elem(1), &result);
//   CU_ASSERT_TRUE(result);

//   ioopm_hash_table_lookup(warehouse->items, int_elem(2), &result);
//   CU_ASSERT_TRUE(result);

//   result = ioopm_linked_list_contains(items, int_elem(1));
//   CU_ASSERT_TRUE(result);

//   result = ioopm_linked_list_contains(items, int_elem(2));
//   CU_ASSERT_TRUE(result);

//   ioopm_warehouse_destroy(warehouse);
//   ioopm_linked_list_destroy(items);
// }

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

  ioopm_edit_merch(warehouse, name, name_edit, desc_edit, price);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name), &merch_ptr);
  merch = merch_ptr.func_point;
  CU_ASSERT_EQUAL(merch->description, desc_edit);

  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name_edit), &merch_ptr); //ska vara true, key har inte uppdaterats, value rätt men key fel
  CU_ASSERT_TRUE(result);
  result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(name), &merch_ptr); //ska vara false, key har inte uppdaterats, value rätt men key fel
  CU_ASSERT_FALSE(result);

  ioopm_warehouse_destroy(warehouse);
}


void test_replenish()
{
  warehouse_t *warehouse = ioopm_warehouse_create();

  ioopm_add_merch(warehouse, "first item", "test desc", 10);
  ioopm_add_merch(warehouse, "second merchandise", "test desc", 40);
  // cart_t *cart1 = ioopm_cart_create(warehouse);
  // cart_t *cart2 = ioopm_cart_create(warehouse);
  
  ioopm_replenish_stock(warehouse, "first item", "B45", 20);
  ioopm_replenish_stock(warehouse, "first item", "B45", 10);
  ioopm_replenish_stock(warehouse, "second merchandise", "A12", 40);
  ioopm_replenish_stock(warehouse, "second merchandise", "Q90", 10);

  elem_t merch1;
  elem_t merch2;

  ioopm_hash_table_lookup(warehouse->items, ptr_elem("first item"), &merch1);
  ioopm_hash_table_lookup(warehouse->items, ptr_elem("second merchandise"), &merch2);

  merch_t *item1 = merch1.func_point;
  merch_t *item2 = merch2.func_point;

  ioopm_list_t *shelves_item1 = item1->location;
  ioopm_list_t *shelves_item2 = item2->location;


  

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
  ioopm_add_to_cart(warehouse, cart2, "test name", 12);
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
  
  // free(cart); // FIXME: frigör detta i free_all_carts istället
  // free(cart2); // FIXME:
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

  ioopm_remove_from_cart(warehouse,cart, "bord", 12);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("bord"), &merch);
  CU_ASSERT_FALSE(result);

  ioopm_remove_from_cart(warehouse,cart, "stol", 9);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("stol"), &merch);
  CU_ASSERT_TRUE(result);

  result = ioopm_hash_table_is_empty(cart->items);
  CU_ASSERT_FALSE(result);

  ioopm_remove_from_cart(warehouse, cart, "stol", 2);
  result = ioopm_hash_table_lookup(cart->items, ptr_elem("stol"), &merch);
  CU_ASSERT_FALSE(result);  
  
  // free(cart); //FIXME: gör detta i free_all_carts
  ioopm_warehouse_destroy(warehouse);
}


void test_cart_cost()
{
  // elem_t merch;
  // bool result = NULL;

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

  // free(cart); //FIXME:
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
  ioopm_replenish_stock(warehouse, "stol", "b13", 11);
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

  // free(cart);
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

    /*(NULL == CU_add_test(store_test_suite1, "test list merch", test_list_merch))||*/
    
    /*(NULL == CU_add_test(store_test_suite1, "test ht_to_list", test_ht_to_list))||*/
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_remove_merch", test_remove_merch))|| 
  
    (NULL == CU_add_test(store_test_suite1, "test ioopm_edit_merch", test_edit_merch))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_replenish_", test_replenish))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_add_to_cart", test_add_to_cart))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_remove_from_cart", test_remove_from_cart))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_calc_cart_cost", test_cart_cost))||
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_cart_checkout", test_cart_checkout))/*||
    
    (NULL == CU_add_test(HT_test_suite1, "test table empty 2", test_table_empty2))||
    
    (NULL == CU_add_test(HT_test_suite1, "test table clear1", test_table_clear1))||
    
    (NULL == CU_add_test(HT_test_suite1, "test has value1", test_has_value1))|| 
    
    (NULL == CU_add_test(HT_test_suite1, "test has key1", test_has_key1))|| 

    (NULL == CU_add_test(HT_test_suite1, "test apply to all", test_apply_to_all))|| 

    (NULL == CU_add_test(HT_test_suite1, "test resize",  test_resize))*/)

    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}