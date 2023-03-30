#include "s21_grep.h"

int main(int argc, char **argv) {
  flag_search flag = {0};
  list *pattern = NULL;
  list *file_sourse = NULL;
  flag_parser(argc, argv, &flag, &pattern, &file_sourse);
  read_file(flag, pattern, file_sourse);
  class_clear_all(pattern);
  class_clear_all(file_sourse);
  pattern = NULL;
  file_sourse = NULL;
  return 0;
}

void *first_class_create(char *str) {
  list *temp = (list *)malloc(sizeof(list));
  strcpy(temp->str, str);
  temp->next = NULL;
  return (temp);
}

void class_adding(char *str, list *head) {
  list *temp = (list *)malloc(sizeof(list));
  strcpy(temp->str, str);
  temp->next = NULL;
  list *pt = head;
  while (pt->next != NULL) {
    pt = pt->next;
  }
  pt->next = temp;
}

void class_clear_all(list *head) {
  while (head != NULL) {
    list *temp = head;
    head = head->next;
    free(temp);
  }
}

list *outto(char *str, list *head) {
  if (head != NULL) {
    class_adding(str, head);
  } else {
    head = first_class_create(str);
  }
  return head;
}

void flag_parser(int argc, char **argv, flag_search *flag, list **pattern,
                 list **file_sourse) {
  int opt = 0, file_amount = 0, pattern_line = 0;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != EOF) {
    switch (opt) {
      case 'i':
        flag->i = REG_ICASE;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      case 'e':
        *pattern = outto(optarg, *pattern);
        pattern_line++;
        break;
      case 'f': {
        FILE *file = NULL;
        if ((file = fopen(optarg, "r")) != NULL) {
          char *line = NULL;
          unsigned long plug = 0;
          long line_size = 0;
          while ((line_size = getline(&line, &plug, file)) >= 0) {
            if (line[line_size - 1] == '\n') line[line_size - 1] = '\0';
            *pattern = outto(line, *pattern);
          }
          free(line);
          fclose(file);
          file = NULL;
          pattern_line++;
        } else {
          if (!flag->s)
            printf("%s: %s: No such file or directory", argv[0], optarg);
        }
        break;
      }
      case '?':
        if (!flag->s)
          printf("usage: %s [-chilnosv] [-e pattern] [-f file]", argv[0]);
        break;
    }
  }
  for (; optind < argc; optind++) {
    if (flag->h == 0 && file_amount > 0) {
      flag->h = -1;
    }
    if (!pattern_line) {
      *pattern = outto(argv[optind], *pattern);
      pattern_line++;
    } else {
      *file_sourse = outto(argv[optind], *file_sourse);
      file_amount++;
    }
  }
}

void read_file(flag_search flag, list *pattern, list *file_sourse) {
  regex_t regex;
  unsigned long plug;
  regmatch_t regmatch;
  int one_more_flag = 0, line_swift = 0, line_number = 0;
  char *line = NULL;
  if (flag.v && flag.o) flag.o = 0;
  while (file_sourse != NULL) {
    FILE *file;
    if ((file = fopen(file_sourse->str, "r")) != NULL) {
      long line_size;
      while ((line_size = getline(&line, &plug, file)) >= 0) {
        line_number++;
        list *pattern_pointer = pattern;
        while (pattern_pointer != NULL) {
          if ((regcomp(&regex, pattern_pointer->str, flag.i)) == 0) {
            int match = regexec(&regex, line, 1, &regmatch, 0);
            if (match == 0 && one_more_flag == 0) {
              if (flag.o && !flag.v && !flag.c && !flag.l) {
                char *pt = line;
                if (flag.h == -1) printf("%s:", file_sourse->str);
                if (flag.n) printf("%d:", line_number);
                while (match == 0) {
                  for (unsigned long i = regmatch.rm_so;
                       i < (unsigned long)regmatch.rm_eo; i++)
                    printf("%c", pt[i]);
                  printf("\n");
                  pt = pt + regmatch.rm_eo;
                  match = regexec(&regex, pt, 1, &regmatch, 0);
                }
              }
              one_more_flag = 1;
            }
            pattern_pointer = pattern_pointer->next;
          }
          regfree(&regex);
        }
        if (flag.v) one_more_flag = one_more_flag == 1 ? 0 : 1;
        if (one_more_flag == 1) {
          one_more_flag = 0;
          line_swift++;
          if (line[line_size - 1] == '\n') line[line_size - 1] = '\0';
          if (!flag.c && !flag.l && !flag.o) {
            if (flag.h == -1) printf("%s:", file_sourse->str);
            if (flag.n) printf("%d:", line_number);
            printf("%s\n", line);
          }
        }
      }
      if (flag.c) {
        if (flag.l && line_swift) line_swift = 1;
        if (flag.h == -1) printf("%s:", file_sourse->str);
        printf("%d\n", line_swift);
      }
      if (flag.l && line_swift) {
        printf("%s\n", file_sourse->str);
      }
      line_number = 0;
      line_swift = 0;
      fclose(file);
      file_sourse = file_sourse->next;
    } else {
      if (!flag.s)
        fprintf(stderr, "grep: %s: No such file or directory\n",
                file_sourse->str);
      file_sourse = file_sourse->next;
    }
  }
  free(line);
  line = NULL;
}
