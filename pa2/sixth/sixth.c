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

struct queue {
    int* array;
    int front;
    int rear;
    int itemcount;
};

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

struct queue* makequeue(int x) {
    struct queue* queue = malloc(sizeof(struct queue));
    queue->array = malloc(x * sizeof(int));
    queue->front = 0;
    queue->rear = -1;
    queue->itemcount = 0;

    return queue;
}

int peek(struct queue* queue) {
    return queue->array[queue->front];
}

int isEmpty(struct queue* queue) {
    return queue->itemcount == 0;
}

void enqueue(struct queue* queue, int data) {
    queue->array[++queue->rear] = data;
    queue->itemcount++;
}

int dequeue(struct queue* queue) {
    int data = queue->array[queue->front++];
    queue->itemcount--;
    return data;
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
    fscanf(file, "%d\n", &x);
    char name[100];
    char name2[100];
    int y;
    int* array;
    array = malloc(x * sizeof(int));
    struct graph* graph = makegraph(x);

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
    
    for (int i = 0; i < x; i++) {
        array[i] = i;
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
        shortestpath(graph, num, array);
        printf("\n");
    }
    // for (int i = 0; i < x; i++) {
    //     print(graph->adjlist[i]);
    // }
    freeGraph(graph);
    free(array);
    return EXIT_SUCCESS;
}