#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include "CUnit/Basic.h"

/**
 * @file hash_table.c
 * @author Felix Agnerdahl and Christian Ocklind
 * @date 7/10-2021
 * @brief A complex data structure for hash tables.
 *
 * Includes 'common.h' since we use the elem_t typedef struct for our type definitions.
 * Includes 'linked_list.h' file since linked lists are used in this file.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

#define int_elem(x) (elem_t) { .int_value =(x) }
#define ptr_elem(x) (elem_t) { .func_point =(x) }

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq check, ioopm_predicate pred_ptr, ioopm_predicate pred_int)
{
  // Allocate space for a ioopm_hash_table_t = 'size' pointers to
  // entry_t's, which will be set to NULL
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  result->hash_function = hash_function;
  result->capacity = 17;
  result->load_factor = 0.75;
  result->buckets = calloc(result->capacity, sizeof(entry_t));
  result->func = check;
  result->predicate_ptr = pred_ptr;
  result->predicate_int = pred_int;
  return result;
}

int hash_int(int key)
{
  return key;
}

unsigned long ascii_sum(const char *str)
{
  unsigned long result = 0;
  do
    {
      result += *str; /// *str is the ASCII value of
    }
  while (*++str != '\0');
  return result;
}

static entry_t *find_previous_entry_for_key(entry_t *bucket, int key_ascii, ioopm_hash_table_t *ht)
{
  ioopm_hash_function hashing = ht->hash_function;
  entry_t *current_entry = bucket->next;
  entry_t *previous_entry = bucket;

  while (current_entry != NULL)                                // if the entry that is being checked is NULL we end the while loop and return
  {                                                            // the entry whose next pointer is pointing to the entry that was being checked
    if (hashing(current_entry->key) == key_ascii)              // if the entry being checked has a key greater or equal to our key we return the
    {                                                          // entry pointing to it
      return previous_entry;
    }
    previous_entry = current_entry;                            // updating current and previous entry
    current_entry = current_entry->next;
  }
  return previous_entry;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *result = calloc(1, sizeof(entry_t));
  result->key = key;
  result->value = value;
  result->next = next;
  return result;
}


// INSERTS DUMMY ENTRIES TO THE NEW ALLOCATED BUCKETS 
static void remaining_dummies(int previous_capacity,ioopm_hash_table_t *ht)
{
  entry_t *dummy = entry_create(int_elem(0), ptr_elem(NULL), NULL);
  while (previous_capacity < ht->capacity)
  {
    ht->buckets[previous_capacity] = *dummy;
    previous_capacity++;
  }
  free(dummy);
}

// CALCUALTES A NEW CAPACITY (PRIME NUMBER) FOR HT DEPENDING ON THE CURRENT CAPACITY
static int new_capacity(ioopm_hash_table_t *ht, int current_capacity)
{
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381}; // Exponential increase from minimum amount buckets
  int i = 1;
  while(i<10)
  {
    if (current_capacity<primes[i])
    {
      return primes[i];
    }
    i++;
  }
  return primes[i];
}

// CHECKS IF THE LOAD FACTOR IS EXCEEDED, IF EXCEEDED THE FUNCTION UPDATES THE HT WITH NEW AMOUNT OF BUCKETS
static void check_ht_capacity(ioopm_hash_table_t *ht)
{
  float no_buckets = ht->capacity;                                      // NUMBER OF BUCKETS
  int32_t updated_capacity = new_capacity(ht,no_buckets);         // NEXT CAPICITY IF THE LOAD FACTOR IS EXCEEDED                                          
  float current_ht_size = ioopm_hash_table_size(ht);                    // NUMBER OF ENTRIES OF CURRENT HT
  float current_load_factor = current_ht_size/no_buckets;

  if( current_load_factor > ht->load_factor)
  {
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    ioopm_list_t *values = ioopm_hash_table_values(ht);

    ioopm_hash_table_clear(ht);                                         // CLEAR OLD HT

    ht->capacity = updated_capacity;
    ht->buckets = realloc(ht->buckets, updated_capacity * sizeof(entry_t));
    
    remaining_dummies(no_buckets, ht);                                  // INSERT NEW DUMMY ENTRIES TO ONLY THE NEW BUCKETS! entry_create(0, NULL, NULL)
    // REHASH HERE 
    elem_t i = int_elem(0);
    while(i.int_value < current_ht_size)
    {
      elem_t key = ioopm_linked_list_get(keys, i);
      elem_t value = ioopm_linked_list_get(values, i);
      ioopm_hash_table_insert(ht,key,value);
      i.int_value++;
    }
    ioopm_linked_list_destroy(keys);
    ioopm_linked_list_destroy(values);
  }
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  ioopm_hash_function hashing = ht->hash_function;
  int key_ascii = hashing(key);
  int bucket =  key_ascii % ht->capacity;                                           // Calculate 
  entry_t *entry = find_previous_entry_for_key(&ht->buckets[bucket], key_ascii,ht); // Search for an existing entry for a key
  entry_t *next = entry->next;
  if (next != NULL && hashing(next->key) == key_ascii)                              // Check if the next entry should be updated or not
  {
    next->value = value;                                                            // If key is already in use, replace value of said key
  }
  else
  {
    entry->next = entry_create(key, value, next);                                   // If key is not in use, create an entry with with key
  }
  check_ht_capacity(ht);
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result)
{
  ioopm_hash_function hashing = ht->hash_function;
  // Find the previous entry for key
  int key_ascii = hashing(key);
  int bucket = key_ascii % ht->capacity;                                          // Bucket is chosen acording to the hash function in the hash table
  entry_t *tmp = find_previous_entry_for_key(&ht->buckets[bucket], key_ascii,ht); // We chose the second option to add a dummy entry
  entry_t *next = tmp->next;

  if (next && hashing(next->key) == key_ascii)
  {
    *result = next->value;
    return (true);                                                         // If entry was found, return its value...
  }
  else
  {
    return (false);
  }
}

static void entry_destroy(entry_t *entry)
{
  free(entry); // FREE THE MEMORY OF ENTRY
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  ioopm_hash_function hashing = ht->hash_function;
  int key_ascii = hashing(key);
  int bucket = key_ascii % ht->capacity; 
  elem_t value;
  ioopm_hash_table_lookup(ht, key, &value);                                              // Get string of entry that will be removed
  entry_t *previous_entry = find_previous_entry_for_key(&ht->buckets[bucket], key_ascii, ht);   // Get previous entry
  entry_t *current_entry = previous_entry->next;                                                // Current entry (what we will remove)
  
  if (current_entry->next == NULL)                                                              // If there is no next_entry
  {
    previous_entry->next = NULL;              // Set previous_entry next to NULL since current_entry next is NULL
    entry_destroy(current_entry);
    return value;                             // Return string of the entry that wwe removed
  }
  previous_entry->next = current_entry->next; // Set previous_entry next pointer to point to the next_entry
  entry_destroy(current_entry);               // Free current_entry
  return value;                               // Return the value/string of the removed entry (current_entry)
}


int32_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  assert(ht);              // Terminate if ht is NULL
  int32_t c = 0;           // Set the counter to zero
  int i = 0;               // Set bucket index to zero
  while (i < ht->capacity) // Iterate over the buckets
  {
    entry_t *ht_buckets = ht->buckets; // Array of buckets from ht
    entry_t *current_bucket = &ht_buckets[i];
    entry_t *current_entry = current_bucket->next;
    while (current_entry != NULL)      // Check if there is another entry in the bucket
    {
      entry_t *next_entry = current_entry->next;
      current_entry = next_entry;
      c++; // Increment the counter
    }
    i++;   // Increment bucket index
  }
  return c;// Return size
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  int i = 0;               // Set bucket index to zero
  while (i < ht->capacity) // Check all buckets
  {
    entry_t *ht_buckets = ht->buckets;        // Array of buckets
    entry_t *current_bucket = &ht_buckets[i]; // The bucket of the current index
    entry_t *current_entry = current_bucket->next;
    if (current_entry != NULL)
    {
      return false; // If we find an entry, return false
    }
    i++;
  }
  return true;      // Return true if all buckets are empty
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  int i = 0;                                       // Bucket index
  while (i < ht->capacity)
  {
    entry_t *ht_buckets = ht->buckets;             // Buckets in ht
    entry_t *current_bucket = &ht_buckets[i];      // Bucket at index i
    entry_t *current_entry = current_bucket->next; // Skip the first entry since we have a dummy entry at the start of  each bucket
    entry_t *next_entry;
    while (current_entry != NULL)       // If the next value is NULL then there are no more entries
    {
      next_entry = current_entry->next; // Save the next pointer of the entry we are going to remove
      entry_destroy(current_entry);     // Free current_entry
      current_entry = next_entry;       // Set current_entry equal to the next entry in the bucket
    }
    current_bucket->next = NULL;        // Stops the dummy from pointing to where the first entry once was
    i++;                                // Increment index of buckets
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht);
  free(ht->buckets);
  free(ht);
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  int i = 0;
  elem_t index = int_elem(0);
  while (i < ht->capacity)
  {
    entry_t *ht_buckets = ht->buckets;
    entry_t *current_bucket = &ht_buckets[i];      // Bucket at index i
    entry_t *current_entry = current_bucket->next; // Skip the first entry since we have a dummy entry at the start of  each bucket
    entry_t *next_entry;
    while (current_entry != NULL)       // If the next value is NULL then there are no more entries
    {
      next_entry = current_entry->next; // Save the next  pointer of the entry we are going to clear
      elem_t insert = current_entry->key;
      ioopm_linked_list_insert(list, index, insert);
      index.int_value++;
      current_entry = next_entry;       // Set current_entry equal to the next entry in the bucket
    }
    i++;                                // Increment index of buckets
  }
  return list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(NULL);
  int i = 0;
  elem_t array_index = int_elem(0);
  while (i < ht->capacity)
  {
    entry_t *ht_buckets = ht->buckets;
    entry_t *current_bucket = &ht_buckets[i];      // Bucket at index i
    entry_t *current_entry = current_bucket->next; // Skip the first entry since we have a dummy entry at the start of  each bucket
    entry_t *next_entry;
    while (current_entry != NULL)       // If the next value is NULL then there are no more entries
    {
      next_entry = current_entry->next; // Save the next  pointer of the entry we are going to clear
      elem_t insert = current_entry->value;
      ioopm_linked_list_insert(list, array_index, insert);
      array_index.int_value++;
      current_entry = next_entry;       // Set current_entry equal to the next entry in the bucket
    }
    i++; // Increment index of buckets
  }
  return list;
}

bool ioopm_ht_value_equiv(elem_t value, elem_t x)
{
  char *val1 = (char *)value.func_point;
  char *val2 = (char *)x.func_point;
  return (strcmp(val1, val2) == 0);
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg)
{
  int32_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  bool result = true;
  for (elem_t i = int_elem(0); i.int_value < size && result; ++i.int_value) // IF RESULT IS FALSE AT ANY LOOP THEN WE TERMINATE THE LOOP
  {
    result = result && pred(ioopm_linked_list_get(keys, i), ioopm_linked_list_get(values, i), arg);
    if (result == false)
    {
      ioopm_linked_list_destroy(keys);
      ioopm_linked_list_destroy(values);
      return result;
    }
  }
  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);
  return result;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg)
{
  int32_t size = ioopm_hash_table_size(ht);
  ioopm_list_t *keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *values = ioopm_hash_table_values(ht);
  bool result;
  elem_t i = int_elem(0);
  while (i.int_value < size)
  {
    elem_t key = ioopm_linked_list_get(keys, i);
    elem_t value = ioopm_linked_list_get(values, i);
    result = pred(key, value, arg);
    if (result == true)
    {
      ioopm_linked_list_destroy(keys);
      ioopm_linked_list_destroy(values);
      return true;
    }
    i.int_value++;
  }
  ioopm_linked_list_destroy(keys);
  ioopm_linked_list_destroy(values);
  return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t arg)
{
  int i = 0;
  while (i < ht->capacity)
  {
    entry_t *ht_buckets = ht->buckets;
    entry_t *current_bucket = &ht_buckets[i];
    entry_t *current_entry = current_bucket->next;
    elem_t current_value_ptr;
    elem_t current_key;
    while (current_entry != NULL)
    {
      current_key = current_entry->key;
      current_value_ptr.func_point = &current_entry->value;
      apply_fun(current_key, current_value_ptr, arg);
      current_entry = current_entry->next;
    }
    i++;
  }
}

static bool int_key_equiv_ht( elem_t key, elem_t index_ignored, elem_t x)
{
  elem_t *other_value_ptr = &x;
  elem_t other_value = *other_value_ptr;
  return key.int_value == other_value.int_value;
}

static bool string_value_equiv_ht(elem_t index_ignored, elem_t value, elem_t x)
{
  elem_t *other_value_ptr = &x;
  elem_t other_value = *other_value_ptr;
  return (strcmp(value.func_point, other_value.func_point) == 0);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key, ioopm_predicate pred)
{
  return ioopm_hash_table_any(ht, pred, key);
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value, ioopm_predicate pred)
{
  return ioopm_hash_table_any(ht, pred, value);
}

bool ioopm_hash_table_all_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_all(ht, int_key_equiv_ht, key);
}

bool ioopm_hash_table_all_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_all(ht, string_value_equiv_ht, value);
}

static void replace_ht(elem_t key, elem_t value, elem_t new_value)
{
  char **val = value.func_point;
  *val = (char *) new_value.func_point;
}

void ioopm_hash_table_apply_to_all_test(ioopm_hash_table_t *ht, elem_t value)
{
  ioopm_hash_table_apply_to_all(ht, replace_ht, value);
}