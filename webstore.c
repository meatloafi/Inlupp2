#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h>
#include <ctype.h> 
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


struct shelf
{
    char section;
    int number;
};



void key_array_destroy(char **keys, size_t size)
{
  for (int i = 0; i < size; i++)
  {
    free (keys[i]);
  }
}

//ask_question_shelf("Shelf: ");
entry_t *create_merch()
{
    char *name_sd = ask_question_string("Name: ");
    char *desc_sd = ask_question_string("Description: ");
    size_t price = ask_question_int("Price: ");
    int stock = ask_question_int("Quantity: ");
    char name[255] = {0};
    char desc[255] = {0};
    strcat(name, desc_sd);
    strcat(desc, name_sd);
    free(name_sd);
    free(desc_sd);
    
    ioopm_list_t *location = ioopm_linked_list_create(NULL);
    char shelf_section = (ask_question_char("Shelf section: "));
    int shelf_number = ask_question_int("Shelf number: ");

    shelf_t shelf = { .section = shelf_section, .number = shelf_number};
    ioopm_linked_list_append(location, ptr_elem(&shelf));

    merch_t merch = {.name = name, .description = desc, .price = price, .stock = stock, .location = location};

    entry_t *merchandise = calloc(1, sizeof(entry_t));

    merchandise->key = ptr_elem(merch.name);
    merchandise->value = ptr_elem(&merch);
    // merchandise->next = NULL;
    
    return merchandise;
}

void ioopm_add_merch(ioopm_hash_table_t *warehouse)
{
    entry_t *new_merch = create_merch();

    if(!ioopm_hash_table_has_key(warehouse, new_merch->key)) // TODO; Verkar inte förhindra att man kan stoppa många med samma namn
    {
        ioopm_hash_table_insert(warehouse, new_merch->key, new_merch->value);
    }
}



void ioopm_list_merch(ioopm_hash_table_t *warehouse)
{
    ioopm_list_t *items = ioopm_hash_table_keys(warehouse);
    size_t items_size = ioopm_linked_list_size(items);

    char **keys = calloc(items_size+1, sizeof(char *));
    int i;

    if (ioopm_linked_list_is_empty(items))
    {
        printf("The warehouse is empty \n");
    }
    else
    {
        for(i = 0; i < items_size; i++) // alla namn på merch till en array
    {
        keys[i] = ioopm_linked_list_get(items, i).extra;
    }

    for (i = 0; i < items_size; i++)  // printar namnen på all merch
    {                                 // TODO: printa 20 items i taget
        printf ("%d. %s \n",(i+1), keys[i]);
    }
    }
    free(keys);
    ioopm_linked_list_destroy(items);
}
 


void ioopm_remove_merch(ioopm_hash_table_t *warehouse)
{
    ioopm_list_t *items = ioopm_hash_table_keys(warehouse);
    size_t items_size = ioopm_linked_list_size(items);

    char **keys = calloc(items_size+1, sizeof(char *));
    int i;

    for(i = 0; i < items_size; i++) // alla namn på merch till en array
    {
        keys[i] = ioopm_linked_list_get(items, i).extra;
    }
    int input = ask_question_int("Enter the item you wish to remove: "); // ge ett val att inte ta bort något item kanske

    elem_t merch_to_remove = {.extra = (void *)keys[input-1]};

    ioopm_hash_table_remove(warehouse, merch_to_remove);
}




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

// merch_t ioopm_edit_merch(ioopm_hash_table_t warehouse, merch_t merch, char *new_name, char *new_desc, size_t new_price)
// {
//     if(ioopm_hash_table_has_key(warehouse, ptr_elem(new_name))
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