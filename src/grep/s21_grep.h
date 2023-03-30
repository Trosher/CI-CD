#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#define BUFFSIZE 1024

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <regex.h>
#include <stdlib.h>

typedef struct flag {
  int i;  //  Игнорирует различия регистра
  int c;  //  Выводит только количество совпадающих строк.
  int l;  //  Выводит только совпадающие файлы.
  int n;  //  Предваряет каждую строку вывода номером строки из файла ввода.
  int h;  //  Выводит совпадающие строки, не предваряя их именами файлов.
  int s;  //  Подавляет сообщения об ошибках о несуществующих или нечитаемых файлах.
  int o;  //  Печатает только совпадающие (непустые) части совпавшей строки.
  int v;  //  Отображение непечатаемых символов
} flag_search;

typedef struct node {
  char str[1024];
  struct node *next;
} list;

void *first_class_create(char *str);
void class_adding(char *str, list *head);
void class_clear_all(list *head);
list *outto(char *str, list *head);
void flag_parser(int argc, char **argv, flag_search *flag, list **pattern, list **file_sourse);
void read_file(flag_search flag, list *pattern, list *file_sourse);

#endif  // SRC_GREP_S21_GREP_H_
