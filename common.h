#pragma once
#include <stdbool.h>

/**
 * @file common.h
 * @author David Kidane & Felix Agnerdahl
 * @date --
 * @brief 
 * 
 * 
 * 
 * @see https://uppsala.instructure.com/courses/47154/assignments/75644
 */

union elem
{
  int number;
  void *extra;
};

typedef union elem elem_t;


#define int_elem(x) (elem_t) { .number=(x) }
#define ptr_elem(x) (elem_t) { .extra=(x) }
#define bad_elem (elem_t) { .number=(0), .extra=(NULL) }

typedef bool (*ioopm_predicate)(elem_t key, elem_t value, void *extra);
typedef void (*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

typedef int(*ioopm_hash_function)(elem_t key);


/// @brief //TODO
/// @param a first element being compared
/// @param b second element being compared
/// @return a == b
bool compare_bool(elem_t a, elem_t b);

/// @brief //TODO
/// @param a first element being compared
/// @param b second element being compared
/// @return a == b
bool compare_str(elem_t a, elem_t b);

/// @brief //TODO
/// @param a first element being compared
/// @param b second element being compared
/// @return a == b
//bool compare_ptr(elem_t a, elem_t b);

bool compare_int(elem_t a, elem_t b);