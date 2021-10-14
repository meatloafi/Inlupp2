#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include "list_linked.h"
#include "iterator.h"

struct link 
{
  elem_t element; //data    
  link_t *next; //link
};

struct list
{
    link_t *first; //first link
    link_t *last; //last link
    size_t size; // size of list
    ioopm_eq_function compare; // comparison function
};


struct iterator
{
  link_t *current_link_ptr; //pointer to current link
  ioopm_list_t *list; // pointer to list
};


typedef struct iterator ioopm_list_iterator_t;

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
  ioopm_list_iterator_t *result = calloc(1, sizeof(struct iterator));
  result->current_link_ptr = list->first;
  result->list = list;
  return result; 
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (&iter->current_link_ptr->next)
    {
        return true;
    }
    return false;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{   
    link_t *current_link = (iter->current_link_ptr);
    if(current_link)
    {
        iter->current_link_ptr = current_link->next;
        return ioopm_iterator_current(iter);
    }
    else
    {
    errno = EINVAL;
    return (bad_elem);
    }
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    link_t *current_link = (iter->current_link_ptr);
    if (current_link == iter->list->first)
    {
     ioopm_linked_list_prepend(iter->list, element);
     current_link = iter->list->first;
    }
    else if (current_link == NULL)
    {
      ioopm_linked_list_append(iter->list, element);
      current_link = iter->list->last;
    }
    else
    {
        link_t *link_new = link_create(element, current_link);
        current_link = link_new;
        iter->list->size++;
        link_destroy(link_new);
    }
    ioopm_iterator_destroy(iter);
}

elem_t ioopm_iterator_remove(ioopm_list_iterator_t *iter)
{
  link_t *to_remove = iter->current_link_ptr->next; 
  elem_t result = to_remove->element;

  iter->current_link_ptr->next = to_remove->next; 

  free(to_remove); 

  return result;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current_link_ptr = iter->list->first;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    link_t *current_link = (iter->current_link_ptr);
    if(current_link)
        {
            return current_link->element;
        }
    else
    {
        errno = EINVAL;
        return (bad_elem);
    }
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}
