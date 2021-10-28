#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h>
#include <ctype.h> 
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"
#include "common.h"
#include "business.h"
#include "utils.h"

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct warehouse warehouse_t;


struct merch 
{
    int id;
    char *name;
    size_t price;
    char *description;
    ioopm_list_t *location;
};


struct shelf
{
    char *shelf;
    int stock;
};

struct warehouse
{
    ioopm_hash_table_t *items;
    ioopm_hash_table_t *shelves;
    size_t item_count;
};

unsigned long string_sum_hash(const char *str)
{
  unsigned long result = 0;
  do
    {
      result += *str;
    }
  while (*++str != '\0');
  return result;
}

static bool string_value_equiv_ht(elem_t index_ignored, elem_t value, elem_t x)
{
  elem_t *other_value_ptr = &x;
  elem_t other_value = *other_value_ptr;
  return (strcmp(value.func_point, other_value.func_point) == 0);
}

warehouse_t *ioopm_warehouse_create()
{
    ioopm_hash_table_t *items = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, NULL, string_value_equiv_ht, NULL);
    ioopm_hash_table_t *shelves = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, NULL, NULL, NULL);
    warehouse_t *warehouse = calloc(1, sizeof(warehouse_t));
    warehouse->items = items;
    warehouse->shelves = shelves;
    warehouse->item_count = 0;
    return warehouse;
}

void ioopm_warehouse_destroy(warehouse_t *warehouse)
{
    ioopm_hash_table_destroy(warehouse->items);
    ioopm_hash_table_destroy(warehouse->shelves);
    free(warehouse);
}

//ask_question_shelf("Shelf: ");
merch_t *create_merch(int id, char *name, char *desc, size_t price)
{
    merch_t *merchandise = calloc(1, sizeof(merch_t));
    merchandise -> id = id;
    merchandise -> name = name;
    merchandise -> description = desc;
    merchandise -> price = price;
    merchandise -> location = ioopm_linked_list_create(NULL);
    return merchandise;
}

shelf_t *create_shelf(char *shelf_name)
{
    shelf_t *result = calloc(1, sizeof(shelf_t));
    result->shelf = shelf_name;
    result->stock = 0;
    return result;
}


bool ioopm_add_merch(warehouse_t *warehouse, char *name, char *desc, size_t price) //success shown by bool value
{
    if(ioopm_hash_table_has_key(warehouse->items, ptr_elem(name), warehouse->items->predicate_ptr))
    {
        return false;
    }
    else
    {
        warehouse->item_count++;
        merch_t *new_merch = create_merch(warehouse->item_count, name, desc, price);
        ioopm_hash_table_insert(warehouse->items, ptr_elem(name), ptr_elem(new_merch));
        return true;
    }   
}


void print_merch (merch_t *merch)
{
    printf (" Name: %s \n Desc: %s \n Price: %ld \n", merch -> name, merch -> description, merch -> price);
}


void ioopm_list_merch(warehouse_t *warehouse) //TODO; merch kommer in som den ska i HT men när vi gör en lista med den keyn får vi bara med key och där finns endast id, name,desc,price finns i value
{
    ioopm_list_t *item_values = ioopm_hash_table_values(warehouse->items);

    if (ioopm_hash_table_is_empty(warehouse->items))
    {
        printf("The warehouse is empty \n");
    }
    else
    {
        char *input;
        int size = ioopm_linked_list_size(item_values);
        
        for (int i = 0; i < size; ++i)
        {
            
            elem_t current_value = ioopm_linked_list_get(item_values, int_elem(i));
            merch_t *current_merch = (merch_t *)current_value.func_point;
            printf("Item no. %d: \n",i+1);
            print_merch(current_merch);
            printf("\n");
            if (((i+1) % 20 == 0))
            {
                input = ask_question_string("Press Y if you wish to continue printing. Otherwise press any key to end print \n");
                if(!((input = "Y") || (input = "y")))
                {
                    break;
                }
            }
        }
    }
    ioopm_linked_list_destroy(item_values);
    make_spacing;
}



void merch_destroy(merch_t *merch)
{
    free(merch); 
    //ioopm_linked_list_destroy(merch->location); ??
    merch = NULL;
}



// merch_t *merch_get(warehouse_t *warehouse, char *merch) //ändrade så den tar en char * istället för en merch_t, om vi redan har vår merch behöver vi inte hitta den
// {
//     ioopm_list_t *items = ioopm_hash_table_values(warehouse->items);
//     ioopm_list_iterator_t *iter = ioopm_list_iterator(items);

//     while(((merch_t *)ioopm_iterator_current(iter).extra)->name != merch)
//     {
//         ioopm_iterator_next(iter);
//     }
//     merch_t *current_item = ((merch_t *) ioopm_iterator_current(iter).extra);

//     ioopm_iterator_destroy(iter);
//     ioopm_linked_list_destroy(items);

//     return current_item;
// }

// void empty_stock(warehouse_t *warehouse, merch_t *merch)
// {
//     ioopm_list_iterator_t *iter = ioopm_list_iterator(merch->location);
//     while(ioopm_iterator_has_next(iter))
//     {
//         merch->location->
//     }

//     warehouse->shelves->current_shelf->stock = 0;
// }


// merch_t *merch_get(warehouse_t *warehouse, char *merch) //ändrade så den tar en char * istället för en merch_t, om vi redan har vår merch behöver vi inte hitta den
// {
//     ioopm_list_t *items = ioopm_hash_table_values(warehouse->items);
//     ioopm_list_iterator_t *iter = ioopm_list_iterator(items);

//     while(((merch_t *)ioopm_iterator_current(iter).extra)->name != merch)
//     {
//         ioopm_iterator_next(iter);
//     }
//     merch_t *current_item = ((merch_t *) ioopm_iterator_current(iter).extra);

//     ioopm_iterator_destroy(iter);
//     ioopm_linked_list_destroy(items);

//     return current_item;
// }



bool ioopm_remove_merch(warehouse_t *warehouse, char *merch_name)
{
    elem_t to_remove;
    bool result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_remove);
    if(!result)
    {
        printf("The merchandise named %s could not be found.\n", merch_name);
        return false;
    }

    merch_t *merch_to_remove = to_remove.func_point; 
    //empty_stock(warehouse, merch_to_remove);
    printf("%s was removed.\n", merch_name);

    //ioopm_linked_list_destroy(merch_to_remove->location);
    ioopm_hash_table_remove(warehouse->items, ptr_elem(merch_name));
    merch_destroy(merch_to_remove);

    return true;
}



// bool ioopm_remove_merch(warehouse_t *warehouse, char *merch_name)
// {
//     bool result;
//     merch_t *merch = merch_get(warehouse, merch_name);
//     elem_t to_remove = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch), &result);

//     if(!result)
//     {
//         printf("The merchandise named %s could not be found.\n", merch_name);
//         return false;
//     }
//     else
//     {
//        merch_t *merch_to_remove = /*(merch_t *)*/to_remove.extra; 
//        //empty_stock(warehouse, merch_to_remove);
//        printf("%s was removed.\n", merch_name);

//        ioopm_linked_list_destroy(merch_to_remove->location);
//        ioopm_hash_table_remove(warehouse->items, ptr_elem(to_remove));
//        merch_destroy(merch_to_remove);

//        return true;
//     }
// }

bool ioopm_edit_merch(warehouse_t *warehouse, char *merch_name, char *new_name, char *new_desc, size_t new_price) 
{
    elem_t to_remove;
    bool result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_remove);
    if(!result)
    {
        printf("The merchandise named %s could not be found.\n", merch_name);
        return false;
    }

    elem_t value;
    ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &value);
    merch_t *current_merch = value.func_point;
    current_merch -> name = new_name;
    current_merch -> description = new_desc;
    current_merch -> price = new_price;
    return true;
}

// void print_locations (ioopm_list_t *locations)
// {
//     ioopm_list_iterator_t *iter_locations = ioopm_list_iterator(locations);
//     int i = 1;
//     shelf_t *current_shelf;
//     while (ioopm_iterator_current(iter_locations).extra != NULL)
//     {
//         current_shelf = ioopm_iterator_current(iter_locations).extra;
//         printf("Storage location no.%d : %s\n",i, current_shelf->shelf);
//         printf("Stock: %d \n", current_shelf->stock);
//         ioopm_iterator_next(iter_locations);
//         i++;
//     }
//     ioopm_iterator_destroy(iter_locations);

// }



// // void ioopm_show_stock(warehouse_t *warehouse, merch_t *merch) //original
// // {
// //     merch_t *current_merch = merch_get(warehouse, merch);

// //     ioopm_list_iterator_t *iter_locations = ioopm_list_iterator(current_merch->location);

// //     int i = 0;
    
// //     printf("The following are the storage locations for merch: \n");
// //     print_merch(((merch_t *) ioopm_iterator_current(iter_locations).extra));
// //     printf("\n Locations and stock: \n");
// //     print_locations(current_merch);
// // }

// void ioopm_show_stock(warehouse_t *warehouse, char *merch)
// {
//     merch_t *current_merch = merch_get(warehouse, merch);

//     ioopm_list_iterator_t *iter_locations = ioopm_list_iterator(current_merch->location);

//     // int i = 0;
    
//     printf("The following are the storage locations for merch: \n");
//     print_merch(((merch_t *) ioopm_iterator_current(iter_locations).extra));
//     printf("\n Locations and stock: \n");
//     print_locations(current_merch->location);
// }



// void ioopm_remove_merch(ioopm_hash_table_t *warehouse)
// {
//     ioopm_list_t *list = ioopm_hash_table_keys(warehouse);
//     char *keys[ioopm_linked_list_size(list)];
//     for(int i = 0; i < ioopm_linked_list_size(list); i++)
//     {
//       keys[i] = ioopm_linked_list_get(list, i).extra;
//     }
//     ioopm_linked_list_destroy(list);
    
//     int input = ask_question_int("Enter the merchendise you wish to remove: "); // ge ett val att inte ta bort något item kanske
    
//     elem_t merch_to_remove = ptr_elem(keys[input-1]);
//     printf("You have removed the following merchendise: %s \n", keys[input-1]);

//     ioopm_hash_table_remove(warehouse, merch_to_remove);
//     key_array_destroy(keys, ioopm_hash_table_size(warehouse));
    
// }
/*

void ioopm_remove_merch(ioopm_hash_table_t *warehouse, merch_t *merch)
{
    char *merchname = merch->name;
    ioopm_hash_table_remove(warehouse, ptr_elem(merch)); // TODO: ht_remove tar en elem_t  ta bort stock också om det inte redan sker
    printf("Removed %s\n", merchname);
}*/

// void ioopm_edit_merch(warehouse_t *warehouse, merch_t *merch, char *new_name, char *new_desc, size_t new_price)
// {
    // ioopm_list_t *items = ioopm_hash_table_keys(warehouse->items);
    // size_t items_size = ioopm_linked_list_size(items);

    // char **keys = calloc(items_size+1, sizeof(char *));
    // int i;
    
    // for(i = 0; i < items_size; i++) // alla namn på merch till en array
    // {
    //     keys[i] = ioopm_linked_list_get(items, i).extra;
    // }

    // int input = ask_question_int("Enter the item you wish to edit: ");
    // merch = {(void *)keys[input-1]};
//     merch -> name = new_name;
//     merch -> description = new_desc;
//     merch -> price = new_price;
// }
    
    /*
    if(ioopm_hash_table_has_key(warehouse, ptr_elem(new_name))
    {
        // TODO: om det nya namnet redan existerar avbryts funktionen
    }   
    int merch_stock = merch.stock;
    ioopm_list_t merch_location = merch.location; 
    ioopm_remove_merch(warehouse, merch);

    merch = {.name = new_name, .description = new_desc, .price = new_price, .stock = merch_stock, .location = merch_location};

    ioopm_add_merch(merch);
    */


// void ioopm_replenish_stock(ioopm_hash_table_t warehouse, merch_t merch, size_t quantity) // TODO: ska stock läggas till på shelves också
// {
//     merch.stock = merch.stock + quantity;
// }


// void ioopm_show_stock(warehouse_t *warehouse, merch_t *merch)
// {
    // ioopm_list_t *items = ioopm_hash_table_keys(warehouse->items);
    // size_t items_size = ioopm_linked_list_size(items);

    // char **keys = calloc(items_size+1, sizeof(char *));
    // int i;

    // for(i = 0; i < items_size; i++) // alla namn på merch till en array
    // {
    //     keys[i] = ioopm_linked_list_get(items, i).extra;
    // }
    //int input = ask_question_int("Enter the item you wish to view the stock of: "); // ge ett val att inte ta bort något item kanske

    //elem_t merch_to_remove = {.extra = (void *)keys[input-1]};
    //int stock = &(merch_to_remove.extra)->stock;
    //int stock = merch->location->
    
    //printf("Stock: %d", stock);

// }