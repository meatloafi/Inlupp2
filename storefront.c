#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h> 
#include <string.h>
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
    char *name;
    size_t price;
    char *description;
    int id;
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
    ioopm_list_t *carts;
    size_t item_count;
    size_t cart_count;
};

struct cart
{
    ioopm_hash_table_t *items;
    size_t id;
};



void key_array_destroy(char **keys, size_t size)
{
  for (int i = 0; i < size; i++)
  {
    free (keys[i]);
  }
}


// void ioopm_add_merch(warehouse_t *warehouse)
// {
//     char *name_sd = ask_question_string("Name: ");
//     char *desc_sd = ask_question_string("Description: ");
//     size_t price = ask_question_int("Price: ");
//     char name[255] = {0};
//     char desc[255] = {0};
//     strcat(name, name_sd);
//     strcat(desc, desc_sd);
//     free(name_sd);
//     free(desc_sd);
//     int id = warehouse->item_count;

//     // char shelf_section = (ask_question_char("Shelf section: "));
//     // int shelf_number = ask_question_int("Shelf number: ");
//     char *shelf_UI = ask_question_shelf("Shelf section and number: ");
//     shelf_t *shelf = create_shelf(shelf_UI);
//     ioopm_linked_list_append(new_merch->location, ptr_elem(&shelf));

//     ioopm_hash_table_insert(warehouse->shelves, ptr_elem(new_merch->name), ptr_elem(&shelf));
    
//     if(!ioopm_hash_table_has_key(warehouse->items, ptr_elem(new_merch->name))) // TODO; Verkar inte förhindra att man kan stoppa många med samma namn
//     {
//         merch_t *new_merch = create_merch(id, name, desc, price);
//         ioopm_hash_table_insert(warehouse->items, ptr_elem(new_merch->name), ptr_elem(new_merch));
//     }
//     warehouse->item_count++;
// }


// void ioopm_list_merch(warehouse_t *warehouse)
// {
//     ioopm_list_t *items = ioopm_hash_table_keys(warehouse->items);
//     size_t items_size = ioopm_linked_list_size(items);

//     char **keys = calloc(items_size+1, sizeof(char *));
//     int i;

//     if (ioopm_linked_list_is_empty(items))
//     {
//         printf("The warehouse is empty \n");
//     }
//     else
//     {
//         for(i = 0; i < items_size; i++) // alla namn på merch till en array
//         {
//             keys[i] = ioopm_linked_list_get(items, i).extra;
//         }

//     for (i = 0; i < items_size; i++)  // printar namnen på all merch
//         {                                 // TODO: printa 20 items i taget
//             printf ("%d. %s \n",(i+1), keys[i]);
//         }
//     }
//     free(keys);
//     ioopm_linked_list_destroy(items);
// }


void ioopm_add_merch_interface(warehouse_t *warehouse)
{
  make_spacing;
  char *name = ask_question_string("Name: ");
  char *desc = ask_question_string("Description: ");
  size_t price = ask_question_int("Price: ");
  
  ioopm_add_merch(warehouse, name, desc, price);

  make_spacing;
}



void ioopm_edit_merch_interface(warehouse_t *warehouse, char *merch)
{
  char *new_name = ask_question_string("New name: ");
  char *new_desc = ask_question_string("New description: ");
  size_t new_price = ask_question_int("New price: ");
  ioopm_edit_merch(warehouse, merch, new_name, new_desc, new_price);
  make_spacing;
}

void ioopm_remove_merch_interface(warehouse_t *warehouse)
{
  char *merch_name;
  merch_name = ask_question_string("Which merchendise would you like to remove? \n");
  ioopm_remove_merch(warehouse, merch_name);
  //free(merch_name);
  make_spacing;
}

void ioopm_add_cart_interface(warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to add to (ID)?:  ");
  cart_t *cart;
  bool result = get_cart(warehouse, id, &cart);
  if(result)
  {
    char *name = ask_question_string("What merchendise do you want to add?: ");
    int quantity = ask_question_int("What quantity of the merch do you want to add?: ");
    ioopm_add_to_cart(warehouse, cart, name, quantity);
  }
  printf("The cart add was not successful. \n");
  
  make_spacing;
}

void ioopm_remove_cart_interface(warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to remove from(ID)?:  ");
  cart_t *cart;
  bool result = get_cart(warehouse, id, &cart);
  if(result)
  { char *name = ask_question_string("What merchendise do you want to remove?: ");
    int quantity = ask_question_int("What quantity of the merch do you want to remove?: ");

    result = ioopm_remove_from_cart(cart, name, quantity);
    if(result)
    {
      printf("The merch has successfully been removed from the cart!\n ");
    }
    else
    { 
      printf("The merch could not be removed from the cart\n ");
    }
    
  }
  else 
  {
    printf("The cart was not removed\n ");
  }
  make_spacing;
}

void print_options_menu()
{
  printf("1: Add merchendise\n");
  printf("2: List items in store\n");
  printf("3: Remove merchendise\n");
  printf("4: Edit merchendise\n");
  printf("5: Show stock\n");
  printf("6: Replenish\n");
  printf("7: Create cart\n");
  printf("8: Remove cart\n");
  printf("9: Add to cart\n");
  printf("10: Remove from cart\n");
  printf("11: Calculate Cost\n");
  printf("12: Checkout\n");
  printf("0: QUIT\n");
}

#define name_merch(x) (merch_t) { .name=(x) }

void event_loop(warehouse_t *warehouse)
{
  print_options_menu();
  
  int input;
  char *merch_name;
  char *shelf_name;
  int quantity;
  cart_t *cart;
  do
  {
    input = ask_question_input_int("Input: ");
      switch(input)
      {
          case 1:  
          ioopm_add_merch_interface(warehouse);
          ioopm_list_merch(warehouse);
          print_options_menu();
          break;

          case 2:
          make_spacing;  
          ioopm_list_merch(warehouse);
          print_options_menu();
          break;
          case 3:
          ioopm_remove_merch_interface(warehouse);
          // merch_name = ask_question_string("Which merchendise would you like to remove? \n"); //ask question merch för att kolla om svaret är gilitigt?? 
          // ioopm_remove_merch(warehouse, merch_name);                           
          ioopm_list_merch(warehouse);
          print_options_menu();
          break;

          case 4:
          merch_name = ask_question_string("Which merchendise would you like to edit? \n");
          ioopm_edit_merch_interface(warehouse, merch_name);
          ioopm_list_merch(warehouse);

          break;
          case 5:
          make_spacing; 
          merch_name = ask_question_string("Which merchendise would you like to show the locations of? \n");
          ioopm_show_stock(warehouse, merch_name);
          make_spacing; 
          
          break;
          case 6:
          merch_name = ask_question_string("Which merchendise would you like to replenish the stock of? \n");
          shelf_name = ask_question_shelf("Which shelf would you like to replenish? \n");
          quantity = ask_question_int("Input new quantity \n");
          bool result = ioopm_replenish_stock(warehouse, merch_name, shelf_name, quantity);
          if(result)
          {
            printf("The replenish was successfull!\n");
          }
          else
          {
            printf("The replenish was unsuccessfull.\n");
          }
          make_spacing; 
          break;
          case 7:

          cart = ioopm_cart_create(warehouse);
          int cart_id = cart->id;
          printf("A shopping cart has been created \n");
          printf("Cart ID: %d \n", cart_id);
          make_spacing; 
          break;

          case 8:
          ioopm_remove_cart_interface(warehouse);

          break;
          case 9:

          ioopm_add_cart_interface(warehouse);
          break;
          printf("TO BE IMPLEMENTED!\n"); break;
          case 10:
          //ioopm_remove_from_cart
          printf("TO BE IMPLEMENTED!\n"); break;
          case 11:
          //ioopm_calc_cart_cost
          printf("TO BE IMPLEMENTED!\n"); break;
          case 12:
          //ioopm_checkout_cart
          printf("TO BE IMPLEMENTED!\n"); break;
          case 0:
          make_spacing;
          printf("Goodbye!\n"); break; 
      }
}
while (input != 0);
// free(merch_name);
// free(shelf_name);
}


int main(int argc, char *argv[])
{
  warehouse_t *warehouse = ioopm_warehouse_create();
  event_loop(warehouse);
  ioopm_warehouse_destroy(warehouse);
  return 0;
}