CC=gcc
CFLAGS=-c -Wall -Wextra -Werror

all: s21_grep

s21_grep: s21_grep.o
	$(CC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c

clean:
	rm -rf *.o s21_grep log.txt

test:
	chmod +x test_func_grep.sh
	./test_func_grep.sh

clang: 
	cp ../../materials/linters/.clang-format ./ 
	clang-format -Werror -n *.c *.h
	rm -rf .clang-format