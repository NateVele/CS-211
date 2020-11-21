#include <stdio.h>
#include <stdlib.h>

int isPrime(int num) {
    int flag = 0;
    for (int i = 2; i < num/2; i++) {
        if (num % i == 0) {
            flag = 1;
            break;
        }
    }
    if (flag == 0) {
        return 1;
    } else if (flag == 1) {
        return 0;
    }
    return EXIT_SUCCESS;
}
int main(int argc, char* argv[argc + 1]) {
    
    if (argc < 2) {
        printf("insufficient arguments");
        return EXIT_SUCCESS;
    }

    FILE *file = fopen(argv[1], "r");

    int x, z; 

    while (fscanf(file, "%d\n", &x) != EOF) {
        if (x <= 1) {
            printf("no\n");
            continue;
        }

        z = isPrime(x);
        if (z == 1) { //x is a prime number, check to see if it is a twin prime
            if (isPrime(x+2) == 1 || isPrime(x-2) == 1) {
                printf("yes\n");
                continue;
            } else {
                printf("no\n");
                continue;
            }
        } else if (z == 0) {
            printf("no\n");
        }
    }
    return EXIT_SUCCESS;
}