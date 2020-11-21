all : program
program : fifth.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 fifth.c -o fifth
clean :
	rm -rf fifth
