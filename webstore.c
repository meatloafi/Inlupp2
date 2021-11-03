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
    ioopm_list_t *location;
    size_t total_stock;
    bool lock;
};


struct shelf
{
    char *shelf;
    char *item_in_shelf;
    size_t stock;
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

static void free_all_merch(ioopm_hash_table_t *items)
{
    ioopm_list_t *merch_list = ioopm_hash_table_values(items);
    size_t merch_size = ioopm_linked_list_size(merch_list);

    for (size_t i = 0; i < merch_size; i++)
    {
        elem_t current_merch_elem = ioopm_linked_list_get(merch_list, int_elem(i));
        merch_t *current_merch = (merch_t *)current_merch_elem.func_point;
        ioopm_list_t *current_locations = current_merch->location;
        ioopm_linked_list_destroy(current_locations);
        free(current_merch);
    }
    ioopm_linked_list_destroy(merch_list);
    ioopm_hash_table_destroy(items);
}

static void free_all_shelves(ioopm_hash_table_t *shelves) // FIXME: när replenish är fixat
{
    ioopm_list_t *shelves_list = ioopm_hash_table_values(shelves);
    size_t shelves_size = ioopm_linked_list_size(shelves_list);

    for (size_t i = 0; i < shelves_size; i++)
    {
        elem_t current_shelf_elem = ioopm_linked_list_get(shelves_list, int_elem(i));
        shelf_t *current_shelf = (shelf_t *)current_shelf_elem.func_point;
        free(current_shelf);
    }
    ioopm_hash_table_destroy(shelves);
    ioopm_linked_list_destroy(shelves_list);
}

static void free_all_carts(ioopm_list_t *carts)
{
    size_t carts_size = ioopm_linked_list_size(carts);

    for(size_t i = 0; i < carts_size; i++)
    {
        elem_t current_cart_elem = ioopm_linked_list_get(carts, int_elem(i));
        ioopm_hash_table_t *current_cart_ht = (ioopm_hash_table_t *)current_cart_elem.func_point;
        ioopm_hash_table_destroy(current_cart_ht);
        // cart_t *current_cart = (cart_t *)current_cart_elem.func_point;
        // free(current_cart);
    }
    ioopm_linked_list_destroy(carts);
}


void merch_destroy(merch_t *merch)
{
    if(merch->location != NULL)
    {
        if(!ioopm_linked_list_is_empty(merch->location))
        {
            ioopm_linked_list_destroy(merch->location);
        }
        free(merch); 
        merch = NULL;
    }
}

void ioopm_warehouse_destroy(warehouse_t *warehouse)
{
    ioopm_linked_list_destroy(warehouse->carts);
    free_all_shelves(warehouse->shelves);
    free_all_merch(warehouse->items);
    // free_all_carts(warehouse->carts);
    free(warehouse);
}

cart_t *ioopm_cart_create(warehouse_t *warehouse)
{
    ioopm_hash_table_t *cart_ht = ioopm_hash_table_create((ioopm_hash_function) string_sum_hash, NULL, string_value_equiv_ht, NULL);
    cart_t *cart = calloc(1, sizeof(cart_t));
    cart->items = cart_ht;
    cart->id = ++warehouse->cart_count;
    ioopm_linked_list_append(warehouse->carts, ptr_elem(cart));
    return cart;
}

void ioopm_remove_cart(warehouse_t *warehouse, cart_t *cart)
{
    ioopm_hash_table_destroy(cart->items);
    ioopm_linked_list_remove(warehouse->carts, int_elem(cart->id));
}

merch_t *create_merch(int id, char *name, char *desc, size_t price)
{
    merch_t *merchandise = calloc(1, sizeof(merch_t));
    merchandise -> id = id;
    merchandise -> name = name;
    merchandise -> description = desc;
    merchandise -> price = price;
    merchandise -> location = ioopm_linked_list_create(string_value_equiv_ht);
    merchandise -> total_stock = 0;
    merchandise -> lock = false;
    return merchandise;
}

shelf_t *create_shelf(char *shelf_name)
{
    shelf_t *result = calloc(1, sizeof(shelf_t));
    result->shelf = shelf_name;
    result->stock = 0;
    result->item_in_shelf = NULL;
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
    printf ("Name: %s \nDesc: %s \nPrice: %ld \n", merch -> name, merch -> description, merch -> price);
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
        //free(input);
    }
    ioopm_linked_list_destroy(item_values);
    make_spacing;
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
    ioopm_linked_list_destroy(merch_to_remove->location);
    ioopm_hash_table_remove(warehouse->items, ptr_elem(merch_name));
    free(merch_to_remove);

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

    merch_t *current_merch = to_edit.func_point;

    if (current_merch->lock == true)
    {
        printf("The merchendise is currently in a cart and is therefore not editable");
        return false;
    }

    merch_t *updated_merch = calloc(1, sizeof(merch_t));
    updated_merch->name = new_name;
    updated_merch->description = new_desc;
    updated_merch->price = new_price;
    updated_merch->location = current_merch->location;
    updated_merch->total_stock = current_merch->total_stock;
    updated_merch->lock = false;
    ioopm_hash_table_remove(warehouse->items, ptr_elem(merch_name));
    ioopm_hash_table_insert(warehouse->items, ptr_elem(new_name), ptr_elem(updated_merch));

    size_t shelves_size = ioopm_linked_list_size(updated_merch->location);
    elem_t current_shelf_elem;
    for(size_t i = 0; i < shelves_size; i++)
    {
        elem_t merch_locations = ioopm_linked_list_get(updated_merch->location, int_elem(i));
        char *current_merch_location = merch_locations.func_point;
        ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(current_merch_location), &current_shelf_elem);
        shelf_t *current_shelf_info = current_shelf_elem.func_point;

        shelf_t *shelf_with_new_merch_name = calloc(1, sizeof(shelf_t));
        shelf_with_new_merch_name->shelf = current_shelf_info->shelf;
        shelf_with_new_merch_name->stock = current_shelf_info->stock;
        shelf_with_new_merch_name->item_in_shelf = new_name;

        ioopm_hash_table_remove(warehouse->shelves, ptr_elem(current_merch_location));
        ioopm_hash_table_insert(warehouse->shelves, ptr_elem(shelf_with_new_merch_name->shelf), ptr_elem(shelf_with_new_merch_name));

        shelf_t *current_shelf_shelves = current_shelf_elem.func_point;
        current_shelf_shelves->item_in_shelf = new_name;
    }

    return true;
}



void print_locations (warehouse_t *warehouse, merch_t *merch)
{
    ioopm_list_iterator_t *iter_locations = ioopm_list_iterator(merch->location);
    int i = 1;
    
    char *current_shelf = ioopm_iterator_current(iter_locations).func_point;

    elem_t shelf_elem;
    ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(current_shelf), &shelf_elem);
    shelf_t *shelf = shelf_elem.func_point;

    printf("Storage location no.%d : %s\n",i, current_shelf);
    printf("- Stock in %s: %ld \n",current_shelf, shelf->stock);
    
    while (ioopm_iterator_has_next(iter_locations))
    {
        ioopm_iterator_next(iter_locations);
        current_shelf = ioopm_iterator_current(iter_locations).func_point;
        i++;

        ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(current_shelf), &shelf_elem);
        shelf = shelf_elem.func_point;

        printf(" \n Storage location no.%d : %s\n",i, current_shelf);
        printf("- Stock in %s: %ld \n",current_shelf, shelf->stock);
        
    }

    printf("\n~ Total stock: %ld ~\n", merch->total_stock);
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
    
    printf("The following are the storage locations for merch: \n\n");
    print_merch(current_merch);
    printf("\nLocations and stock: \n\n");
    print_locations(warehouse, current_merch);
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
    // if(strcmp(current_shelf->shelf, shelf_eq) == 0)
    // {
    //     return true;
    // }

    while (ioopm_iterator_has_next(iter_locations))
    {
        current_shelf = ioopm_iterator_current(iter_locations).func_point;
        if(strcmp(current_shelf->shelf, shelf_eq) == 0)
        {
            return true;
        }
        ioopm_iterator_next(iter_locations);
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

        merch_t *current_merch = merch_to_replenish.func_point;

        elem_t shelf_elem;
        result = ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(shelf_name), &shelf_elem);
        shelf_t *shelf = shelf_elem.func_point;

        if((result) && (strcmp(shelf->item_in_shelf, merch_name) != 0))
        {
            printf("The shelf is used for another merch. \n");
            return false;
        }

        if((result))
        {
            //använd att shelf nu har namnet på sin merch för att bestämma shelf objected för att sedan kunna öka dess stock
            shelf->stock += quantity;
        }
        
        else
        {
            shelf = create_shelf(shelf_name);
            shelf->item_in_shelf = merch_name;
            shelf->stock += quantity;
            ioopm_linked_list_append(current_merch->location, ptr_elem(shelf_name));
            ioopm_hash_table_insert(warehouse->shelves, ptr_elem(shelf_name), ptr_elem(shelf));
        }

        current_merch->total_stock += quantity;
        return true;
        }
        else
        {
            printf("Enter a valid quantity (>1) \n");
            return false;
    }
}


bool get_cart(warehouse_t *warehouse, int cart_id, cart_t **cart)
{
    if (ioopm_linked_list_is_empty(warehouse->carts))
    {
        return false;
    }
    ioopm_list_iterator_t *iter = ioopm_list_iterator(warehouse->carts);

    if((((cart_t *)ioopm_iterator_current(iter).func_point)->id) == cart_id)
        {   
            *cart = (ioopm_iterator_current(iter).func_point);
            ioopm_iterator_destroy(iter);
            return true;
        }

    while (ioopm_iterator_has_next(iter))
    {
        ioopm_iterator_next(iter);
        if((((cart_t *)ioopm_iterator_current(iter).func_point)->id) == cart_id)
        {   
            *cart = (ioopm_iterator_current(iter).func_point);
            ioopm_iterator_destroy(iter);
            return true;
        }
        // ioopm_iterator_next(iter);
    }
    ioopm_iterator_destroy(iter);
    return false;
}

// bool get_cart(warehouse_t *warehouse, int cart_id, cart_t **cart)
// {
//     if (ioopm_linked_list_is_empty(warehouse->carts))
//     {
//         return false;
//     }
//     ioopm_list_iterator_t *iter = ioopm_list_iterator(warehouse->carts);

//     if((((cart_t *)ioopm_iterator_current(iter).func_point)->id) == cart_id)
//         {   
//             cart = (ioopm_iterator_current(iter).func_point);
//             return true;
//         }

//     while (ioopm_iterator_has_next(iter))
//     {
//         if((((cart_t *)ioopm_iterator_current(iter).func_point)->id) == cart_id)
//         {   
//             cart = (ioopm_iterator_current(iter).func_point);
//             return true;
//         }
//         ioopm_iterator_next(iter);
//     }
//     // int cart_idd = (((cart_t *)ioopm_iterator_current(iter).func_point)->id);
//     // printf("ID: %d \n", cart_idd);
//     return false;
// }


bool ioopm_add_to_cart(warehouse_t *warehouse, cart_t *cart, char *merch_name, size_t quantity) // TODO; hitta ett sätt att kolla så alla carts tillsammans inte överstiger stock av en merch
{
    if(quantity >= 1)
    {                    
        elem_t to_check_quantity;                                           // om denna merch redan finns ska det plussas på nuvarande mängd i cart
        
        if(ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity))
        {
            return false;
        }

        if(!ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_check_quantity))
        {
            printf("The merchandise named %s could not be found.\n", merch_name);
            return false;
        }
        merch_t *merch = to_check_quantity.func_point;
        size_t current_stock = merch->total_stock;

        if(quantity <= current_stock)
        {
            merch->lock = true;
            ioopm_hash_table_insert(cart->items, ptr_elem(merch_name), int_elem(quantity));
            return true;
        }

        printf("Please select fewer %s since there are only %ld %s in stock.", merch->name, current_stock, merch->name);
        return false;
    }
    else
    {
        printf("Enter a valid quantity (>1) \n");
        return false;
    }
}

bool ioopm_remove_from_cart(warehouse_t *warehouse, cart_t *cart, char *merch_name, size_t quantity)
{
    elem_t to_check_quantity;

    elem_t to_unlock;
    ioopm_hash_table_lookup(warehouse->items, ptr_elem(merch_name), &to_unlock);
    merch_t *merch = to_unlock.func_point;

    bool result = ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity);
    if(quantity < 1 || !result/*!ioopm_hash_table_lookup(cart->items, ptr_elem(merch_name), &to_check_quantity)*/)
    {
        return false;
    }
    
    else if(quantity == (size_t)to_check_quantity.func_point)
    {
        ioopm_hash_table_remove(cart->items, ptr_elem(merch_name));
        merch->lock = false; //unlock
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
    merch_t *current_merch = merch_value.func_point;
    ioopm_list_t *locations = current_merch->location;

    int i = 0;
    while(quantity > 0)
    {
        char *shelf_name = ioopm_linked_list_get(locations, int_elem(i)).func_point;
        elem_t shelf_elem;
        ioopm_hash_table_lookup(warehouse->shelves, ptr_elem(shelf_name), &shelf_elem);
        shelf_t *current_shelf = shelf_elem.func_point;
        if(current_shelf->stock == quantity)
        {
            quantity = 0;
            ioopm_linked_list_remove(locations, int_elem(i));
            // free(current_shelf);
            ioopm_hash_table_remove(warehouse->shelves, ptr_elem(shelf_name));
        }
        else if(current_shelf->stock > quantity)
        {
            current_shelf->stock = current_shelf->stock - quantity;
            current_merch->total_stock -= quantity;
            quantity = 0;
        }
        else if(current_shelf->stock < quantity)
        {
            quantity = quantity - current_shelf->stock;
            ioopm_linked_list_remove(locations, int_elem(i));
            // free(current_shelf);
            ioopm_hash_table_remove(warehouse->shelves, ptr_elem(shelf_name));
        }
        i++;
    }
}

void ioopm_checkout_cart(warehouse_t *warehouse, cart_t *cart)
{
    size_t cart_cost = ioopm_calc_cost_cart(warehouse, cart);
    printf("Total cost: %ld.\n", cart_cost);

    elem_t to_unlock;

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

        ioopm_hash_table_lookup(warehouse->items, ptr_elem(current_merch_name.func_point), &to_unlock);
        merch_t *current_merch = to_unlock.func_point;
        current_merch->lock = false; //unlock

        merch_name = (char *)current_merch_name.func_point;
        merch_quantity = (size_t)current_merch_quantity.func_point;

        remove_stock_from_shelves(warehouse, merch_name, merch_quantity);
    }

    ioopm_linked_list_destroy(names);
    ioopm_linked_list_destroy(quantities);
    // ioopm_remove_cart(cart);
}

void print_carts (warehouse_t *warehouse)
{
    if (!ioopm_linked_list_is_empty(warehouse->carts))
    {
        ioopm_list_iterator_t *iter_carts = ioopm_list_iterator(warehouse->carts);
        int i = 1;
        int counter;
        
        cart_t *current_cart = ioopm_iterator_current(iter_carts).func_point;
        ioopm_list_t *item_values = ioopm_hash_table_keys(current_cart->items);

        printf(" \n Cart no.%d - ID: %d\n",i, current_cart->id);

        if(!ioopm_hash_table_is_empty(current_cart->items))
            {   
                printf(" \n   -Merch in cart:\n"); 
                for (counter = 0; counter < ioopm_linked_list_size(item_values); counter++)
                {
                   char *current_merch = ioopm_linked_list_get(item_values, int_elem(counter)).func_point;
                   printf("\n   * Merch no.%d : %s \n",counter+1, current_merch);
                }                
        }      
        while (ioopm_iterator_has_next(iter_carts))
                {
                    ioopm_iterator_next(iter_carts);
                    current_cart = ioopm_iterator_current(iter_carts).func_point;
                    printf(" \n Cart no.%d - ID: %d\n",i + 1, current_cart->id);
                    item_values = ioopm_hash_table_keys(current_cart->items);
                    if(!ioopm_hash_table_is_empty(current_cart->items))
                    {
                        printf(" \n   -Merch in cart:\n"); 
                        for (counter = 0; counter < ioopm_linked_list_size(item_values); counter++)
                        {
                            char *current_merch = ioopm_linked_list_get(item_values, int_elem(counter)).func_point;
                            printf("\n   * Merch no.%d : %s \n",counter+1, current_merch);
                        }
                    }
                    i++;    
                }
        ioopm_iterator_destroy(iter_carts);
    }
}



bool ioopm_list_carts(warehouse_t *warehouse)
{
    if(ioopm_linked_list_is_empty(warehouse->carts))
    {
        printf("No carts available\n");
        return true;
    }
    print_carts(warehouse);
    return true;
}