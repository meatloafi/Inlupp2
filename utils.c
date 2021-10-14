#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <ctype.h>
#include "utils.h"

void clear_input_buffer()
{
    int c;
    do
    {
        c = getchar();
    }
    while (c != '\n' && c != EOF);
}

int read_string(char *buf, int buf_siz)
{
    int i = 0;
    char c = getchar();
    while (c != '\n' && c != EOF && i < buf_siz)
    {
        buf[i] = c;
        c = getchar();
        i++;
    }
    buf[i] = '\0';
    return i;
}

bool is_number(char *str)
{
    for(int i = 0; i < strlen(str); i++)
    {
        if(!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool is_shelf(char *str)
{
    if (!isalpha(str[0])) { return false; }
    if (strlen(str) < 2 || strlen(str) > 3) { return false; }
    for(int i = 1; i < strlen(str); i++)
    {
        
        if(!isdigit(str[i]))
        {
          return false;
        }
    }
    return true;
}

bool not_empty(char *str)
{
  return strlen(str) > 0;
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
    int buf_siz = 30;
    char buf[buf_siz];
    do
    {
        memset(buf, '0', buf_siz);
        printf("%s", question);
        read_string(buf, buf_siz);
    }
    while(!check(buf));
    return convert(buf);
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func) strdup).string_value;
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func) atoi);
    return answer.int_value;
}

char *ask_question_shelf(char *question)
{
    return ask_question(question, is_shelf, (convert_func) strdup).string_value;
}

void print(char *str)
{
    int i = 0;
    while(str[i] != '\0' && str[i] != '\n')
    {
        putchar(str[i]);
        i++;
    }
}

void println(char *str)
{
    print(str);
    putchar('\n');
}
