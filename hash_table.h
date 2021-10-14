#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "list_linked.h"

/**
 * @file hash_table.h
 * @author David Kidane and Ian Nyfelt
 * @date 13 Sep 2021
 * @brief Simple hash table that maps integer keys to string values.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\
 * or by an at symbol @@.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

#define No_Buckets 17

typedef bool (*ioopm_predicate_hash)(ioopm_hash_table_t *ht, elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function_hash)(elem_t key, elem_t *value, void *extra);

int extract_int_hash_key(elem_t key);


/// @brief Create a new hash table
/// @return A new empty hash table
ioopm_hash_table_t *ioopm_hash_table_create(size_t capacity, float load_factor, ioopm_hash_function hash_func, ioopm_eq_function compare_key, ioopm_eq_function compare_val);

/// @brief Delete a hash table and free its memory
/// @param ht a hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief add key => value entry in hash table ht
/// @param ht hash table operated upon
/// @param key key to insert
/// @param value value to insert
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);


/// @brief lookup value for key in hash table ht
/// @param ht hash table operated upon
/// @param key key to lookup
/// @return The Value of the entry being looked up
elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, bool *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table operated upon
/// @param key key to remove
/// @return the value mapped to by key (FIXME: incomplete)
void ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key);


// del 5

/// @brief returns the number of key => value entries in the hash table
/// @param h hash table operated upon
/// @return the number of key => value entries in the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);

/// @brief checks if the hash table is empty
/// @param h hash table operated upon
/// @return true is size == 0, else false
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *h);

/// @brief clear all the entries in a hash table
/// @param h hash table operated upon
void ioopm_hash_table_clear(ioopm_hash_table_t *h);

/// @brief return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
/// @param h hash table operated upon
/// @return an array of keys for hash table h
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *h);

/// @brief return the values for all entries in a hash map (in no particular order, but same as ioopm_hash_table_keys)
/// @param h hash table operated upon
/// @return an array of values for hash table h
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht);

/// @brief check if a hash table has an entry with a given key
/// @param h hash table operated upon
/// @param key the key sought
bool ioopm_hash_table_has_key(ioopm_hash_table_t *h, elem_t key);

/// @brief check if a hash table has an entry with a given value
/// @param h hash table operated upon
/// @param value the value sought
bool ioopm_hash_table_has_value(ioopm_hash_table_t *h, elem_t value);

/// @brief check if a predicate is satisfied by all entries in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate_hash pred, void *arg);

/// @brief check if a predicate is satisfied by any entry in a hash table
/// @param h hash table operated upon
/// @param pred the predicate
/// @param arg extra argument to pred
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate_hash pred, void *arg);

/// @brief apply a function to all entries in a hash table
/// @param h hash table operated upon
/// @param apply_fun the function to be applied to all elements
/// @param arg extra argument to apply_fun
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *h, ioopm_apply_function_hash apply_fun, void *arg);

/// @brief checks if two keys are equal
/// @param ht hash table to be operated upon
/// @param key first key
/// @param x second key
/// @param value_ignored value to be ignored
bool key_equiv(ioopm_hash_table_t *ht, const elem_t key, elem_t value_ignored, void *x);

/// @brief checks if two values are equal
/// @param ht hash table operated upon
/// @param key_ignored key to be ignored
/// @param value first value
/// @param x second value
bool value_equiv(ioopm_hash_table_t *ht, const elem_t key_ignored, elem_t value, void *x);


/// @brief inserts value at into entry
/// @param int_ignored int to be ignored
/// @param value the value to be inserted
/// @param extra extra argument to insert
void value_insert(elem_t int_ignored, elem_t *value, void *extra);

/// @brief readjust the size of hash table
/// @param ht hash table operated upon
void hash_table_resize(ioopm_hash_table_t *ht);