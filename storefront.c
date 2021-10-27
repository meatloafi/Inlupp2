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
    size_t item_count;
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
  char *name = ask_question_string("Name: ");
  char *desc = ask_question_string("Description: ");
  size_t price = ask_question_int("Price: ");
  ioopm_add_merch(warehouse, name, desc, price);
}

void make_spacing()
{ 
  printf("\n----------------------------------------------------------\n");
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
  do
  {
    input = ask_question_input_int("Input: ");
      switch(input)
      {
          case 1:
          make_spacing();  
          ioopm_add_merch_interface(warehouse);
          make_spacing();  
          ioopm_list_merch(warehouse);
          make_spacing();  
          print_options_menu();
          break;

          case 2:
          make_spacing();  
          ioopm_list_merch(warehouse);
          make_spacing();
          print_options_menu();
          break;
          case 3:
          
          merch_name = ask_question_string("Which merchendise would you like to remove? \n"); //ask question merch för att kolla om svaret är gilitigt?? 
          ioopm_remove_merch(warehouse, merch_name);   
          make_spacing();                             
          ioopm_list_merch(warehouse);
          make_spacing();
          print_options_menu();
          break;
          //ioopm_edit_merch
          case 4:
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
  warehouse_t *warehouse = ioopm_warehouse_create();
  event_loop(warehouse);
  ioopm_hash_table_destroy(warehouse->items);
  return 0;
}