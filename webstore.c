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
typedef struct cart cart_t;


struct merch 
{
    int id;
    char *name;
    size_t price;
    char *description;
    ioopm_list_t *location; //FIXME: frigör detta
    size_t total_stock;
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
    ioopm_list_t *carts;
    size_t item_count;
    size_t cart_count;
};

struct cart
{
    ioopm_hash_table_t *items;
    int id;
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

// static bool shelf_compare(elem_t index_ignored, elem_t a, elem_t b)
// {
//   shelf_t *s1 = a.func_point;
//   shelf_t *s2 = b.func_point;
//   return (s1->shelf == s2->shelf);
// }



warehouse_t *ioopm_warehouse_create()
{
    ioopm_hash_table_t *items = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, NULL, string_value_equiv_ht, NULL);
    ioopm_hash_table_t *shelves = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, NULL, NULL, NULL);
    ioopm_list_t *carts = ioopm_linked_list_create(string_value_equiv_ht);
    warehouse_t *warehouse = calloc(1, sizeof(warehouse_t));
    warehouse->items = items;
    warehouse->shelves = shelves;
    warehouse->carts = carts;
    warehouse->item_count = 0;
    warehouse->cart_count = 0;
    return warehouse;
}

void ioopm_warehouse_destroy(warehouse_t *warehouse)
{
    ioopm_hash_table_destroy(warehouse->items);
    ioopm_hash_table_destroy(warehouse->shelves);
    ioopm_linked_list_destroy(warehouse->carts);
    free(warehouse);
}

cart_t *ioopm_cart_create(warehouse_t *warehouse)
{
    ioopm_hash_table_t *cart_ht = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, NULL, string_value_equiv_ht, NULL);
    ioopm_linked_list_append(warehouse->carts, ptr_elem(cart_ht));
    cart_t *cart = calloc(1, sizeof(cart_t));
    warehouse->cart_count++;
    cart->items = cart_ht;
    cart->id = warehouse->cart_count;
    return cart;
}

void ioopm_remove_cart(cart_t *cart)
{
    ioopm_hash_table_destroy(cart->items);
    free(cart);
}

//ask_question_shelf("Shelf: ");
merch_t *create_merch(int id, char *name, char *desc, size_t price)
{
    merch_t *merchandise = calloc(1, sizeof(merch_t));
    merchandise -> id = id;
    merchandise -> name = name;
    merchandise -> description = desc;
    merchandise -> price = price;
    merchandise -> location = ioopm_linked_list_create(string_value_equiv_ht);
    merchandise -> total_stock = 0;
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
    /*
    if(ioopm_linked_list_is_empty(merch_to_remove->location))
    {
        ioopm_linked_list_destroy(merch_to_remove->location);
    }*/ //BÖR TESTAS

    ioopm_hash_table_remove(warehouse->items, ptr_elem(merch_name));
    merch_destroy(merch_to_remove);

    return true;
}




bool ioopm_edit_merch(warehouse_t *warehouse, char *merch_name, char *new_name, char *new_desc, size_t new_price) 
{
    elem_t to_edit;
    bool result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_edit);
    if(!result)
    {
        printf("The merchandise named %s could not be found.\n", merch_name);
        return false;
    }

    //elem_t value;
    merch_t *current_merch = to_edit.func_point;
    current_merch -> name = new_name;
    current_merch -> description = new_desc;
    current_merch -> price = new_price;
    return true;
}



void print_locations (merch_t *merch)
{
    ioopm_list_iterator_t *iter_locations = ioopm_list_iterator(merch->location);
    int i = 1;
    
    shelf_t *current_shelf;
    current_shelf = ioopm_iterator_current(iter_locations).func_point;
    printf("Storage location no.%d : %s\n",i, current_shelf->shelf);
    
    while (ioopm_iterator_has_next(iter_locations))
    {
        ioopm_iterator_next(iter_locations);
        current_shelf = ioopm_iterator_current(iter_locations).func_point;
        i++;
        printf("Storage location no.%d : %s\n",i, current_shelf->shelf);
        
    }

    printf("Stock: %ld \n", merch->total_stock);
    ioopm_iterator_destroy(iter_locations);
}



bool ioopm_show_stock(warehouse_t *warehouse, char *merch_name) //original
{
    elem_t to_show;
    bool result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_show); //gör detta till egen funktion
    if(!result)
    {
        printf("The merchandise named %s could not be found.\n", merch_name);
        return false;
    }
    //elem_t value;
    merch_t *current_merch = to_show.func_point;

    if(ioopm_linked_list_is_empty(current_merch->location))
    {
        printf("The merchandise named %s is not in stock anywhere.\n", merch_name);
        return true;
    }
    
    printf("The following are the storage locations for merch: \n");
    print_merch(current_merch);
    printf("\nLocations and stock: \n");
    print_locations(current_merch);
    return true;
}


bool ioopm_location_contains(ioopm_list_t *location, char *shelf)
{
    if (ioopm_linked_list_is_empty(location))
    {
        return false;
    }
    ioopm_list_iterator_t *iter = ioopm_list_iterator(location);

    while(ioopm_iterator_has_next(iter))
    {
        if ((ioopm_iterator_current(iter).func_point == ptr_elem(shelf).func_point))
        {
            ioopm_iterator_destroy(iter);
            return true;
        }
        ioopm_iterator_next(iter);
    }
    ioopm_iterator_destroy(iter);
    return false;
    

}


bool is_shelf_in_merch(merch_t *merch, char *shelf_eq)
{
    if (ioopm_linked_list_is_empty(merch->location))
    {
        return false;
    }

    ioopm_list_iterator_t *iter_locations = ioopm_list_iterator(merch->location);

    shelf_t *current_shelf;

    current_shelf = ioopm_iterator_current(iter_locations).func_point;
    if((current_shelf->shelf = shelf_eq))
        {
            return true;
        }  
    
    while (ioopm_iterator_has_next(iter_locations))
    {
        ioopm_iterator_next(iter_locations);
        current_shelf = ioopm_iterator_current(iter_locations).func_point;
        if((current_shelf->shelf = shelf_eq))
        {
            return true;
        }  
    }
    return false;
}



bool ioopm_replenish_stock(warehouse_t *warehouse, char *merch_name, char *shelf_name, int quantity)
{
    if(quantity >= 1)
    {
        elem_t merch_to_replenish;
        bool result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &merch_to_replenish);
        if(!result)
        {
            printf("The merchandise named %s could not be found.\n", merch_name);
            return false;
        }

        ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &merch_to_replenish);

        merch_t *current_merch = merch_to_replenish.func_point;

        shelf_t *shelf;
        elem_t shelf_to_replenish;
        result = ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(shelf_name), &shelf_to_replenish);

        if((result) && (!is_shelf_in_merch(current_merch, shelf_to_replenish.func_point)))
        {
            printf("The shelf is used for another merch. \n");
            return false;
        }

        if(is_shelf_in_merch(current_merch, shelf_to_replenish.func_point))
        {
            shelf = (shelf_t *)(shelf_to_replenish).func_point;
            shelf->stock = shelf->stock + quantity;
        }
        
        else
        {
            shelf = create_shelf(shelf_name);
            shelf->stock = shelf->stock + quantity;
            ioopm_linked_list_append(current_merch->location, ptr_elem(shelf));
            ioopm_hash_table_insert(warehouse->shelves, ptr_elem(current_merch->name), ptr_elem(shelf_name));
        }

        current_merch->total_stock = current_merch->total_stock + quantity;
        return true;
        }
        else
        {
            printf("Enter a valid quantity (>1) \n");
            return false;
    }
}

// bool ioopm_replenish_stock(warehouse_t *warehouse, char *merch_name, char *shelf_name, int quantity)
// {
//     if(quantity >= 1)
//     {
//         elem_t merch_to_replenish;
//         bool result = ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &merch_to_replenish);
//         if(!result)
//         {
//             printf("The merchandise named %s could not be found.\n", merch_name);
//             return false;
//         }

//         ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &merch_to_replenish);

//         merch_t *current_merch = merch_to_replenish.func_point;

//         shelf_t *shelf;
//         elem_t shelf_to_replenish;
//         result = ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(shelf_name), &shelf_to_replenish);

//         if((result) && (!ioopm_linked_list_contains(current_merch->location, ptr_elem(shelf_name))))
//         {
//             printf("The shelf is used for another merch. \n");
//             return false;
//         }

//         if(ioopm_linked_list_contains(current_merch->location, ptr_elem(shelf_name)))
//         {
//             shelf = shelf_to_replenish.func_point;
//             shelf->stock = shelf->stock + quantity;
//         }
        
//         else
//         {
//             shelf = create_shelf(shelf_name);
            
//             shelf->stock = shelf->stock + quantity;
//             ioopm_linked_list_append(current_merch->location, ptr_elem(shelf));
//         }

//         current_merch->total_stock = current_merch->total_stock + quantity;
//         return true;
//         }
//         else
//         {
//             printf("Enter a valid quantity (>1) \n");
//             return false;
//     }
// }


// bool ioopm_add_to_cart(warehouse_t *warehouse, cart_t *cart, char *merch_name, size_t quantity) // TODO; hitta ett sätt att kolla så alla carts tillsammans inte överstiger stock av en merch
// {
//     elem_t to_check_quantity;
//     if(ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity))
//     {
//         return false;
//     }
//     ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_check_quantity);
//     merch_t *merch = to_check_quantity.func_point;
//     size_t current_stock = merch->total_stock;
//     merch_t *merch_info;
//     merch_info -> total_stock = quantity;
//     merch_info -> price = merch->price;
//     // merch_info -> location = merch->location; // om den behövs
//     if(quantity <= current_stock)
//     {
//         ioopm_hash_table_insert(cart->items, ptr_elem(merch_name), /*int_elem(quantity)*//*to_edit*/ptr_elem(merch_info));
//         return true;
//     }

//     printf("Please select fewer %s since there are only %ld %s in stock.", merch->name, current_stock, merch->name);
//     return false;
// }

// bool ioopm_remove_from_cart(cart_t *cart, char *merch_name, size_t quantity)
// {
//     elem_t to_check_quantity;
//     if(quantity < 1 || !ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity))
//     {
//         printf("%s does not exist in the store.\n", merch_name);
//         return false;
//     }

//     ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity);
//     size_t current_quantity = ((merch_t *)to_check_quantity.func_point)->total_stock;

//     if(quantity == current_quantity)
//     {
//         ioopm_hash_table_remove(cart->items, ptr_elem(merch_name));
//         return true;
//     }
//     else if(quantity < current_quantity)
//     {
//         // ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity);
//         // ioopm_hash_table_remove(cart->items, ptr_elem(merch_name));
//         size_t new_quantity = current_quantity - quantity;
//         merch_t updated_quantity = {.total_stock = new_quantity, .price = ((merch_t *)to_check_quantity.func_point)->price};
//         // updated_quantity -> total_stock = new_quantity;
//         // updated_quantity -> price = ((merch_t *)to_check_quantity.func_point)->price;
//         // ioopm_hash_table_remove(cart->items, ptr_elem(merch_name));
//         ioopm_hash_table_insert(cart->items, ptr_elem(merch_name), ptr_elem(&updated_quantity));
//         return true;
//     }
//     // ioopm_hash_table_remove(cart->items, ptr_elem(merch_name));
//     // return true;
// }

cart_t *get_cart(warehouse_t *warehouse, int cart_id)
{
cart_t *cart;
bool result = ioopm_hash_table_lookup(warehouse->carts, int_elem(cart_id), &cart);
if(result)
{
    return cart;
}
errno = EINVAL;
}



bool ioopm_add_to_cart(warehouse_t *warehouse, cart_t *cart, char *merch_name, size_t quantity) // TODO; hitta ett sätt att kolla så alla carts tillsammans inte överstiger stock av en merch
{                                                                                    // om denna merch redan finns ska det plussas på nuvarande mängd i cart
    elem_t to_check_quantity;
    if(ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity))
    {
        return false;
    }
    ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_check_quantity);
    merch_t *merch = to_check_quantity.func_point;
    size_t current_stock = merch->total_stock;

    if(quantity <= current_stock)
    {
        ioopm_hash_table_insert(cart->items, ptr_elem(merch_name), int_elem(quantity)/*to_edit*//*ptr_elem(merch)*/);
        return true;
    }

    printf("Please select fewer %s since there are only %ld %s in stock.", merch->name, current_stock, merch->name);
    return false;
}

bool ioopm_remove_from_cart(cart_t *cart, char *merch_name, size_t quantity)
{
    elem_t to_check_quantity;
    bool result = ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity);
    if(quantity < 1 || !result/*!ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity)*/)
    {
        return false;
    }
    else if(quantity == (size_t)to_check_quantity.func_point)
    {
        ioopm_hash_table_remove(cart->items, ptr_elem(merch_name));
        return true;
    }
    // else if(quantity < (size_t)to_check_quantity.func_point)
        // ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity);
    size_t new_quantity = (size_t)to_check_quantity.func_point - quantity;
    ioopm_hash_table_insert(cart->items, ptr_elem(merch_name), int_elem(new_quantity)); // TODO; tillfällig, tar bort och lägger dit en ny med samma namn istället för att uppdatera den
    return true;
}

size_t ioopm_calc_cost_cart(warehouse_t *warehouse, cart_t *cart)
{
    size_t total_cart_cost = 0;
    elem_t current_merch_name;
    elem_t current_merch_price;
    elem_t current_merch_quantity;
    ioopm_list_t *names = ioopm_hash_table_keys(cart->items);
    ioopm_list_t *quantity = ioopm_hash_table_values(cart->items);
    size_t size = ioopm_linked_list_size(names);

    for(int i = 0; i < size; i++)
    {
        current_merch_name = ioopm_linked_list_get(names, int_elem(i));
        ioopm_hash_table_lookup(warehouse->items, current_merch_name, &current_merch_price);
        current_merch_quantity = ioopm_linked_list_get(quantity, int_elem(i));
        size_t quantity = (size_t)current_merch_quantity.func_point;
        size_t price =  (*(merch_t *)current_merch_price.func_point).price;
        total_cart_cost = total_cart_cost + (quantity * price);
    }
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(quantity);
    return total_cart_cost;
}

static void remove_stock_from_shelves(warehouse_t *warehouse, char *merch_name, size_t quantity)
{
    // bool result;
    elem_t merch_value;
    /*result = */ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &merch_value);
    // ioopm_list_t *stock = (*(merch_t *)merch_value.func_point).location;
    ioopm_list_t *locations = (*(merch_t *)merch_value.func_point).location;
    int i = 0;
    while(quantity > 0)
    {
        elem_t location = ioopm_linked_list_get(locations, int_elem(i));
        shelf_t *current_shelf = (shelf_t *)location.func_point;
        if(current_shelf->stock == quantity)
        {
            quantity = 0;
            ioopm_linked_list_remove(locations, int_elem(i));
        }
        else if(current_shelf->stock > quantity)
        {
            current_shelf->stock = current_shelf->stock - quantity;
            (*(merch_t *)merch_value.func_point).total_stock = (*(merch_t *)merch_value.func_point).total_stock - quantity;
            quantity = 0;
        }
        else if(current_shelf->stock < quantity)
        {
            quantity = quantity - current_shelf->stock;
            ioopm_linked_list_remove(locations, int_elem(i));
        }
    }
}

void ioopm_checkout_cart(warehouse_t *warehouse, cart_t *cart)
{
    size_t cart_cost = ioopm_calc_cost_cart(warehouse, cart);
    printf("Total cost: %ld.\n", cart_cost);

    char *merch_name;
    elem_t current_merch_name;
    elem_t current_merch_quantity;
    size_t merch_quantity;
    ioopm_list_t *names = ioopm_hash_table_keys(cart->items);
    ioopm_list_t *quantities = ioopm_hash_table_values(cart->items);
    size_t size = ioopm_linked_list_size(names);

    for(int i = 0; i < size; i++)
    {
        current_merch_name = ioopm_linked_list_get(names, int_elem(i));
        current_merch_quantity = ioopm_linked_list_get(quantities, int_elem(i));

        merch_name = (char *)current_merch_name.func_point;
        merch_quantity = (size_t)current_merch_quantity.func_point;

        remove_stock_from_shelves(warehouse, merch_name, merch_quantity);
    }
    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(quantities);
    ioopm_remove_cart(cart);
}