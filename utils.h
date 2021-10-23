#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

typedef union {
    int   int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef answer_t (*convert_func)(char *);

typedef bool (*check_func)(char *);

struct item{
    char *name;
    char *desc;
    int price;
    char *shelf;
};

typedef struct item item_t;

extern char *strdup(const char *);

int read_string(char *buf, int buf_siz);

bool is_number(char *str);

int ask_question_int(char *question);

bool not_empty(char *str);

bool is_shelf(char *str);

answer_t ask_question(char *question, check_func check, convert_func convert);

char *ask_question_string(char *question);

char *ask_question_shelf(char *question);

void print(char *str);

void println(char *str);

#endif