#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "list_linked.h"
#include "iterator.h"
#include "common.h"

typedef struct link link_t;

typedef struct list ioopm_list_t;

struct link 
{
  elem_t element; //data    
  link_t *next; //link
};

struct list
{
    link_t *first; //first link
    link_t *last; // last link
    size_t size; // size of link
    ioopm_eq_function compare; //comparison function
};

bool compare_int(elem_t a, elem_t b)
{
    return b.number == a.number;
}

bool compare_str(elem_t a, elem_t b)
{
    return (strcmp((char *)a.extra, (char *)b.extra) == 0);
}

/// creates an empty list
ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function compare)
{
  ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t)); // allocated memory for list
  if (compare == NULL)
  {
    compare = compare_int; //if input is NULL then set compare function to compare ints
  }
  list->compare = compare; //update struct
  return list;
}

/// creates a new link
link_t *link_create(const elem_t element, link_t *next)
{
  link_t *new_link = calloc(1, sizeof(link_t)); //Allocates memory for a new link
  new_link->element = element; // sets element value to the function input
  new_link->next = next;
  return new_link; //returns the new link
}

/// destroys a link
void link_destroy(link_t *link)
{
  free(link);
}

void ioopm_linked_list_prepend(ioopm_list_t *list, const elem_t element)
{
  list->first = link_create(element, list->first); //create a new link with the first link values
  ioopm_linked_list_insert(list, 0, element); // set it as the first link in list
  ++list->size; //update size of list
}

void ioopm_linked_list_append(ioopm_list_t *list, const elem_t element)
{
  link_t *new_link = link_create(element, NULL); //create a new link with pointer to NULL
  link_t *cursor = list->last; // set cursor to last link
  if (ioopm_linked_list_is_empty(list))
  {
    list->first = new_link; // if empty set link as first
  }
  else
  {
    cursor->next = new_link; // else set newlink as cursor next
  }
  list->last = new_link; // set newlink as last link of list
  ++list->size;
}

///inserts a link in a list given an index
void ioopm_linked_list_insert(ioopm_list_t *list, const size_t index, elem_t element)
{
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list); // create iterator of list
  if (list->size >= index && index >= 0)
    {
      for (int i = 0; i <= index; i++)
      {
        ioopm_iterator_next(iter); // move iterator to index
      }
      ioopm_iterator_insert(iter, element); // insert element at index
    }
  else
    {
      errno = EINVAL; // invalid input if index is negative or larger than list size
      ioopm_iterator_destroy(iter);
    }
}



/// calculates the size of the list
size_t ioopm_linked_list_size(ioopm_list_t *list)
{
  return list->size;
}
/// extracts an element at given index
elem_t ioopm_linked_list_get(ioopm_list_t *list, const size_t index)
{
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  if (list->size > index && index >= 0 && !ioopm_linked_list_is_empty(list))
    {
      for (int i = 0; i < index; i++)
      {
        ioopm_iterator_next(iter);
      }
    elem_t element = ioopm_iterator_current(iter);
    ioopm_iterator_destroy(iter);
    return element;
    } 
  else
    {
    errno = EINVAL;
    return (bad_elem);
    }
}


///Removes the indexed link from the list 
elem_t ioopm_linked_list_remove(ioopm_list_t *list, const size_t index)
{
  if ((list->size > index && index >= 0) || (!ioopm_linked_list_is_empty(list)))
  {
    errno = EINVAL;
    return (bad_elem);
  }
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list);
  for (int i = 0; i <= index; i++)
    {
      ioopm_iterator_next(iter);
    }
  elem_t removed = ioopm_iterator_remove(iter);
  ioopm_iterator_destroy(iter);
  return removed;
}

/// checks if list is empty
bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
  if (ioopm_linked_list_size(list) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
/// checks if a supplied property holds for all elements in list
bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate prop, void *arg)
{
  link_t *current_link = list->first; //set current link as first link
  while ((current_link != (list->last)) && (current_link != NULL))
    {
    if(!(prop(current_link->element, current_link->element, arg))) // checks supplied function upon elements until end of list is reached
    {
      return false;
    }
    current_link = current_link->next; // continiue with next
    }
  return true;
}

/// checks if a supplied property holds for any elements in list
bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate prop, void *arg)
{
  link_t *current_link = list->first;
  while ((current_link != (list->last)) && (current_link != NULL))
    {
    if((prop(current_link->element, current_link->element, arg)))
    {
      return true;
    }
    current_link = current_link->next;
    }
  return false;
}

/// checks if a list contains supplied element
bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
  if (ioopm_linked_list_is_empty(list)) // if empty return false
    return false;
  for (link_t *cursor = list->first->next; cursor; cursor = cursor->next)
    {
      if (list->compare(cursor->element, element)) 
      {
        return true;
      };
    }
  return false;
}

/// destroys the entire list
void ioopm_linked_list_destroy(ioopm_list_t *list)
{  
  link_t *next;
  link_t *current_link = list->first; //first link
  while (current_link != (list->last))
  {
    next = current_link->next; // save current_link next pointer
    link_destroy(current_link); // destroy current link
    current_link = next; // continiue with next link
  }
  link_destroy(list->last);
  next = NULL;
  free(list); // free allocated memory
  list = NULL;
}
/// removes all elements in list
void ioopm_linked_list_clear(ioopm_list_t *list)
{
  link_t *next;
  link_t *current_link = list->first;
  while (current_link != (list->last))
  {
    next = current_link->next;
    link_destroy(current_link);
    current_link = next;
  }
}
/// apply a function to all elements in list
void ioopm_linked_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, void *arg)
{
  link_t *current_link = list->first;
  while ((current_link != (list->last)) && (current_link != NULL))
  {
  fun(current_link->element, &current_link->element, arg);
  current_link = current_link->next;
  }
}
/// convert a list into an array
char **convert_list_array(ioopm_list_t *list)
{
  ioopm_list_iterator_t *iter = ioopm_list_iterator(list); /// create iterator for list
  int length = ioopm_linked_list_size(list); // length of list 
  char **array = calloc(length, sizeof(char*)); // allocate memory for size of elements

  array[0] = ioopm_iterator_current(iter).extra;

  for (int i = 1; i < length; i++)
  {
    array[i] = ioopm_iterator_next(iter).extra; // create the array
  }

  ioopm_linked_list_destroy(list);
  ioopm_iterator_destroy(iter);
  return array;
}
