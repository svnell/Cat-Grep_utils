#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} options_t;

int parser(int argc, char* argv[], options_t* options);
void s21_cat(int argc, char* argv[]);
void reader(char* argv[], options_t options);
