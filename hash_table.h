#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "common.h"
#pragma once
#define No_buckets ht->capacity

  
/**
 * @file hash_table.h
 * @author Felix Agnerdahl and Christian Ocklind
 * @date
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

// TODO Kolla med en TA om hur vi ska göra med våra typedefs och structs
// om de kan ligga här eller om vi bör flytta på dem
/// ALIASES ///
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

// typedef bool(*ioopm_predicate)(int key, char *value, void *extra);
// typedef void(*ioopm_apply_function)(int key, char **value, void *extra);

typedef bool (*ioopm_predicate)(elem_t key, elem_t value, elem_t extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t value, elem_t extra);

typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file

/// Compares two elements and returns true if they are equal
typedef bool(*ioopm_eq)(elem_t a, elem_t b);

// STRUCTS ///

struct entry
{
  elem_t key;    // holds the key
  elem_t value;  // holds the value
  entry_t *next; // points to the qnext entry (possibly NULL)
};

struct hash_table
{
  ioopm_predicate predicate_ptr;
  ioopm_predicate predicate_int;
  ioopm_eq func;
  float load_factor;
  int32_t capacity;
  entry_t *buckets;
  ioopm_hash_function hash_function;
};


/// @brief Create a new hash table
/// @param hash_function hash function used to sort entries to their designated buckets. 
/// @param
/// @param
/// @param
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq check, ioopm_predicate pred_ptr, ioopm_predicate pred_int);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
//makefile / @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup.
/// @return the value mapped to by key or an error message if no entry was found. Otherwise we return an error message.
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value.
/// @param ht hash table operated upon
/// @param key key to remove.
/// @return the value mapped to by key, if there is a value that we can remove. Otherwise we return an error message.
elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);

//TODO
bool ioopm_ht_value_equiv(elem_t value, elem_t x);

/// @brief Free all the entries in the hash table and the hash table itself
/// @param ht hash table operated upon
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
int32_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *ht);

/// @brief return the keys for all entries in a hash map as a linked list
/// @param h hash table operated upon
/// @return an array of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key, ioopm_predicate pred);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value, ioopm_predicate pred);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg);

/// @brief check if all values in a hash table are equivalent.
/// @param ht hash table.
/// @param value string argument.
/// @return bool true if all values are equivalent, otherwise false.
bool ioopm_hash_table_all_value(ioopm_hash_table_t *ht, elem_t value);

/// @brief check if all keys in a hash table are equivalent.
/// @param ht hash table.
/// @param key int argument.
/// @return bool true if all keys are equivalent, otherwise false.
bool ioopm_hash_table_all_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, elem_t arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, elem_t arg);

/// @brief A function that is only used in the test files to run and test ioopm_hash_table_apply_to_all().
/// @param ht hash table operated upon.
/// @param value the value that is going to replace all the values for all entries in the hash table. 
void ioopm_hash_table_apply_to_all_test(ioopm_hash_table_t *ht, elem_t value);
 
/// @brief Function that hashes the entries that are inserted into the hash table. 
/// @param key The key that is going to be hashed by the function (bucket placement).
/// @param ht Hash table operated upon. 
/// @return A number repsresenting the bucket which the key will be placed in. 

unsigned long ascii_sum(const char *str);

int hash_int(int key);

