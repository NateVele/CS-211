#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct node
{
    int data;
    struct node* next;
} node;

void enqueue(struct node* n, int data) {
    // printf("enqueue\n");
    struct node* newNode = malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = NULL;

    // printf("after malloc\n");

    while (n->next != NULL) {
        n = n->next;
    }

    // printf("after while\n");
    n->next = newNode;
}

void push(struct node** n, int data) {
    // printf("push\n");
    struct node* newNode = malloc(sizeof(struct node));
    newNode->data = data;
    newNode->next = *n;

    *n = newNode;
}

void pop(struct node** n) {
    // printf("pop\n");
    struct node* tmp = *n;
    while (tmp->data == INT_MAX) {
        // printf("temp is %d \n", tmp->data);
        tmp = tmp->next; 
    }
    tmp->data = INT_MAX;

    // if ((*n)->data == INT_MAX) {
    //     tmp = *n;
    //     (*n) = (*n)->next;
    //     // free(tmp);
    // }

    // printf("after skipping max\n");
    // tmp = *n;
    // (*n) = (*n)->next;
    // free(tmp);
}

void print(struct node* n) { 
    // if (n == NULL || (n->data == INT_MAX && n->next == NULL)) {
    //     printf("EMPTY\n");
    // }

    while (n != NULL) {
        if (n->data == INT_MAX) {
            n = n->next;
            continue;
        } 
        printf("%d ", n->data);
        n = n->next;
    }
    printf("\n");
}

void freeList(struct node* head) {
   struct node* tmp;

   while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main(int argc, char* argv[argc + 1]) {

    if (argc < 2) {
        return EXIT_SUCCESS;
    }
    // printf("i am here\n");

    FILE *file = fopen(argv[1], "r");

    int x;
    char str[10];

    struct node* head = malloc(sizeof(struct node));
    head->data = INT_MAX;
    head->next = NULL;

    int i = 0;

    while (fscanf(file, "%s %d\n", str, &x) != EOF) {
        // printf("str is %s x is %d\n", str, x);
        if (strcmp(str, "ENQUEUE") == 0) {
            // printf("here\n");
            enqueue(head, x);
            i++;
        }

        if (strcmp(str, "PUSH") == 0) {
            push(&head, x);
            i++;
        }

        if (i != 0 && strcmp(str, "POP") == 0) {
            pop(&head);
            i--;
        }
        if (i == 0) {
            printf("EMPTY\n");
        } else {
            print(head);
        }
    }
    fclose(file);
    freeList(head);
    return EXIT_SUCCESS;
}