all : program
program : first.c
	gcc -g -Wall -Werror -fsanitize=address -std=c11 first.c -o first
clean :
	rm -rf first