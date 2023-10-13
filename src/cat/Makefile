CC=gcc
CFLAGS=-c -Wall -Wextra -Werror

all: s21_cat

s21_cat: s21_cat.o
	$(CC) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c cat_functions.h
	$(CC) $(CFLAGS) -c s21_cat.c

clean:
	rm -rf *.o s21_cat log.txt

test:
	chmod +x t_cat.sh
	./t_cat.sh

clang: 
	cp ../../materials/linters/.clang-format ./ 
	clang-format -Werror -n *.c *.h
	rm -rf .clang-format
