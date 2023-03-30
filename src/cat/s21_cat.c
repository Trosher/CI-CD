#include <ctype.h>
#include <getopt.h>
#include <stdio.h>

typedef struct flag {
  int b;  
  int e;  
  int n;  
  int s;  
  int t;  
  int v;  
} flag_search;

static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                       {"number", 0, 0, 'n'},
                                       {"squeeze-blank", 0, 0, 's'},
                                       {0, 0, 0, 0}};

void read_file(FILE *file, flag_search flag);
int find_flags(char ch, flag_search *flag);

int main(int argc, char *argv[]) {
  int flag_error = 0;
  flag_search flag = {0};
  while (!flag_error) {
    int ch = getopt_long(argc, argv, "benstvET", long_options, NULL);
    if (ch == -1) {
      break;
    } else {
      flag_error = find_flags(ch, &flag);
    }
  }
  if (flag_error == 0) {
    argc -= optind;
    argv += optind;
    for (flag_error = 0; *argv != NULL; argv++) {
      FILE *fp;
      if ((fp = fopen(*argv, "r+")) != NULL) {
        read_file(fp, flag);
        fclose(fp);
      } else {
        printf("read_file: %s No such file or directory ", *argv);
      }
    }
  }
  if (flag_error != 0) {
    printf(
        "usage: %s [-benstv --number-nonblank --number --squeeze-blank] [file "
        "...]\n",
        argv[0]);
  }
  return 0;
}

int find_flags(char ch, flag_search *flag) {
  int flags = 0;
  switch (ch) {
    case 'b':
      flag->b = 1;
      flag->n = 1;
      break;
    case 'e':
      flag->e = 1;
      flag->v = 1;
      break;
    case 'E':
      flag->e = 1;
      break;
    case 'n':
      flag->n = 1;
      break;
    case 's':
      flag->s = 1;
      break;
    case 't':
      flag->t = 1;
      flag->v = 1;
      break;
    case 'T':
      flag->t = 1;
      break;
    case 'v':
      flag->v = 1;
      break;
    case '?':
    default:
      flags = 1;
  }
  return flags;
}

void read_file(FILE *file, flag_search flags) {
  int ch, last_sym;
  int empty_str_count = 0;
  long unsigned line = 0;
  for (last_sym = '\n'; (ch = getc(file)) != EOF; last_sym = ch) {
    if (last_sym == '\n') {
      if (flags.s) {
        if (ch == '\n') {
          if (empty_str_count) {
            continue;
          }
          empty_str_count = 1;
        } else {
          empty_str_count = 0;
        }
      }
      if (flags.n && (!flags.b || ch != '\n')) {
        printf("%6lu\t", ++line);
      }
    }
    if (ch == '\n') {
      if (flags.e) printf("$");
    } else if (ch == '\t') {
      if (flags.t) {
        printf("^I");
        continue;
      }
    } else if (flags.v) {
      if (iscntrl(ch)) {
        printf("^%c", ch == '\177' ? '?' : ch | 0100);
        continue;
      }
    }
    printf("%c", ch);
  }
}