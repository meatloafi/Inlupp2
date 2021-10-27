#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h>
#include "hash_table.h"
#include "list_linked.h"
#include "iterator.h"
#include "common.h"


typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;

#define prime_amount 11

// struct entry
// {
//   elem_t key;       // holds the key
//   elem_t value;   // holds the value
//   entry_t *next; // points to the next entry (possibly NULL)
// };

struct hash_table
{
  size_t size; // size of hashtable
  size_t capacity; // storage capacity for buckets
  float load_factor; // loadfactor for hashtable
  entry_t *buckets; // pointer to buckets in hashtable
  ioopm_hash_function hash_func; // hash function
  ioopm_eq_function compare_key; // key comparison function
  ioopm_eq_function compare_val; // value comparison function
};

static int mod_negative_support(int a, int b)
{
  return ((a % b) + b) % b;
}

int extract_int_hash_key(elem_t key)
{
  return key.number;
}

ioopm_hash_table_t *ioopm_hash_table_create(size_t capacity, float load_factor, ioopm_hash_function hash_func, ioopm_eq_function compare_key, ioopm_eq_function compare_val)
{
  ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
  if (load_factor <= 0)
  {
    result->load_factor = 0.75f;
  }
  else
  {
    result->load_factor = load_factor; //17
  }
  if (capacity <= 0)
  {
    result->capacity = 17; //17
  }
  else
  {
    result->capacity = capacity; //17
  }
  if (hash_func == NULL)
  {
    result->hash_func = extract_int_hash_key;
  }
  else
  {
    result->hash_func = hash_func;
  }
  if (compare_key == NULL)
  {
    result->compare_key = compare_int;
  }
  else
  {
    result->compare_key = compare_key;
  }
  if (compare_val == NULL)
  {
    result->compare_val = compare_str;
  }
  else
  {
    result->compare_val = compare_val;
  }
  result->size = 0;
  result->buckets = calloc(result->capacity, sizeof(entry_t));
  return result;
}



void entry_destroy(entry_t *entry) 
{
  free(entry);
  entry = NULL;
}
///checks if a hash tables needs increased capacity
static size_t find_new_capacity(ioopm_hash_table_t *ht)
{
  size_t primes[] = {17, 31, 67, 127, 257, 509, 1021, 2053, 4099, 8191, 16381, 31991}; //11 primes
  size_t new_capacity = 0;
  
  if (ht->capacity < primes[prime_amount - 1]) //prime_amount is the amount of values in primes[]
  {
    for (int i = 0; i < prime_amount; i++)
      {
        if (primes[i] > ht->capacity) //if prime is larger than capacity
        {
          new_capacity = primes[i]; // set new capacity as chosen prime
          return new_capacity;
        }
      }
  }
  return new_capacity;
}
///creates a new hashtable with adjusted elements 
void hash_table_resize(ioopm_hash_table_t *ht)
{  
  ioopm_hash_table_t *old_ht = ht; //save old ht
  size_t new_capacity = find_new_capacity(ht); //sets new capacity
  //create a new ht with new capacity
  ioopm_hash_table_t *new_ht = ioopm_hash_table_create(new_capacity, old_ht->load_factor, old_ht->hash_func, old_ht->compare_key, old_ht->compare_val);
  
  for (int i = 0; i < ht->capacity; i++)
  {
    entry_t *current_entry = &ht->buckets[i];//sets current entry as first entry of each bucket
    if (current_entry)
    {
      current_entry = current_entry->next; //if current_entry is not NULL then continiue to next entry
      while (current_entry)
      {
        ioopm_hash_table_insert(new_ht, current_entry->key, current_entry->value);//insert elements into hashtable
        current_entry = current_entry->next; //continiue
      }
    } 
  }
   /// free memory and pointers
  ioopm_hash_table_clear(ht);
  free(ht->buckets);
  *ht = *new_ht; // set input ht as the created ht
  free(new_ht);
}


static entry_t *entry_create(const elem_t key, elem_t value, entry_t *next)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t)); //allocates new memory for the entry
  new_entry->key = key; // sets values for new entry
  new_entry->value = value;
  new_entry->next = next;
  return new_entry; //returns new entry
}
///finds previous entry before the searched key
static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *first_entry, const elem_t key)
{
  entry_t *cursor = first_entry; // Cursor to be used to check different entries
  while (cursor->next != NULL) // if entry is empty then previous is returned
  { 
    if (ht->compare_key(cursor->next->key, key)) // if cursor contains same key as input then previous gets returned
      {
        return cursor;
      }
    cursor = cursor->next;
  }
  return first_entry;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, const elem_t key, elem_t value)
{
  /// Calculate the bucket for this entry
  int bucket = mod_negative_support(ht->hash_func(key), ht->capacity);
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next != NULL && (ht->compare_key(next->key, key)))
    {
      next->value = value;
    }
  else
    {
  /// otherwise create the new entry
      entry->next = entry_create(key, value, next);
      ht->size++;
      if (ht->size > (ht->load_factor * ht->capacity))
      {
      hash_table_resize(ht);
      }
    }
}

elem_t ioopm_hash_table_lookup(ioopm_hash_table_t *ht, const elem_t key, bool *result)
{
  /// Calculate the bucket for this entry
  int bucket = mod_negative_support(ht->hash_func(key), ht->capacity);
  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(ht, &ht->buckets[bucket], key);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not
  if (next && ht->compare_key(next->key, key))
  {
    *result = true;
    return(next->value);
  }
  else
  {
    *result = false;
    return (bad_elem);
  }
}


//4.4

/// frees memory of a single entry


/// removes a single entry in a hashtable
void ioopm_hash_table_remove(ioopm_hash_table_t *ht, const elem_t key)
{
  entry_t *tmp = find_previous_entry_for_key(ht, &ht->buckets[key.number % ht->capacity], key);
  entry_t *to_unlink = tmp->next; // entry we want to unlink
  tmp->next = to_unlink->next; // relink previous to to_unlink:s next
  if (to_unlink)
    {
    entry_destroy(to_unlink); // frees memory of that entry
    ht->size--;
    }
  else
  {
    errno = EINVAL;
  }  
}

/// destroys an entire hashtable
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{  
  for (int i = 0; i < ht->capacity; i++) // loop for each bucket in ht
  {
      /// free the memory of all entries in a bucket for each bucket in the hashtable
      entry_t *next;
      entry_t *current_entry = (ht->buckets[i]).next; // first entry in bucket
      while (current_entry != NULL)
      {
        next = current_entry->next; //set current_entry next pointer to next
        entry_destroy(current_entry); 
        current_entry = next; // continiue with next entry
      } 
  }
  free(ht->buckets);
  free(ht); // frees memory of ht
  ht = NULL;
}

//5

/// counts the size of a hashtable i.e how many buckets there exists.
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
  return ht->size;
}


/// returns a boolean depending on wether the hashtable is empty or not
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  return ioopm_hash_table_size(ht) == 0;
  for (int i = 0; i < ht->capacity; i++)
  {
    entry_t *current_entry = (ht->buckets[i]).next; 
    if (current_entry != NULL) //checks if current entry is empty
    {
      return false;
    }
  }
  return true;
}

/// clears the hashtable of all entries except for dummy entries
void ioopm_hash_table_clear(ioopm_hash_table_t *ht) 
{  
  for (int i = 0; i < ht->capacity; i++)
  {
      entry_t *next;
      entry_t *dummy = &ht->buckets[i]; // first entry for each bucket (dummy entry)
      entry_t *current_entry = dummy->next; // second entry for each bucket
      /// frees the memory for each entry in a bucket 
      while (current_entry != NULL)
      {
        next = current_entry->next; // save current entry next pointer
        entry_destroy(current_entry);
        current_entry = next; // continiue with next entry
      }
      dummy->next = NULL; // lastly depoint dummys next pointer
  }
  ht->size = 0;
}

///return the keys for all entries in a hash map (in no particular order, but same as ioopm_hash_table_values)
ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
   ioopm_list_t *key_list = ioopm_linked_list_create(ht->compare_key); //allocate memory for an array of arrays the size of ht
   for (int i = 0; i < ht->capacity; i++)
   {
    entry_t *current_entry = &ht->buckets[i]; //first entry for each bucket
    while (current_entry->next != NULL)
    {
      current_entry = current_entry->next;
      ioopm_linked_list_append(key_list, current_entry->key);
    }
   }
   return key_list; // each string in the array gets its value from current_entry
} 

/// return the values for all entries in a hash map
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
   ioopm_list_t *val_list = ioopm_linked_list_create(ht->compare_val); //allocate memory for an array of arrays the size of ht
   for (int i = 0; i < ht->capacity; i++)
   {
    entry_t *current_entry = (ht->buckets[i]).next; //first entry for each bucket
    while (current_entry)
    {
      ioopm_linked_list_append(val_list, current_entry->value);
      current_entry = current_entry->next;
    }
   }
   return val_list; // each string in the array gets its value from current_entry
}

/// checks if keys are equal
bool key_equiv(ioopm_hash_table_t *ht, const elem_t key, elem_t value_ignored, void *x)
{
  return ht->compare_key(key, *(elem_t *)x);
}

///checks if values are equal
bool value_equiv(ioopm_hash_table_t *ht, const elem_t key_ignored, elem_t value, void *x)
{
  elem_t *valcheck = x;
 return ht->compare_val(value, *valcheck);
}

/// check if a predicate is satisfied by all entries in a hash table
bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate_hash pred, void *arg)
{
    if (ioopm_hash_table_is_empty(ht))
    {
      return false;
    }
    for (int i = 0; i < ht->capacity; i++)
    {
      entry_t *current_entry = (&ht->buckets[i])->next; //first entry in each bucket
      while(current_entry != NULL)
      {
        current_entry = current_entry->next; // continiue to next entry
        if(!(pred(ht, current_entry->key, current_entry->value, arg))) //checks for predicate is true
        {
          return false;
        }
      }
    }
    return true;
}

/// check if a predicate is satisfied by any entry in a hash table
/// works the same as above, key difference being it checks satisfaction for any entry in hashtable
bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate_hash pred, void *arg)
{
    if (ioopm_hash_table_is_empty(ht))
    {
      return false;
    }
    for (int i = 0; i < ht->capacity; i++)
    {
      entry_t *current_entry = &ht->buckets[i];
      while(current_entry->next != NULL)
      {
        current_entry = current_entry->next;
        if(pred(ht, current_entry->key, current_entry->value, arg))
        {
          return true;
        }
      }
    }
    return false;
}

/// apply a function to all entries in a hash table
void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function_hash apply_fun, void *arg)
{
  for (int i = 0; i < ht->capacity; i++)
    {
      entry_t *current_entry = (&ht->buckets[i])->next; // first entry in bucket
      while(current_entry != NULL)
      {
      apply_fun(current_entry->key, &current_entry->value, arg); // apply the function
      current_entry = current_entry->next; // continiue with next
      }
    }
}

void value_insert(elem_t int_ignored, elem_t *value, void *extra)
{
  *value = *(elem_t *)extra;
}

/// checks if hashtable uses given key
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  return ioopm_hash_table_any(ht, key_equiv, &key);
}

/// checks if hastable uses a given value
bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  return ioopm_hash_table_any(ht, value_equiv, &value);
}

/*
void main()
{
  elem_t key_insert = {.number = 1};
  elem_t value_insert = {.extra = "a"};
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  print_ht(ht, true, print_int_elem, print_str_elem);
  ioopm_hash_table_insert(ht, key_insert, value_insert);
  entry_t first_entry = ht->buckets[1];
  elem_t key = first_entry->next->key;
  elem_t val = first_entry->next->value;
  print_ht(ht, true, print_int_elem, print_str_elem);
  ioopm_hash_table_destroy(ht);
}
*/