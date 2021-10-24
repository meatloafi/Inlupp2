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

void print_merch (merch_t *merch)
{

    printf (" Name: %s \n Desc: %s \n Price: %d \n", merch -> name, merch -> description, merch -> price);
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

void event_loop(ioopm_hash_table_t *warehouse)
{
  print_options_menu();
  
  int input;
  do
  {
    input = ask_question_input_int("Input: ");
      switch(input)
      {
          case 1:

          ioopm_add_merch(warehouse);
          break;
          case 2:
          ioopm_list_merch(warehouse);
          break;

          case 3:
          {
            char *merchname = ask_question_string("Which merchandise do you wish to remove?: ");
            elem_t requested_merch = {.extra = merchname};
            bool result;
            ioopm_remove_merch(warehouse, merch_to_remove, &result);
            if (!result)
            {
              printf("No merchenandise found with the name %s \n", merchname); break;
            }
            //printf("TO BE IMPLEMENTED!\n"); 
          }
          case 4:
          //ioopm_edit_merch
          printf("TO BE IMPLEMENTED!\n"); break;
          case 5:
          //ioopm_show_stock
          printf("TO BE IMPLEMENTED!\n"); break;
          case 6:
          //ioopm_replenish_stock
          printf("TO BE IMPLEMENTED!\n"); break;
          case 7:
          //ioopm_create_cart
          printf("TO BE IMPLEMENTED!\n"); break;
          case 8:
          //ioopm_remove_cart
          printf("TO BE IMPLEMENTED!\n"); break;
          case 9:
          //ioopm_add_to_cart
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
          printf("Hejdå!\n"); break; 
      }
}
while (input != 0);
}

int main(int argc, char *argv[])
{
  ioopm_hash_table_t *warehouse = ioopm_hash_table_create(0, 0, NULL, NULL, NULL);
  event_loop(warehouse);
  return 0;
}