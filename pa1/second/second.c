#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct node {
    int data;
    struct node* next;
} node;

void insert(struct node** n_ref, struct node* newNode) {

    struct node* curr;
    if (*n_ref == NULL || (*n_ref)->data >= newNode->data) {
        newNode->next = *n_ref;
        *n_ref = newNode;
    } else {
        curr = *n_ref;
        while (curr->next != NULL && curr->next->data < newNode->data) {
            curr = curr->next;
        }
        newNode->next = curr->next;
        curr->next = newNode;
    }
}

struct node* makeNode(int num){
    struct node* newNode = malloc(sizeof(struct node));
    newNode->data = num;
    newNode->next = NULL;

    return newNode;
}

void delete(struct node** n, int num) {

    struct node* temp = *n, *prev;

    if (temp != NULL && temp->data == num) {
        //printf("here1\n");
        *n = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != num) {
        //printf("here2\n");
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        //printf("here13\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
}

void print(struct node* n) { 

    if (n == NULL || (n->data == INT_MAX && n->next == NULL)) {
        printf("EMPTY");
    }

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

int duplicate(struct node* n, int num) {

    while (n != NULL) {
        if (n->data == num) {
            return 1;
        }
        n = n->next;
    }
    return 0;
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

    struct node* head = malloc(sizeof(struct node));
    head->data = INT_MAX;
    head->next = NULL;

    FILE *file = fopen(argv[1], "r");
    
    int x;
    char str[10];

    while (fscanf(file, "%s %d\n", str, &x) != EOF) {
        //printf("the value of x is %d and value of str is %s\n", x, str);

        if (strcmp(str, "INSERT") == 0) {
            if (head == NULL) {
                //printf("i am here\n");
                head->data = x;
            }
            if (duplicate(head, x) == 0) {
                struct node* newNode;
                newNode = makeNode(x);
                insert(&head, newNode);
            }
        } else if (strcmp(str, "DELETE") == 0) {
            //printf("in delete\n");
            delete(&head, x);
        }
        print(head);
    }
    fclose(file);
    freeList(head);
    return EXIT_SUCCESS;
}