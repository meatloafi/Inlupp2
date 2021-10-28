#include <stdbool.h>
#pragma once

typedef union  
{
    int int_value;
    unsigned int u;
    bool b;
    float f;
    void *func_point;
} elem_t;

typedef int (*ioopm_hash_function)(elem_t key); // Hash function

#define int_elem(x) (elem_t) { .int_value =(x) }
#define ptr_elem(x) (elem_t) { .func_point =(x) }
#define bad_elem (elem_t) { .int_value=(0), .func_point=(NULL) }

// unsigned long string_sum_hash(const char *str)
// {
//   unsigned long result = 0;
//   do
//     {
//       result += *str;
//     }
//   while (*++str != '\0');
//   return result;
// }

// static bool string_key_equiv_ht( elem_t key, elem_t index_ignored, elem_t x)
// {
//   elem_t *other_value_ptr = &x;
//   elem_t other_value = *other_value_ptr;
//   return (strcmp(key.func_point, other_value.func_point) == 0);
// }

// static bool int_key_equiv_ht( elem_t key, elem_t index_ignored, elem_t x)
// {
//   elem_t *other_value_ptr = &x;
//   elem_t other_value = *other_value_ptr;
//   return key.int_value == other_value.int_value;
// }
