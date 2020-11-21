#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(int** mat, int n);

void magicsquare(int** mat, int n) {
    int count = 1;
    int i = 0;
    int j = n/2;
    int num = 0;

    mat[i][j] = count;

    for (; num < n*n;) {
        if (count + 1 > n*n) {
            break;
        }
            if (i - 1 >= 0 && j + 1 <= n - 1 && mat[i - 1][j + 1] == 0) {
                i--, j++, count++;
                mat[i][j] = count;
            } else if (i - 1 >= 0 && j + 1 <= n - 1 && mat[i - 1][j + 1] != 0) { 
                i++, count++;
                mat[i][j] = count;
            } else if (i - 1 < 0 && j + 1 <= n - 1) { 
                if (mat[n - 1][j + 1] != 0) {
                    i++, count++;
                    mat[i][j] = count;
                } else {
                    i = n - 1, j++, count++;
                    mat[i][j] = count;
                }
            } else if (i - 1 < 0 && j + 1 > n - 1) {
                  if (mat[n - 1][0] != 0) {
                    i++, count++;
                    mat[i][j] = count;
                } else {
                    i = n - 1, j = 0, count++;
                    mat[i][j] = count;
                }
            } else if (i - 1 >= 0 && j + 1 > n - 1) { 
                i--, j = 0, count++;
                mat[i][j] = count;
            } 
            num++;
        }
    }

void print(int** mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d	", mat[i][j]);
        }
        printf("\n");
    }
}

void freemat(int** mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}
int main(int argc, char* argv[argc + 1]) {
    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    int n = atoi(argv[1]);

    if (n % 2 == 0) {
        printf("error\n");
        return EXIT_SUCCESS;
    }

    int** mat = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));
    }
   for (int i = 0; i < n; i++) {
       for (int j = 0; j < n; j++) {
           mat[i][j] = 0;
       }
   }
    magicsquare(mat, n);
    print(mat, n);
    freemat(mat, n);
    return EXIT_SUCCESS;
}