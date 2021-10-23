#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h>
#include "hash_table.h"
#include "list_linked.h"
#include "iterator.h"
#include "common.h"
#include "business.h"
#include "utils.h"

typedef struct merch merch_t;
typedef struct shelf shelf_t;


struct merch 
{
    char *name;
    int price;
    char *description;
    int stock;
    ioopm_list_t *location;
};



merch_t create_merch()
{
    char *name = ask_question_string("Name: ");
    char *desc = ask_question_string("Description: ");
    size_t price = ask_question_int("Price: ");
    int stock = ask_question_int("Quantity: ");
    //ioopm_list_t *location = ask_question_shelf("Shelf: ");
    merch_t merch = {.name = name, .description = desc, .price = price, .stock = stock, .location = location};

    return merch;
}

void ioopm_add_merch(ioopm_hash_table_t *warehouse)
{
    if(!ioopm_hash_table_has_key(merch.name))
    {
        merch_t new_merch = create_merch();
        ioopm_hash_table_insert(warehouse, merch.name, ptr_elem(merch)); // TODO: insert tar en elem_t
    }
}

// void ioopm_list_merch(ioopm_hash_table_t *warehouse)
// {
//     ioopm_list_t *items = ioopm_hash_table_keys(warehouse);
//     size_t items_size = list_ioopm_linked_list_size(items);

//     char *keys[items_size];
//     int i;

//     for(i = 0; i < items_size; i++) // alla namn på merch till en array
//     {
//       keys[i] = ioopm_linked_list_get(list, i).extra;
//     }

//     for (i = 0; i < items_size; i++)  // printar namnen på all merch
//     {                                 // TODO: printa 20 items i taget
//         printf ("%d. %s \n ",(i+1), keys[i]);
//     }
// }

// void ioopm_remove_merch(ioopm_hash_table_t *warehouse, merch_t merch)
// {
//     ioopm_hash_table_remove(warehouse, merch.name); // TODO: ht_remove tar en elem_t
//                                                     // TODO: ta bort stock också om det inte redan sker
// }

// merch_t ioopm_edit_merch(ioopm_hash_table_t warehouse, merch_t merch, char *new_name, char *new_desc, size_t new_price)
// {
//     if(ioopm_hash_table_has_key(new_name))
//     {
//         // TODO: om det nya namnet redan existerar avbryts funktionen
//     }
//     int merch_stock = merch.stock;
//     ioopm_list_t merch_location = merch.location; 
//     ioopm_remove_merch(warehouse, merch);

//     merch = {.name = new_name, .description = new_desc, .price = new_price, .stock = merch_stock, .location = merch_location};

//     ioopm_add_merch(merch);
// }

// void ioopm_replenish_stock(ioopm_hash_table_t warehouse, merch_t merch, size_t quantity) // TODO: ska stock läggas till på shelves också
// {
//     merch.stock = merch.stock + quantity;
// }


// void ioopm_show_stock(ioopm_hash_table_t warehouse, merch_t merch)
// {
//     printf("Stock: %d", merch.stock);
// }