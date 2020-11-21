#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    struct node* next;
    int weight; 
    char name[100];
} node;

void freenodes(struct node** array, int x);
void freeLL(struct node* head);

struct node* makenode(char n[], int weight) {
    struct node* newnode = malloc(sizeof(struct node));
    strcpy(newnode->name, n);
    newnode->next = 0;
    newnode->weight = weight;

    return newnode;
}

void connect(struct node* n, char name2[], int y) {
    while (n->next != NULL) {
        n = n->next;
    }
    n->next = makenode(name2, y);
}

void freenodes(struct node** array, int x) {
    for(int i = 0; i < x; i++) {
        freeLL(array[i]);
    }
    free(array);
}

void freeLL(struct node* head) {
   struct node* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

void print(struct node* n) {
    struct node* temp = n;
    while (n != NULL) {
        if (temp == n) {
            printf("%s ", n->name);
            n = n->next;
            continue;
        }
        printf("%s %d ", n->name, n->weight);
        n = n->next;
    }
    printf("\n");
}

int outward (struct node* n) {
    int count = 0;
    n = n->next;
    while (n != NULL) {
        count++;
        n = n->next;
    }
    return count;
}

int inward(struct node* n, char name[], int x) {
    int y = 0;
    if (strcmp(n->name, name) == 0) {
        return 0;
    }
    while (n != NULL) {
        if (strcmp(n->name, name) == 0) {
            y++;
        }
        n = n->next;
    }
    return y;
}

int compare(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

void freemat(char** mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void adjacent(struct node* n, int x) {
    char** names;
    int count = 0;
    struct node* temp = n->next;
    n = n->next;
    int i = 0;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    names = malloc(count * sizeof(char*));
    for (int z = 0; z < count; z++) {
        names[z] = malloc(x * sizeof(char));
    }
    while (n != NULL) {
        strcpy(names[i], n->name);
        i++;
        n = n->next;
    }
    qsort(names, count, sizeof(char*), compare);
    for (int j = 0; j < count; j++) {
        for (int y = 0; y < x; y++) {
            if (names[j][y] == '\0') {
                break;
            }
            printf("%c", names[j][y]);
        }
        printf(" ");
    }
    printf("\n");
    freemat(names, count);
}

int main(int argc, char* argv[argc + 1]) {
    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    FILE* file = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "r");

    char name[100];
    char name2[100];
    int x;
    int y;
    char query;
    char str[100]; 
    fscanf(file, "%d\n", &x);
    struct node** array = malloc(x * sizeof(struct node));
    for (int i = 0; i < x; i++) {
        fscanf(file, "%s\n", name);
        array[i] = makenode(name, 0);
    }

    while (fscanf(file, "%s %s %d", name, name2, &y) != EOF) {
        int num1;
        for (int i = 0; i < x; i++) {
            if (strcmp(array[i]->name, name) == 0) {
                num1 = i;
                continue;
            }
        }
        connect(array[num1], name2, y);
    }
    // for (int i = 0; i < x; i++) {
    //     print(array[i]);
    // }
    while (fscanf(file2, "%c %s\n", &query, str) != EOF) {
        if (query == 'o') {
            for (int i = 0; i < x; i++) {
                if (strcmp(array[i]->name, str) == 0) {
                    printf("%d\n", outward(array[i]));
                    break;
                }
            }
        } else if (query == 'a') {
            for (int i = 0; i < x; i++) {
                // printf("array[i]->name is %s str is %s\n", array[i]->name, str);
                if (strcmp(array[i]->name, str) == 0) {
                    adjacent(array[i], x);
                    break;
                }
            }
        } else if (query == 'i') {
            int count = 0;
            for (int i = 0; i < x; i++) {
                count += inward(array[i], str, x);
            }
            printf("%d\n", count);
        }
    }
    // for (int i = 0; i < x; i++) {
    //     print(array[i]);
    // }
    freenodes(array, x);
    return EXIT_SUCCESS;
}