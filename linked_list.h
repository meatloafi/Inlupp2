#include <stdint.h>
#include <stdbool.h>
#include "common.h"
#pragma once

/**
 * @file linked_list.h
 * @author Felix Agnerdahl and Christian OcklindM
 * @date 7/10-2021
 * @brief Linked list that store data in nodes.
 *
 * Includes 'common.h' since the elem_t typedef struct is used for the functions in this file.
 * 
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

typedef struct list ioopm_list_t; /// Meta: struct definition goes in C file

// typedef bool(*ioopm_int_predicate)(int key, int value, void *extra);
// typedef void(*ioopm_apply_int_function)(int key, int *value, void *extra);

typedef bool (*ioopm_int_predicate)(elem_t key, elem_t value, elem_t extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t value, elem_t extra);

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b, elem_t c);

bool value_equiv_int(elem_t index_ignored, elem_t value, elem_t x);

/// @brief Creates a new empty list
/// @param func function that determines equality of two values
/// @return an empty linked list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function func);

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_linked_list_destroy(ioopm_list_t *list);

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_linked_list_append(ioopm_list_t *list, elem_t value);

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value);

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// If you choose an invalid index, prints an error message.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted 
void ioopm_linked_list_insert(ioopm_list_t *list, elem_t index, elem_t value);

/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
elem_t ioopm_linked_list_remove(ioopm_list_t *list, elem_t index);

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
elem_t ioopm_linked_list_get(ioopm_list_t *list, elem_t index);
elem_t ioopm_linked_list_get2(ioopm_list_t *list, const size_t index);

/// @brief Test if an element is in the list
/// @param list the linked list
/// @param element the element sought
/// @return true if element is in the list, else false
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element);

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
int32_t ioopm_linked_list_size(ioopm_list_t *list);

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_linked_list_is_empty(ioopm_list_t *list);

/// @brief Remove all elements from a linked list
/// @param list the linked list
void ioopm_linked_list_clear(ioopm_list_t *list);

/// @brief Test if a supplied property holds for all elements in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested (function pointer)
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for all elements in the list, else false
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra);

/// @brief Test if a supplied property holds for any element in a list.
/// The function returns as soon as the return value can be determined.
/// @param list the linked list
/// @param prop the property to be tested
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of prop
/// @return true if prop holds for any elements in the list, else false
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra);

/// @brief Apply a supplied function to all elements in a list.
/// @param list the linked list
/// @param fun the function to be applied
/// @param extra an additional argument (may be NULL) that will be passed to all internal calls of fun
void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, elem_t extra);

/// @brief A function that is only used in the test files to run and test ioopm_linked_list_apply_to_all().
/// @param list The linked list operated upon.
/// @param value The value that is going to be replace all the values for all entries in the linked list. 
void ioopm_linked_list_apply_to_all_test(ioopm_list_t *list, elem_t new_value);

bool value_equiv_int(elem_t index_ignored, elem_t value, elem_t x);

bool value_equiv_string(elem_t index_ignored, elem_t value, elem_t x);

