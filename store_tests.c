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
};


// struct shelf
// {
//     char *shelf;
//     int stock;
// };

struct warehouse
{
    ioopm_hash_table_t *items;
    ioopm_hash_table_t *shelves;
    size_t item_count;
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
  ioopm_hash_table_lookup(warehouse->items, ptr_elem("test name"), &result); //id på första tillagda merch -> 1
  CU_ASSERT_TRUE(result);

  ioopm_warehouse_destroy(warehouse);
}

//void test_list_merch() TODO;

// void test_list_merch()
// {
//   warehouse_t *warehouse = ioopm_warehouse_create();
//   ioopm_add_merch(warehouse, "Stick", "Sick", 420);
//   ioopm_list_merch(warehouse);

//   ioopm_warehouse_destroy(warehouse);
// }


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
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, name, desc, price);
  ioopm_add_merch(warehouse, name2, desc, price);

  size_t ht_size = ioopm_hash_table_size(warehouse->items);
  CU_ASSERT_EQUAL(ht_size, 2);

  ioopm_remove_merch(warehouse, name);

  ht_size = ioopm_hash_table_size(warehouse->items);
  CU_ASSERT_EQUAL(ht_size, 1);

  ioopm_hash_table_lookup(warehouse->items, ptr_elem("test name"), &result);
  CU_ASSERT_FALSE(result);

  ioopm_hash_table_lookup(warehouse->items, ptr_elem("name"), &result);
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
  size_t price = 13;
  bool result = NULL;

  warehouse_t *warehouse = ioopm_warehouse_create();
  ioopm_add_merch(warehouse, name, desc, price);

  merch_t *merch = (ioopm_hash_table_lookup(warehouse->items, (elem_t){.extra = name}, &result)).extra;

  ioopm_edit_merch(warehouse, name, name_edit, desc_edit, price);
  merch = (ioopm_hash_table_lookup(warehouse->items, (elem_t){.extra = name_edit}, &result)).extra;
  char *edited_desc = merch->description;
  CU_ASSERT_EQUAL(edited_desc, desc_edit);

  ioopm_hash_table_lookup(warehouse->items, ptr_elem(name), &result);
  CU_ASSERT_FALSE(result);

  ioopm_warehouse_destroy(warehouse);
}
/*
void test_remove1()
{ ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  elem_t valtest = {.number = 1, .extra = "a"};
  ioopm_hash_table_insert(ht, valtest, valtest);
  ioopm_hash_table_remove(ht, valtest);
  bool result = NULL;
  ioopm_hash_table_lookup(ht, valtest, &result);
  CU_ASSERT_FALSE(result);
  ioopm_hash_table_destroy(ht);
}

void test_table_size1()
{ ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  elem_t keytest = {.number = 12};
  elem_t valtest = {.extra = "abc"};
  ioopm_hash_table_insert(ht, keytest, valtest);
  int size = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(size, 1)
  ioopm_hash_table_destroy(ht);
}

void test_table_size2()
{ ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  elem_t valtest = {.extra = "abc"};
  ioopm_hash_table_insert(ht, int_elem(1), valtest);
  ioopm_hash_table_insert(ht, int_elem(2), valtest);
  ioopm_hash_table_insert(ht, int_elem(3), valtest);
  int size = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(size, 3)
  ioopm_hash_table_destroy(ht);
}

void test_table_size3()
{ ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  int size = ioopm_hash_table_size(ht);
  CU_ASSERT_EQUAL(size, 0)
  ioopm_hash_table_destroy(ht);
}

void test_table_empty1()
{ ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  elem_t keytest = {.number = 12};
  elem_t valtest = {.extra = "abc"};
  ioopm_hash_table_insert(ht, keytest, valtest);
  int is_empty = ioopm_hash_table_is_empty(ht);
  CU_ASSERT_FALSE(is_empty);
  ioopm_hash_table_destroy(ht);
}

void test_table_empty2()
{ 
  ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  elem_t keytest = {.number = 12};
  elem_t valtest = {.extra = "abc"};
  ioopm_hash_table_insert(ht, keytest, valtest);
  ioopm_hash_table_remove(ht, keytest);
  int is_empty = ioopm_hash_table_is_empty(ht);
  CU_ASSERT_TRUE(is_empty);
  ioopm_hash_table_destroy(ht);
}

void test_table_clear1()
{ ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  elem_t valtest = {.extra = "abc"};
  ioopm_hash_table_insert(ht, int_elem(1), valtest);
  ioopm_hash_table_insert(ht, int_elem(2), valtest);
  ioopm_hash_table_insert(ht, int_elem(3), valtest);
  ioopm_hash_table_clear(ht);
  bool is_empty = ioopm_hash_table_is_empty(ht);
  CU_ASSERT_TRUE(is_empty);
  ioopm_hash_table_destroy(ht);
}

void test_has_value1()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
   elem_t keytest = {.number = 12};
   elem_t valtest = {.extra = "a"};
   elem_t valtest2 = {.extra = "awdwad"};  
   ioopm_hash_table_insert(ht, keytest, valtest);
   bool result1 = ioopm_hash_table_has_value(ht, valtest);
   bool result2 = ioopm_hash_table_has_value(ht, valtest2);
   CU_ASSERT_TRUE(result1);
   CU_ASSERT_FALSE(result2);
   ioopm_hash_table_destroy(ht);
}

void test_has_key1()
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
   elem_t keytest = {.number = 12};
   elem_t valtest = {.extra = "a"};
   ioopm_hash_table_insert(ht, keytest, valtest);
   CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, keytest));
   CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(3)));
   ioopm_hash_table_destroy(ht);
}

void test_apply_to_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);

  for(int i = 0; i < 17; i++)
  {
  ioopm_hash_table_insert(ht, (elem_t) {.number = i},ptr_elem("s"));
  }
  elem_t valtest = {.extra = "a"};
  bool result = NULL;
  ioopm_hash_table_apply_to_all(ht, value_insert, &valtest);
  for (int i = 0; i < 17; i++)
  {
      ioopm_hash_table_lookup(ht, int_elem(i), &result);
      if (!result)
      {
          break;
      }
  }
  CU_ASSERT_TRUE(result);
  ioopm_hash_table_destroy(ht);
}

void test_resize()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(0,0,NULL,NULL,NULL);
  for(int i = 0; i < 18; i++)
  {
  ioopm_hash_table_insert(ht, (elem_t) {.number = i},ptr_elem("s"));
  }
  //hash_table_resize(ht);
  CU_ASSERT_EQUAL(ht->capacity,31)
  ioopm_hash_table_destroy(ht);
}
*/
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
    
    (NULL == CU_add_test(store_test_suite1, "test ioopm_remove_merch", test_remove_merch))/*|| 
  
    (NULL == CU_add_test(store_test_suite1, "test ioopm_edit_merch", test_edit_merch))||
    
    (NULL == CU_add_test(HT_test_suite1, "test remove1", test_remove1))||
    
    (NULL == CU_add_test(HT_test_suite1, "test table size 1", test_table_size1))||
    
    (NULL == CU_add_test(HT_test_suite1, "test table size 2", test_table_size2))||
    
    (NULL == CU_add_test(HT_test_suite1, "test table size 3", test_table_size3))||
    
    (NULL == CU_add_test(HT_test_suite1, "test table empty 1", test_table_empty1))||
    
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