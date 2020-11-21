#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct node {
    char name[100];
    struct node* next;
    int weight;
};

struct graph {
    int vertices;
    int* visited;
    struct node** adjlist;
};

struct stack {
    int* items;
    int top;
};

void topsort(struct graph* graph, struct stack* stack, int x);

struct stack* makestack(int size) {
    struct stack* stack = malloc(sizeof(struct stack));
    stack->items = malloc(size * sizeof(int));
    stack->top = -1;

    return stack;
}

struct node* makenode(char name[], int weight) {
    struct node* newnode = malloc(sizeof(struct node));
    strcpy(newnode->name, name);
    newnode->next = 0;
    newnode->weight = weight;

    return newnode;
}

struct graph* makegraph(int vertices) {
    struct graph* graph = malloc(sizeof(struct graph));
    graph->visited = malloc(vertices * sizeof(int));
    graph->vertices = vertices;
    graph->adjlist = malloc(vertices * sizeof(struct node));
    for (int i = 0; i < vertices; i++) {
        graph->adjlist[i] = NULL;
        graph->visited[i] = 0;
    }

    return graph;

}

int isEmpty(struct stack* stack) {
    return stack->top == -1;
}

void push(struct stack* stack, int num) {
    // printf("in push\n");
    stack->items[++stack->top] = num;
}

int pop(struct stack* stack) {

    if (isEmpty(stack)) {
        printf("underflow\n");
        exit(EXIT_FAILURE);
    }

    return stack->items[stack->top--];
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

void freeGraph(struct graph* G) {
    for (int i = 0; i < G->vertices; i++) {
        freeLL(G->adjlist[i]);
    }
    free(G->adjlist);
    free(G->visited);
    free(G);
}


void connect(struct node* n, char name[], int x) {
    // printf("in here, name is %s name2 is %s\n", name, name2);
    struct node* temp;
    int count = 0;
    while (n->next != NULL) {
        if (strcmp(n->next->name, name) > 0) {
            temp = n->next;
            n->next = makenode(name, x);
            n->next->next = temp;
            count = 1;
            break;
        } else if (strcmp(n->next->name, name) < 0) {
            n = n->next;
            continue;
        } else if (strcmp(n->next->name, name) == 0) {
            return;
        }
    }
    if (count == 0) {
        n->next = makenode(name, x);
    }
}

void sortlist(struct graph* graph) {
    struct node* temp;
    for (int i = 0; i < graph->vertices; i++) {
        if (i + 1 < graph->vertices) {
            if (strcmp(graph->adjlist[i + 1]->name, graph->adjlist[i]->name) < 0) {
                temp = graph->adjlist[i + 1];
                graph->adjlist[i + 1] = graph->adjlist[i];
                graph->adjlist[i] = temp;
            } else if (strcmp(graph->adjlist[i + 1]->name, graph->adjlist[i]->name) > 0) {
                continue;
            }
        } else {
            break;
        }
    }
}

void print(struct node* n) {
    while (n != NULL) {
        printf("%s ", n->name);
        n = n->next;
    }
    printf("\n");
}

int inward(struct node* node, char name[], struct graph* graph) {

    int x = 0;
    if (strcmp(node->name, name) == 0) {
        return 0;
    }
    // printf("node->name is %s\n", node->name);
    while (node != NULL) {
        // printf("pain\n");
        if (strcmp(node->name, name) == 0) {
            x++;
        }  
        node = node->next;
    }
    // printf("name is %s and y is %d\n", name, x);
    return x;
}

void updateindegree(int indegree[], struct graph* graph, int x) {
    struct node* temp = graph->adjlist[x];
    if (temp != NULL) {
        temp = temp->next;
    } else {
        return;
    }

    while (temp != NULL) {
        for (int i = 0; i < graph->vertices; i++) {
            if (strcmp(temp->name, graph->adjlist[i]->name) == 0) {
                printf("in here\n");
                indegree[i]--;
            }
        }
        temp = temp->next;
    }
}

int peek(struct stack* stack) {
    return stack->items[stack->top];
}

void dfshelper(struct graph* graph, struct stack* stack) {
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->visited[i] == 0) {
            topsort(graph, stack, i);
        }
    }
    // while (!isEmpty(stack)) {
    //     printf("%d ",pop(stack));
    // }
    // printf("\n");
}

void topsort(struct graph* graph, struct stack* stack, int x) {
    struct node* temp = graph->adjlist[x];

    graph->visited[x] = 1;
    // printf("%s ", temp->name);
    int num;

    while (temp != NULL) {
        // printf("name is %s\n", temp->name);
        for (int i = 0; i < graph->vertices; i++) {
            if (strcmp(graph->adjlist[i]->name, temp->name) == 0) {
                num = i;
                break;
            }
        }
        if (graph->visited[num] == 0) {
            topsort(graph, stack, num);
        }
        temp = temp->next;
    }
    if(!isEmpty(stack)) {
        if (peek(stack) == 0 && x == 0) {
            return;
        }
    }
    push(stack, x);
    freeLL(temp);
}

void freestack(struct stack* stack) {
    free(stack->items);
    free(stack);
}

void shortestpath(struct graph* graph, int src, int *array) {
    // printf("graph->vertices is %d\n", graph->vertices);
    // for (int i = 0; i < graph->vertices; i++) {
    //     printf("%d ", array[i]);
    //     // printf("here\n");
    // }
    int* distance = malloc(graph->vertices * sizeof(int));

    for (int i = 0; i < graph->vertices; i++) {
        distance[i] = INT_MAX;
    }

    // for (int i = 0; i < graph->vertices; i++) {
    //     print(graph->adjlist[i]);
    // }

    distance[src] = 0;
    for (int z = 0; z < graph->vertices; z++) {
        int v = array[z];

        if (distance[v] != INT_MAX) {
            int i = 1;
            struct node* temp = graph->adjlist[v];
            while (temp != NULL) {
                for (int j = 0; j < graph->vertices; j++) {
                    if (strcmp(temp->name, graph->adjlist[j]->name) == 0) {
                        i = j;
                    }
                }
                if (distance[i] > distance[v] + temp->weight) {
                    distance[i] = distance[v] + temp->weight;
                }
                temp = temp->next;
            }
        }
    }

    for (int i = 0; i < graph->vertices; i++) {
        if (distance[i] == INT_MAX) {
            printf("%s INF\n", graph->adjlist[i]->name);
            continue;
        }
        printf("%s %d\n", graph->adjlist[i]->name, distance[i]);
    }
    free(distance);
}


int main(int argc, char* argv[argc + 1]) {
    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    FILE* file = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "r");
    int x;
    int y;
    char name[100];
    char name2[100];
    fscanf(file, "%d\n", &x);

    struct graph* graph = makegraph(x);
    struct stack* stack = makestack(x);

    for (int i = 0; i < x; i++) {
        fscanf(file, "%s\n", name);
        graph->adjlist[i] = makenode(name, 0);
    }

    sortlist(graph);

    while (fscanf(file, "%s %s %d\n",name, name2, &y) != EOF) {
        int num1;
        for (int i = 0; i < x; i++) {
            if (strcmp(graph->adjlist[i]->name, name) == 0) {
                num1 = i;
                continue;
            }
        }
        connect(graph->adjlist[num1], name2, y);
    }
    int z = 0;

    for (int i = 0; i < graph->vertices; i++) {
        for (int j = 0; j < graph->vertices; j++) {
            z += inward(graph->adjlist[j], graph->adjlist[i]->name, graph);
        }
        if (z == 0) {
            break;
        }
        
    }

    if (z != 0) {
        printf("CYCLE\n");
        freeGraph(graph);
        return EXIT_SUCCESS;
    }

    dfshelper(graph, stack);
    int* topsorted;
    topsorted = malloc(x * sizeof(int));

    for (int i = 0; i < graph->vertices; i++) {
        topsorted[i] = pop(stack);
    }

    while (fscanf(file2, "%s\n", name) != EOF) {
        int num;
        for (int i = 0; i < graph->vertices; i++) {
            if (strcmp(name, graph->adjlist[i]->name) == 0) {
                num = i;
            }
        }
        // for (int i = 0; i < graph->vertices; i++) {
        //     printf("%d ", topsorted[i]);
        // }
        printf("\n");
        shortestpath(graph, num, topsorted);
    }
    printf("\n");
    // for (int i = 0; i < x; i++) {
    //     print(graph->adjlist[i]);
    // }
    free(topsorted);
    freestack(stack);
    freeGraph(graph);

    return EXIT_SUCCESS;
}