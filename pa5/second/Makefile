all : program
program : second.c
	gcc -g -Wall -Werror -fsanitize=address second.c -o second -lm
clean :
	rm -rf second