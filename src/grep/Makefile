CFLAGS= -Wall -Wextra -Werror -std=c11
NAME= s21_grep

all: s21_grep

s21_grep: clean
	gcc $(CFLAGS) $(NAME).c -o $(NAME)

test: 
	sh test.sh

check: cppcheck leak linter

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ../../src/grep/s21_grep.c

leak:
	CK_FORK=no leaks --atExit -- ./$(NAME) -e 123 123.txt  

linter:
	cp ../../materials/linters/.clang-format ..
	clang-format -n s21_grep.c

clean: 
	rm -rf $(NAME)