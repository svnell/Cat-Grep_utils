
CFLAGS= gcc -Wall -Wextra -Werror -std=c11
NAME= s21_cat

all: s21_cat

s21_cat: clean
	$(CFLAGS) $(NAME).c -o $(NAME)   

check: cppcheck leak linter

test_fu: 
	sh test.sh

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ../../src/cat/

leak:
	CK_FORK=no leaks --atExit -- ./$(NAME)   

linter:
	cp ../../materials/linters/.clang-format ..
	clang-format -n *.c *.h
	rm -rf ../.clang-format

clean: 
	rm -rf $(NAME)