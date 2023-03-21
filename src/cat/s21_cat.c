#include "s21_cat.h"

int main(int argc, char* argv[]) {
  if (argc > 1) s21_cat(argc, argv);

  return 0;
}

void s21_cat(int argc, char* argv[]) {
  options_t options = {0, 0, 0, 0, 0, 0};
  if (!parser(argc, argv, &options))
    while (optind < argc) {
      reader(argv, options);
      optind++;
    }
}

int parser(int argc, char* argv[], options_t* options) {
  int flag = 0, opt, option_index;
  const char* short_options = "+benstvET";

  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, 0, NULL, 0},
  };

  while ((opt = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
        opt = -1;
        flag = 1;
    }
  }
  if (options->b == 1) options->n = 0;
  return flag;
}

void reader(char* argv[], options_t options) {
  FILE* fp = NULL;
  fp = fopen(argv[optind], "r");
  if (fp != NULL) {
    int cur, str_count = 1, blanc = 0;
    char last = '\n';
    while ((cur = fgetc(fp)) != EOF) {
      if (options.s && cur == '\n' && last == '\n') {
        blanc++;
        if (blanc > 1) continue;
      } else {
        blanc = 0;
      }
      if (options.b && cur != '\n' && last == '\n')
        printf("%6d\t", str_count++);
      if (options.n && last == '\n') printf("%6d\t", str_count++);
      if (options.v &&
          ((cur >= 0 && cur < 9) || (cur > 10 && cur < 32) || cur == 127)) {
        if (cur == 127)
          cur = '?';
        else
          cur += 64;
        printf("^");
      }
      if (options.e && cur == '\n') printf("$");

      if (options.t && cur == 9) {
        printf("^");
        cur = 'I';
      }
      printf("%c", cur);
      last = cur;
    }
    fclose(fp);
  } else {
    fprintf(stderr, "cat: s_cat.c: No such file or directory");
  }
}
