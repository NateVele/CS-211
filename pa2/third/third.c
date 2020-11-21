#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    struct node* next;
    char name[100];
} node;

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

void freeLL(struct node* head); 

struct queue* makequeue(int x) {
    struct queue* queue = malloc(sizeof(struct queue));
    queue->array = malloc(x * sizeof(int));
    queue->front = 0;
    queue->rear = -1;
    queue->itemcount = 0;

    return queue;
}

struct node* makenode(char n[]) {
    struct node* newnode = malloc(sizeof(struct node));
    strcpy(newnode->name, n);
    newnode->next = 0;

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

void print(struct node* n) {
    while (n != NULL) {
        printf("%s ", n->name);
        n = n->next;
    }
    printf("\n");
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

void freenodes(struct node** array, int x) {
    for(int i = 0; i < x; i++) {
        freeLL(array[i]);
    }
    free(array);
}

void freeGraph(struct graph* G) {
    for (int i = 0; i < G->vertices; i++) {
        freeLL(G->adjlist[i]);
    }
    free(G->adjlist);
    free(G->visited);
    free(G);
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

void freequeue(struct queue* queue) {
    free(queue->array);
    free(queue);
}

void freemat(char** mat, int n) {
    printf("freeing\n");
    for (int i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}

void connect(struct node* n, struct node* n2, char name[], char name2[]) {
    // printf("in here, name is %s name2 is %s\n", name, name2);
    struct node* temp;
    struct node* temp2;
    int count = 0;
    int count2 = 0;
    while (n->next != NULL) {
        if (strcmp(n->next->name, name2) > 0) {
            temp = n->next;
            n->next = makenode(name2);
            n->next->next = temp;
            count = 1;
            break;
        } else if (strcmp(n->next->name, name2) < 0) {
            n = n->next;
            continue;
        } else if (strcmp(n->next->name, name2) == 0) {
            return;
        }
    }
    if (count == 0) {
        n->next = makenode(name2);
    }
    while (n2->next != NULL) {
        if (strcmp(n2->next->name, name) > 0) {
            temp2 = n2->next;
            n2->next = makenode(name);
            n2->next->next = temp2;
            count2 = 1;
            break;
        } else if (strcmp(n2->next->name, name) < 0) {
            n2 = n2->next;
            continue;
        }
    }
    if (count2 == 0) {
        n2->next = makenode(name);
    }
}

void BFS(struct graph* graph, int start) {
    struct queue* queue = makequeue(graph->vertices);
    for (int i = 0; i < graph->vertices; i++) {
        graph->visited[i] = 0;
    }
    graph->visited[start] = 1;
    enqueue(queue, start);
    int num;
    while(!isEmpty(queue)) {
        int curr = dequeue(queue);
        printf("%s ", graph->adjlist[curr]->name);

        struct node* temp = graph->adjlist[curr];
        while (temp != NULL) {
            for (int i = 0; i < graph->vertices; i++) {
                if (strcmp(temp->name, graph->adjlist[i]->name) == 0) {
                    num = i;
                }
            }
            int adj = num;
            if (graph->visited[adj] == 0) {
                graph->visited[adj] = 1;
                enqueue(queue, adj);
            }
            temp = temp->next;
        }
    }
    printf("\n");
    freequeue(queue);
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

int main(int argc, char* argv[argc + 1])   {

    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    FILE* file = fopen(argv[1], "r");
    FILE* file2 = fopen(argv[2], "r");

    int x;
    fscanf(file, "%d\n", &x);
    struct graph* graph = makegraph(x);
    char name[100];
    char name2[100];
    char str[100];
    for (int i = 0; i < x; i++) {
        fscanf(file, "%s\n", name);
        graph->adjlist[i] = makenode(name);     
    }

    sortlist(graph);
    while (fscanf(file, "%s %s\n", name, name2) != EOF) {
        int num1, num2;
        for (int i = 0; i < x; i++) {
            if (strcmp(graph->adjlist[i]->name, name) == 0) {
                num1 = i;
                continue;
            }
            if (strcmp(graph->adjlist[i]->name, name2) == 0) {
                num2 = i;
                continue; 
            }
        }
        connect(graph->adjlist[num1], graph->adjlist[num2], name, name2);
    }
    // for (int i = 0; i < x; i++) {
    //     print(graph->adjlist[i]);
    // }
    // printf("DONE\n");
    while (fscanf(file2, "%s\n", str) != EOF) {
        for (int i = 0; i < x; i++) {
            if (strcmp(graph->adjlist[i]->name, str) == 0) {
                BFS(graph, i);
            }
        }
    }
    
    // for (int i = 0; i < x; i++) {
    //     print(array[i]);
    // }
    freeGraph(graph);
    return EXIT_SUCCESS;
}