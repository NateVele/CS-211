#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct node* makenode(char name[], int weight) {
    struct node* newnode = malloc(sizeof(struct node));
    strcpy(newnode->name, name);
    newnode->next = 0;
    newnode->weight = weight;

    return newnode;
}

void dfs(struct graph* graph, int x);

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

void print(struct node* n) {
    while (n != NULL) {
        printf("%s ", n->name);
        n = n->next;
    }
    printf("\n");
}

void dfshelper(struct graph* graph) {
    for (int i = 0; i < graph->vertices; i++) {
        if (graph->visited[i] == 0) {
            dfs(graph, i);
        }
    }
    printf("\n");
}

void dfs(struct graph* graph, int x) {
    struct node* temp = graph->adjlist[x];

    graph->visited[x] = 1;
    printf("%s ", temp->name);

    while (temp != NULL) {
        int num;
        // printf("name is %s\n", temp->name);
        for (int i = 0; i < graph->vertices; i++) {
            if (strcmp(graph->adjlist[i]->name, temp->name) == 0) {
                num = i;
                break;
            }
        }
        if (graph->visited[num] == 0) {
            dfs(graph, num);
        }
        temp = temp->next;
    }
    freeLL(temp);
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

int main(int argc, char* argv[argc + 1]) {

    if (argc < 2) {
        return EXIT_SUCCESS;
    }

    FILE* file = fopen(argv[1], "r");
    // FILE* file2 = fopen(argv[2], "r");
    int x;
    int y;
    char name[100];
    char name2[100];
    fscanf(file, "%d\n", &x);

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

    // for (int i = 0; i < x; i++) {
    //     print(graph->adjlist[i]);
    // }

    dfshelper(graph);

    
    freeGraph(graph);
    return EXIT_SUCCESS;
}