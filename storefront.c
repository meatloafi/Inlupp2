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


void ioopm_add_merch_interface(ioopm_list_t *strdup_list, warehouse_t *warehouse)
{
  make_spacing;
  char *name = ask_question_string("Name: ");
  char *desc = ask_question_string("Description: ");
  size_t price = ask_question_int("Price: ");

  ioopm_linked_list_append(strdup_list, ptr_elem(name));

  ioopm_linked_list_append(strdup_list, ptr_elem(desc));

  ioopm_add_merch(warehouse, name, desc, price);

  make_spacing;
}



void ioopm_edit_merch_interface(ioopm_list_t *strdup_list, warehouse_t *warehouse, char *merch)
{
  char *new_name = ask_question_string("New name: ");
  char *new_desc = ask_question_string("New description: ");
  size_t new_price = ask_question_int("New price: ");
  ioopm_edit_merch(warehouse, merch, new_name, new_desc, new_price);
  ioopm_linked_list_append(strdup_list, ptr_elem(new_name));
  ioopm_linked_list_append(strdup_list, ptr_elem(new_desc));

  make_spacing;
}

void ioopm_remove_merch_interface(ioopm_list_t *strdup_list, warehouse_t *warehouse)
{
  char *merch_name;
  merch_name = ask_question_string("Which merchendise would you like to remove? \n");
  ioopm_remove_merch(warehouse, merch_name);
  ioopm_linked_list_append(strdup_list, ptr_elem(merch_name));
  make_spacing;
}

void ioopm_add_cart_interface(ioopm_list_t *strdup_list, warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to add to (ID)?:  ");
  cart_t *cart;
  bool result = get_cart(warehouse, id, &cart);
  if(result)
  {
    char *name = ask_question_string("What merchendise do you want to add?: ");
    int quantity = ask_question_int("What quantity of the merch do you want to add?: ");
    ioopm_linked_list_append(strdup_list, ptr_elem(name));

    ioopm_add_to_cart(warehouse, cart, name, quantity);
  }
  else
  {
  printf("The cart add was not successful. \n");
  }

  make_spacing;
}

void ioopm_remove_cart_interface(warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to remove?:  ");
  bool result = ioopm_remove_cart(warehouse->carts, id);
  if(result)
  {
    printf("The cart with ID: %d has been removed.", id);
  }
  else
  {
    printf("The cart remove was not successful. \n");
  }
  // cart_t *cart;
  // bool result = get_cart(warehouse, id, &cart);
  // if(result)
  // {
  //   ioopm_remove_cart(warehouse, cart);
  // }
  // else
  // {
  // printf("The cart remove was not successful. \n");
  // }

  make_spacing;
}

void ioopm_remove_from_cart_interface(ioopm_list_t *strdup_list, warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to remove from(ID)?:  ");
  cart_t *cart;
  bool result = get_cart(warehouse, id, &cart);
  if(result)
  { char *name = ask_question_string("What merchendise do you want to remove?: ");
    int quantity = ask_question_int("What quantity of the merch do you want to remove?: ");
    ioopm_linked_list_append(strdup_list, ptr_elem(name));

    result = ioopm_remove_from_cart(warehouse, cart, name, quantity);
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


void ioopm_calculate_cost_interface(warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to calculate the cost of:  ");
  cart_t *cart;
  bool result = get_cart(warehouse, id, &cart);
  if(result)
  {
    ioopm_calc_cost_cart(warehouse, cart);
    size_t total_price = ioopm_calc_cost_cart(warehouse, cart);
    printf("The price of all items in cart: %ld is %ld. \n", cart->id, total_price);
  }
  else
  {
  printf("The cost calculation was not successful. \n");
  }

  make_spacing;
}

void ioopm_checkout_cart_interface(warehouse_t *warehouse)
{
  make_spacing;
  int id = ask_question_int("Which cart do you wish to checkout?:  ");
  cart_t *cart;
  bool result = get_cart(warehouse, id, &cart);
  if(result)
  {
    ioopm_checkout_cart(warehouse, cart);
    printf("The cart id:%ld was checked out!\n", cart->id);
    ioopm_remove_cart(warehouse->carts, id);
  }
  else
  {
  printf("The checkout was not successful. \n");
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
  printf("11: List carts and their contents\n");
  printf("12: Calculate Cost\n");
  printf("13: Checkout\n");
  printf("0: QUIT\n");
}

#define name_merch(x) (merch_t) { .name=(x) }

void event_loop(ioopm_list_t *strdup_list, warehouse_t *warehouse)
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
          ioopm_add_merch_interface(strdup_list, warehouse);
          ioopm_list_merch(warehouse);
          print_options_menu();
          break;

          case 2:
          make_spacing;  
          ioopm_list_merch(warehouse);
          print_options_menu();
          break;
          case 3:
          ioopm_remove_merch_interface(strdup_list, warehouse);  

          ioopm_list_merch(warehouse);
          print_options_menu();
          break;

          case 4:
          merch_name = ask_question_string("Which merchendise would you like to edit? \n");
          ioopm_linked_list_append(strdup_list, ptr_elem(merch_name));
          ioopm_edit_merch_interface(strdup_list, warehouse, merch_name);
          ioopm_list_merch(warehouse);
          print_options_menu();

          break;
          case 5:
          make_spacing; 
          merch_name = ask_question_string("Which merchendise would you like to show the locations of? \n");
          ioopm_linked_list_append(strdup_list, ptr_elem(merch_name));
          ioopm_show_stock(warehouse, merch_name);
          make_spacing; 
          print_options_menu();
          
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
          ioopm_linked_list_append(strdup_list, ptr_elem(merch_name));
          ioopm_linked_list_append(strdup_list, ptr_elem(shelf_name)); 
          make_spacing; 
          print_options_menu();
          break;
          case 7:

          cart = ioopm_cart_create(warehouse);
          int cart_id = cart->id;
          printf("A shopping cart has been created \n");
          printf("Cart ID: %d \n", cart_id);
          make_spacing; 
          print_options_menu();

          break;
          case 8:
          ioopm_remove_cart_interface(warehouse);
          print_options_menu();
          break;

          case 9:
          ioopm_add_cart_interface(strdup_list, warehouse);
          ioopm_list_carts(warehouse);
          make_spacing; 
          print_options_menu();

          break;
          case 10:

          ioopm_remove_from_cart_interface(strdup_list, warehouse);
          ioopm_list_carts(warehouse);
          make_spacing; 
          print_options_menu();
          break;

          case 11:

          ioopm_list_carts(warehouse);
          make_spacing; 
          print_options_menu();
          break;
          
          case 12:
          ioopm_calculate_cost_interface(warehouse);
          print_options_menu();
          break;
          
          case 13:
          ioopm_checkout_cart_interface(warehouse);
          print_options_menu();
          break;

          case 0:
          make_spacing;
          printf("Goodbye!\n"); break; 
      }
}
while (input != 0);

}

void duplicate_destroy(ioopm_list_t *strdup_list)
{
  if(!ioopm_linked_list_is_empty(strdup_list))
  {
  ioopm_list_iterator_t *iter = ioopm_list_iterator(strdup_list);
  elem_t current_elem = ioopm_iterator_current(iter);
  char *current_duplicate = current_elem.func_point;
  free(current_duplicate);

  while (ioopm_iterator_has_next(iter))
  {
      ioopm_iterator_next(iter);
      elem_t current_elem = ioopm_iterator_current(iter);
      char *current_duplicate = current_elem.func_point;
      free(current_duplicate);
  }
  ioopm_iterator_destroy(iter);
  }
}

int main(int argc, char *argv[])
{
  ioopm_list_t *strdup_list = ioopm_linked_list_create(NULL);
  warehouse_t *warehouse = ioopm_warehouse_create();
  event_loop(strdup_list, warehouse);
  duplicate_destroy(strdup_list);
  ioopm_linked_list_destroy(strdup_list);
  ioopm_warehouse_destroy(warehouse);
  return 0;
}

