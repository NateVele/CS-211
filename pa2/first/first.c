#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    struct node* next;
    char name[100];
} node;

void freeLL(struct node* head); 
int compare(const void *a, const void *b);

struct node* makenode(char n[]) {
    struct node* newnode = malloc(sizeof(struct node));
    strcpy(newnode->name, n);
    newnode->next = 0;

    return newnode;
}

void print(struct node* n) {
    while (n != NULL) {
        printf("%s ", n->name);
        n = n->next;
    }
    printf("\n");
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

void freemat(char** mat, int n) {
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void connect(struct node* n, struct node* n2, char name[], char name2[]) {
    while (n->next != NULL) {
        n = n->next;
    }
    n->next = makenode(name2);
    while (n2->next != NULL) {
        n2 = n2->next;
    }
    n2->next = makenode(name);
}

int degree(struct node* n) {
    int count = 0;
    n = n->next;
    while (n != NULL) {
        n = n->next;
        count++;
    }
    return count;
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

int compare(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

int main(int argc, char* argv[argc + 1])   {

    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    FILE* file = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "r");

    int x;
    fscanf(file, "%d\n", &x);
    struct node** array = malloc(x * sizeof(struct node));
    char name[100];
    char name2[100];
    char query;
    char str[100];

    for (int i = 0; i < x; i++) {
        fscanf(file, "%s\n", name);
        array[i] = makenode(name);     
    }
    while (fscanf(file, "%s %s\n", name, name2) != EOF) {
        int num1, num2;
        for (int i = 0; i < x; i++) {
            if (strcmp(array[i]->name, name) == 0) {
                num1 = i;
                continue;
            }
            if (strcmp(array[i]->name, name2) == 0) {
                num2 = i;
                continue; 
            }
        }
        connect(array[num1], array[num2], name, name2);
    }
    while (fscanf(file2, "%c %s\n", &query, str) != EOF) {
        if (query == 'd') {
            for (int i = 0; i < x; i++) {
                if (strcmp(array[i]->name, str) == 0) {
                    printf("%d\n",degree(array[i]));
                    break;
                }
            }
        } else if (query == 'a') {
            for (int i = 0; i < x; i++) {
                if (strcmp(array[i]->name, str) == 0) {
                    adjacent(array[i], x);
                    break;
                }
            }
        }
    }
    freenodes(array, x);
    return EXIT_SUCCESS;
}