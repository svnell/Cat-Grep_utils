#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFF 1024


typedef struct {
  char f_file[BUFF];
  char pattern[BUFF];
  int many_files;
  int print_file_names;
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int is_pattern;  // is option -f or -e active
} options_t;

void s21_grep(int argc, char* argv[]);

int parser(int argc, char* argv[], options_t* options);

void pattern_from_file(options_t* options, char* optarg);

void reader(char* argv[], options_t options);

void option_o(char* s, regex_t compiled, options_t options, int line_count,
              char* argv[]);

void printer(char* tmp_line, char* argv[], options_t options, int* func_launch,
             int line_count);
