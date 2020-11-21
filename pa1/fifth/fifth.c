#include <stdio.h>
#include <stdlib.h>

// int** temp;

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

int twobytwo(int** mat, int n) {
    int num1 = mat[0][0] * mat[1][1];
    int num2 = mat[0][1] * mat[1][0];

    return num1 - num2;
}

int** makemat(int** mat, int n, int count) {
    int x = 0;
    int row = 1;
    int** mat2 = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        mat2[i] = malloc(n * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        // printf("i is %d\n", i);
        x = 0;
        for (int j = 0; j < n; j++) {
            // printf("j is %d\n", j);
            if (count == j) {
                x++;
            }
            // printf("mat[i + 1][x] is %d\n", mat[i + 1][x]);
            mat2[i][j] = mat[i + 1][x];
            x++;
        }
        row++;
    }
    // print(mat2, n);
    return mat2;
}

int determinant(int** mat, int n) {
    // printf("size of mat is %d and mat is :\n", n);
    // print(mat, n);

    int x = -1;
    int count = 0;
    int result = 0;

    if (n == 1) {
        return mat[0][0];
    } else if (n == 2) {
        return twobytwo(mat, n);
    } else if (n > 2) {
        int num = n - 1;
        for (int j = 0; j < n; j++) {
            // printf("result is %d\n", result); 
            // printf("num is %d\n", num);
            int** temp;

            temp = makemat(mat, num, count);
            // print(temp, num);
            // printf("here\n");
            result += ((-1)*x) * (mat[0][j] * determinant(temp, num));
            count++;
            x = x*(-1);
            freemat(temp, num);
            // printf("here2\n");
            // printf("result is %d and count is %d\n", result, count);
        }
        return result;
    }
    return EXIT_SUCCESS;
}

int main (int argc, char* argv[argc + 1]) {
    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    FILE* file = fopen(argv[1], "r");

    int n;
    int result;

    fscanf(file, "%d\n", &n);
    // printf("n is %d\n", n);

    int** mat = malloc(n * sizeof(int*));
    
    for (int i = 0; i < n; i++) {
        mat[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d\n", &mat[i][j]);
        }
    }
    // printf("Here\n");
    result = determinant(mat, n);
    printf("%d\n", result);
    fclose(file);
    freemat(mat, n);
    return EXIT_SUCCESS;
}