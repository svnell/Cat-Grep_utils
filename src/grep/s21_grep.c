#include "s21_grep.h"

int main(int argc, char* argv[]) {
  if (argc > 2) s21_grep(argc, argv);

  return 0;
}

void s21_grep(int argc, char* argv[]) {
  options_t options = {0};
  if (!parser(argc, argv, &options)) {
    if (options.is_pattern ==
        0) {  // if no -f and no -e first argv[optind] is pattern
      sprintf(options.pattern, "%s", argv[optind]);
      optind++;
    }
    if (argc - optind > 1) options.many_files = 1;  // >= 2 files
    if (options.many_files && !options.h)
      options.print_file_names = 1;  // grep should print file names
    while (optind < argc) {
      reader(argv, options);
      optind++;
    }
  }
}

int parser(int argc, char* argv[], options_t* options) {
  const char* short_options = "e:ivclnhsf:o";
  int flag = 0, opt;
  while ((opt = getopt_long(argc, argv, short_options, 0, 0)) != -1) {
    if (opt == 'e') {  // pattern
      options->e = 1;
      if ((strlen(options->pattern) + strlen(optarg)) < (BUFF - 2)) {
        if (options->is_pattern) strcat(options->pattern, "|");
        if (strlen(optarg) == 0)
          strcat(options->pattern, ".");
        else
          strcat(options->pattern, optarg);
      }
      options->is_pattern = 1;
    } else if (opt == 'i') {  // Ignore uppercase vs. lowercase.
      options->i = 1;
    } else if (opt == 'v') {  // Invert match
      options->v = 1;
    } else if (opt == 'c') {  // Output count of matching lines only
      options->c = 1;
    } else if (opt == 'l') {  // Output matching files only
      options->l = 1;
    } else if (opt == 'n') {  // Precede each matching line with a line number
      options->n = 1;
    } else if (opt == 'h') {  // Output matching lines without preceding them by
                              // file names.
      options->h = 1;
    } else if (opt == 's') {  // Suppress error messages about nonexistent or
                              // unreadable files.
      options->s = 1;
    } else if (opt == 'f') {  // Take regexes from a file.
      options->f = 1;
      pattern_from_file(options, optarg);
      options->is_pattern = 1;
    } else if (opt == 'o') {  // Output the matched parts of a matching line.
      options->o = 1;
    } else {
      flag = 1;
      fprintf(stderr, "grep: Invalid argument\n");
    }
  }
  if (options->v) options->o = 0;
  if (options->c) options->o = 0;
  if (options->l) options->o = 0;
  return flag;
}

void reader(char* argv[], options_t options) {  // -i -v
  FILE* fp = NULL;
  fp = fopen(argv[optind], "r");
  if (fp) {
    int reg_flags = REG_EXTENDED;
    regex_t compiled;
    if (options.i) reg_flags = REG_ICASE | REG_EXTENDED;  // -i
    if (regcomp(&compiled, options.pattern, reg_flags) == 0) {
      size_t len;
      int success = 1, func_launch = 0, line_count = 0;
      char* tmp_line = NULL;
      while (getline(&tmp_line, &len, fp) != -1) {
        line_count++;
        if (options.o) {
          option_o(tmp_line, compiled, options, line_count, argv);
        } else {
          if (tmp_line) success = regexec(&compiled, tmp_line, 0, 0, 0);
          if ((success == 0) && (options.v == 0)) {  // -default
            printer(tmp_line, argv, options, &func_launch, line_count);
          } else if (options.v && (success == 1)) {  // -v
            printer(tmp_line, argv, options, &func_launch, line_count);
          }
        }
      }

      if (options.c) {  // -c
        if (options.print_file_names) printf("%s:", argv[optind]);
        if (options.l && func_launch) func_launch = 1;  // -lc
        printf("%d\n", func_launch);
      }

      if (options.l && func_launch)  // -l
        printf("%s\n", argv[optind]);

      if (tmp_line != NULL) {
        free(tmp_line);
      }
      regfree(&compiled);
    }
    fclose(fp);
  } else if (!options.s) {  // -s
    fprintf(stderr, "%s: No such file or directory\n", argv[optind]);
  }
}

void printer(char* tmp_line, char* argv[], options_t options, int* func_launch,
             int line_count) {
  if (!options.l && !options.c) {
    if (options.print_file_names) printf("%s:", argv[optind]);  // -h
    if (options.n) printf("%d:", line_count);                   // -n
    char* first_newline = strchr(tmp_line, '\n');  // check for last blanc line
    if (first_newline) *first_newline = '\0';
    printf("%s", tmp_line);
    printf("\n");
  }
  (*func_launch)++;
}

void option_o(char* s, regex_t compiled, options_t options, int line_count,
              char* argv[]) {
  regmatch_t pmatch[1];
  int launch = 0;
  while (regexec(&compiled, s, 1, pmatch, 0) != REG_NOMATCH) {
    if (!launch) {
      if (options.print_file_names) printf("%s:", argv[optind]);  // -h
      if (options.n) printf("%d:", line_count);                   // -n
    }
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
      printf("%c", s[i]);
      launch++;
    }
    printf("\n");
    s += pmatch[0].rm_eo;
  }
}

void pattern_from_file(options_t* options, char* optarg) {
  FILE* fp = NULL;
  fp = fopen(optarg, "r");
  if (fp) {
    char tmp_line[2];
    int cur, prev = '\n';
    while ((cur = fgetc(fp)) != EOF) {
      if (strlen(options->pattern) < (BUFF - 2)) {
        if (options->is_pattern && (prev == '\n')) {
          strcat(options->pattern, "|");
        }
        if (cur != '\n') {
          tmp_line[0] = cur;
          strcat(options->pattern, tmp_line);
        }
        if (cur == '\n' && prev == '\n') {
          strcat(options->pattern, ".");
        }
        if (cur == '\n') options->is_pattern = 1;
        prev = cur;
      }
    }
    fclose(fp);
  } else if (!options->s) {
    fprintf(stderr, "%s: No such file or directory", options->f_file);
  }
}