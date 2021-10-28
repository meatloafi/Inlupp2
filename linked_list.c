#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "linked_list.h"
#include "iterator.h"

/**
 * @file linked_list.c
 * @author Felix Agnerdahl and Christian OcklindM
 * @date 7/10-2021
 * @brief A collection of nodes which together represent a linked list.
 *
 * Includes the 'iterator.h' file since a few iterator functions are present in this file.
 *
 * @see $CANVAS_OBJECT_REFERENCE$/assignments/gf5efa1610dfd73b58fef071f6c1d7a90
 */

#define int_elem(x) (elem_t) { .int_value =(x) }

#define func_ptr(x) (elem_t) { .func_point =(x) }

/**
 * @file simple_linked_list.c
 * @author FELIX AGNERDAHL & CHRISTIAN OCKLIND
 * @date
 * @brief A simple linked list that implements parts of the interface
 * of `linked_list.h`. The functions that are not implemented will fail
 * at runtime with an assertion error, printing a message to stderr.
 */

// This is a macro that is used in the functions that are not implemented,
// to have them fail at runtime. You do not have to understand it to use
// this file!
#define NOT_SUPPORTED()                                                            \
    do                                                                             \
    {                                                                              \
        fprintf(stderr, "The simple linked list does not support %s\n", __func__); \
        assert(false);                                                             \
    } while (0)

// The links of the linked list
struct link
{
    elem_t value;
    struct link *next;
};

typedef struct link link_t;


// The list contains a pointer to its first link, and its size
struct list
{
    link_t *head;
    link_t *tail;
    ioopm_eq_function pred_func;
    int32_t size;
};

struct iter
{
  link_t *current;
  ioopm_list_t *list;
};

link_t *link_create(elem_t value, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}

bool value_equiv_int( elem_t index_ignored, elem_t value, elem_t x)
{
  elem_t *other_value_ptr = &x;
  elem_t other_value = *other_value_ptr;
  return value.int_value == other_value.int_value;
}

bool value_equiv_string(elem_t index_ignored, elem_t value, elem_t x)
{
  elem_t *other_value_ptr = &x;
  elem_t other_value = *other_value_ptr;
  return (strcmp(value.func_point, other_value.func_point));
}

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function compare)
{
    ioopm_list_t *result = calloc(1, sizeof(struct list));
    result->pred_func = compare;
    return result;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    assert(list);
    link_t *current = list->head;
    while (current)
    {
        link_t *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    assert(list);
    link_t *new_entry = link_create(value, NULL);

    if (list->head == NULL)
    {
        list->head = new_entry;
        list->size++;
    }
    else
    {
        link_t *current_entry = list->head;
        while (current_entry->next != NULL)
        {
            current_entry = current_entry->next;
        }
        current_entry->next = new_entry;
        list->size++;
    }
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    assert(list);
    list->head = link_create(value, list->head);
    list->size++;
}

void ioopm_linked_list_insert(ioopm_list_t *list, elem_t index, elem_t value)
{
    assert(list);
    if (index.int_value > list->size)
    {
        printf("Error: %s, ", (strerror(EINVAL))); // Return error message EINVAL, if no entry was found
        exit(0);                                   // Exit Success (otherwise we dont return a value for all controls paths)
    }
    else if (list->head == NULL)
    {
        link_t *new_entry = link_create(value, NULL);
        list->head = new_entry;
        list->size++;
    }
    else
    {
        link_t *current_entry = list->head;
        for (int i = 0; i < index.int_value - 1; i++)
        {
            current_entry = current_entry->next;
        }
        link_t *next_entry = current_entry->next;
        link_t *new_entry = link_create(value, next_entry);
        current_entry->next = new_entry;
        list->size++;
    }
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, elem_t index)
{
    assert(list);
    assert(list->head);

    if (index.int_value == 0)
    {
        link_t *tmp = list->head;
        elem_t value = tmp->value;
        list->head = tmp->next;
        free(tmp);
        list->size--;
        return value;
    }

    // index > 0
    assert(list->head);
    link_t *prev = list->head;
    link_t *current = prev->next;
    for (int i = 1; i < index.int_value; i++)
    {
        assert(current);
        prev = current;
        current = current->next;
    }

    prev->next = current->next;
    elem_t value = current->value;
    free(current);
    list->size--;
    return value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, elem_t index)
{
    assert(list);
    assert(list->head);
    link_t *current = list->head;
    for (int i = 0; i < index.int_value; i++)
    {
        current = current->next;
        assert(current);
    }
    return current->value;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element, ioopm_eq_function function)
{
    assert(list);
    elem_t tmp = int_elem(0);
    link_t *cursor = list->head;
    while (cursor)
    {
        if (function(tmp, cursor->value, element))
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

int32_t ioopm_linked_list_size(ioopm_list_t *list)
{
    assert(list);
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    assert(list);
    if (list->head == NULL)
    {
        return true;
    }
    return false;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    elem_t index = int_elem(0);
    assert(list);
    while (ioopm_linked_list_is_empty(list) == false)
    {
        ioopm_linked_list_remove(list, index);
    }
}


bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra)
{
    link_t *current_entry = list->head;
    elem_t index = int_elem(0);
    if (list->head == NULL)
    {
        return false;
    }
    while (current_entry->next != NULL)
    {
        elem_t value = current_entry->value;
        bool result = prop(index, value, extra);
        if (result == false)
        {
            return false;
        }
        current_entry = current_entry->next;
        index.int_value++;
    }
    return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_int_predicate prop, elem_t extra)
{
    link_t *current_entry = list->head;
    elem_t index = int_elem(0);
    if (list->head == NULL)
    {
        return false;
    }
    while (current_entry->next != NULL)
    {
        elem_t value = current_entry->value;
        bool result = prop(index, value, extra);
        if (result == true)
        {
            return result;
        }
        current_entry = current_entry->next;
        index.int_value++;
    }
    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, elem_t arg)
{
    link_t *current_entry = list->head;
    elem_t index = int_elem(0);
    elem_t current_value_ptr;
    while(list->head != NULL && current_entry != NULL)
    {
        current_value_ptr.func_point = &current_entry->value;
        fun(index, current_value_ptr, arg);
        current_entry = current_entry->next;
        index.int_value++;
    }
}

static void replace_list(elem_t index_ignored, elem_t value, elem_t new_value)
{
    int *val = value.func_point;
    *val = new_value.int_value;
}

void ioopm_linked_list_apply_to_all_test(ioopm_list_t *list, elem_t new_value)
{
    ioopm_linked_list_apply_to_all(list, replace_list, new_value);
}

// ITERATOR

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *result = malloc(sizeof(struct iter));
  result->current = list->head;
  return result;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
  return iter->current->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
  if(iter->current->next == NULL)
    {
      printf("Error: %s, ", (strerror(EINVAL))); // Return error message EINVAL, if no entry was found
      exit(0);                                   // Exit Success (otherwise we dont return a value for all controls paths)

    }
  return iter->current->next->value;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter, ioopm_list_t *list)
{
  iter->current = list->head;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
  return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
  free(iter);
}
