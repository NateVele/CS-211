#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct Item
{
    long long tag;
    long long L2tag;
} Item;

typedef struct Queue
{
    int front;
    int rear;
    int size;
    int capacity;
    Item *items;

} Queue;

typedef struct node {
    long long tag;
    struct node* next;
    struct node* prev;
    

} node;

int mRead = 0;
int mWrite = 0;
int cMiss = 0;
int L2cMiss = 0;
int L2cHit = 0;
int cHit = 0;
int cSize = 0;
int bSize = 0;
int L2assoc = 0;
int L2nSets = 0;
int L2cSize = 0;
char cPolicy[20] = "";
char L2cPolicy[20] = "";
char associativity[20] = "";
char L2associativity[20] = "";
int assoc = 0;
int nSets = 0;
int tBits = 0;
int L2tBits = 0;
int offset = 0;
int cbits = 0;
int L2offset;
int L2cBits = 0;
int x = 0;
int L2x = 0;
int y = 0;
int L2y = 0;
int nodecount = 0;
int thisnum;
int myrandom = 0;
int L2setindex;
void reposition(Queue *queue, long long tag);
void enqueueL2(Queue *queue, Item *storage);
void cacheReadWrite(char *in, Queue *Queue, struct Queue *Queue2, Item *Storage, long long tag, char* cPolicy, char* associativity, long long L2tag);
void Fifo(unsigned long long int address, char *in, Queue *array, Queue *L2array);
void cacheReadWriteL2(Queue *Queue, long long tag, char* cPolicy, char* associativity, struct Queue *Queue2, long long L2tag, Item* Storage);
void repositionL2(Queue* queue, long long L2tag);

bool checkCapacity(node *list) {
    if (nodecount == assoc) {
        return true;
    }
    return false;
}

char *convert(char *name, char *binary, int length)
{
    int i = 0;
    // printf("%s", name);
    while (i < length)
    {
        switch (name[i])
        {
        case '0':
            strcat(binary, "0000");
            break;
        case '1':
            strcat(binary, "0001");
            break;
        case '2':
            strcat(binary, "0010");
            break;
        case '3':
            strcat(binary, "0011");
            break;
        case '4':
            strcat(binary, "0100");
            break;
        case '5':
            strcat(binary, "0101");
            break;
        case '6':
            strcat(binary, "0110");
            break;
        case '7':
            strcat(binary, "0111");
            break;
        case '8':
            strcat(binary, "1000");
            break;
        case '9':
            strcat(binary, "1001");
            break;
        case 'a':
            strcat(binary, "1010");
            break;
        case 'b':
            strcat(binary, "1011");
            break;
        case 'c':
            strcat(binary, "1100");
            break;
        case 'd':
            strcat(binary, "1101");
            break;
        case 'e':
            strcat(binary, "1110");
            break;
        case 'f':
            strcat(binary, "1111");
            break;
        default:
            break;
        }

        i++;

        if (i == length)
        {
            strcat(binary, "\0");
        }
    }

    return binary;
}

char *getVal(char *binary, char *src, int x, int y)
{
    strncpy(src, (binary + x), y);

    return src;
}

Queue *assignValues(Queue *queue, int assoc)
{
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = assoc;
    queue->items = malloc(sizeof(Item) * assoc); // * assoc
    return queue;
}

Queue *createQueue(int assoc, int nSets, int index)
{
    // printf("index is %d nsets is %d\n", index, nSets);
    Queue *array = malloc(sizeof(Queue) * nSets);

    while (index < nSets)
    {
        // printf("here\n");
        Queue *queue = malloc(sizeof(Queue));
        queue = assignValues(queue, assoc);
        // printf("%d\n", queue->size);
        array[index] = *queue;
        index++;
        free(queue);
    }

    return array;
}

bool searchq(Queue *queue, int index, int tag)
{
    // printf("queue->size is %d", queue->size);
    if (queue->size == 0)
    {
        return false;
    }
    else
    {
        // int index = queue->capacity - queue->size;
        
        while (index < queue->size) //queue->size
        {
            Item *ptr = queue->items;
            int data = ptr[index].tag; //count goes everywhere index would b 

            if (data == tag)
            {
                thisnum = index;
                return true;
            }
            index++;
        }
    }

    return false;
}

bool searchqL2(Queue *queue, int index, long long tag)
{
    // printf("queue->size is %d", queue->size);
    
    if (queue->size == 0) //queue->size == 0
    {
        return false;
    }
    else
    {
        // int index = queue->capacity - queue->size;
        while (index < queue->size) //queue->size
        {
            Item *ptr = queue->items;
            long long data = ptr[index].L2tag; 
            // printf("data is %lld tag is %lld set index is %d\n", data, tag, L2setindex);
            if (data == tag)
            {
                thisnum = index;
                return true;
            }
            index++;
        }
    }

    return false;
}

int resetq(Queue *queue)
{
    if ((queue->capacity - queue->rear) == 0)
    {
        // queue->front = 0;
        // queue->size = 0;
        return 0;
    }

    return queue->rear;
}

void dequeue(Queue *queue, Queue *queue2)
{
    // printf("in dequeue size is %d rear is %d\n", queue->size, queue->rear);
    if (queue->size == 0)
    {
        printf("Error: queue is empty");
        return;
    }
    // printf("ITEMS BEFORE DEQUEUE\n");
    // for (int i = 0; i < queue->size; i++) {
    //     printf("%lld ", queue->items[i].tag);
    // }
    // printf("\n");
    Item storage = queue->items[0]; //queue->size - 1
    // for (int i = 0; i < queue->rear; i++) {
    //     queue->items[i] = queue->items[i + 1];
    // }
    // printf("ENQUUE %lld to L2\n", storage.L2tag);
    enqueueL2(queue2, &storage);
    queue->front++;
    queue->size--; 
    // queue->rear--; //THIS IS NEW
    // printf("ITEMS AFTER DEQUEUE\n");
    // for (int i = 0; i < queue->size; i++) {
    //     printf("%lld ", queue->items[i].tag);
    // }
    // printf("\n");
    // printf("in dequeue size is %d rear is %dPART 2\n", queue->size, queue->rear);

    if (queue->size == 0)
    {
        queue->front = 0;
        queue->rear = -1; // -1
    }
}

void dequeueL2(Queue *queue)
{
    // printf("in dequeue size is %d rear is %d\n", queue->size, queue->rear);
    if (queue->size == 0)
    {
        printf("Error: queue is empty");
        return;
    }
    // for (int i = 0; i < queue->rear; i++) {
    //     queue->items[i] = queue->items[i + 1];
    // }
    queue->front++;
    // queue->rear--; //NEW
    queue->size--; 
    // printf("in dequeue size is %d rear is %dPART 2\n", queue->size, queue->rear);

    if (queue->size == 0)
    {
        queue->front = 0;
        queue->rear = -1;
    }
}

void enqueue(Queue *queue, Item *item, Queue *queue2)
{
    if (queue->size == queue->capacity) //queue->size
    {
        dequeue(queue, queue2);
    }

    // if (queue->front == 0 || queue->size == queue->capacity) {
    //     dequeue(queue, queue2);
    // }

    queue->rear++;
    queue->front--;
    queue->rear = resetq(queue);
    queue->items[queue->rear] = *item;
    // printf("%lld has been enqueued at %d TO L1\n", item->L2tag, queue->rear);
    queue->size++;
}

void enqueueL2(Queue *queue, Item *item)
{
    if (queue->size == queue->capacity) //queue->size
    {
        dequeueL2(queue);
    }

    // if (queue->front == 0 || queue->size == queue->capacity - 1) {
    //     dequeueL2(queue);
    // }

    queue->rear++;
    queue->front--;
    queue->rear = resetq(queue);
    queue->items[queue->rear] = *item;
    // printf("%lld has been enqueued at %d at index %d in L2\n", item->L2tag, queue->rear, L2setindex);
    queue->size++;
    // printf("size is %d rear is %d\n", queue->size, queue->rear);
}


void cacheReadWrite(char *in, struct Queue *Queue,struct Queue *Queue2, Item *Storage, long long tag, char* cPolicy, char* associativity, long long L2tag)
{
    // printf("here queue size is %d\n", Queue->size);
    if (searchq(Queue, 0, tag) == false)
    {
        // random = 0;
        cMiss++;
        if (strcmp(in, "W") == 0)
        {
            mWrite++;
        }  
        cacheReadWriteL2(Queue, tag, cPolicy, associativity, Queue2, L2tag, Storage);
        enqueue(Queue, Storage, Queue2);
        // if (myrandom == 0) {
        //     enqueue(Queue, Storage, Queue2);
        // }       
    }
    else if (searchq(Queue, 0, tag) == true)
    {
        cHit++;
        if (strcmp(in, "W") == 0)
        {
            mWrite++;
        }
        if (strcmp(cPolicy, "lru") == 0 && strcmp(associativity, "direct") != 0) {
            reposition(Queue, tag);
        }
    }
}

void cacheReadWriteL2(Queue *Queue, long long tag, char* cPolicy, char* associativity, struct Queue *Queue2, long long L2tag, Item* Storage)
{
    if (searchqL2(Queue2, 0, Storage->L2tag) == false)
    {
        L2cMiss++;
        mRead++;
        myrandom = 0;
    }
    else if (searchqL2(Queue2, 0, Storage->L2tag) == true) //THIS IS ALL ORIGINALLY L2TAG
    {
        // printf("HIT tag is %lld\n", L2tag);
        L2cHit++;
        repositionL2(Queue2, L2tag);
        enqueue(Queue, Storage, Queue2);
        myrandom = 1;
    }
}

void repositionL2(Queue* queue, long long L2tag) {
    // printf("FIRST queue->size is %d\n", queue->size);
    Item temp = queue->items[thisnum];
    for (int i = thisnum; i > 0; i--) {
        queue->items[i] = queue->items[i - 1];
    }
    // for (int i = 0; i < queue->rear; i++) {
    //     queue->items[i] = queue->items[i + 1];
    // }
    // printf("capacity is %d size is %d\n", queue->capacity, queue->size);
    queue->items[0] = temp; //capacity - size ??
    dequeueL2(queue);
    // printf("queue->size is %d\n", queue->size);
}

void reposition(Queue *queue, long long tag) {
    
    Item temp = queue->items[thisnum];

    // printf("NEW MOVE thisnum is %d rear is %d\n", thisnum, queue->rear);
    for (int i = thisnum; i < queue->rear; i++) { //best shot so far
        queue->items[i] = queue->items[i + 1];
        // printf("moving queue->items[i] = %lld from spot %d to spot %d\n", queue->items[i].tag, i + 1, i);
        // count = 1;
    }

    if (thisnum > queue->rear) {
        queue->rear++;
        queue->items[queue->rear] = temp;
        return;
    } 
    queue->items[queue->rear] = temp;
    return;
}

void Fifo(unsigned long long int address, char *in, Queue *array, Queue *L2array)
{
    // char delete = name[2];
    // char *hex = strchr(name, delete);
    // int hex = (int)strtol(name, NULL, 0);
    // char *mybinary;
    // int raisedoffset = (int)pow(2, offset);
    // printf("before %x\n", address);
    address = address | 0x000000000000;
    // printf("%x\n", address);
    int raisedindex = (int)pow(2, cbits);
    int raisedtbits = (int)pow(2, tBits);
    // int L2raisedoffset = (int)pow(2, L2offset);
    int L2raisedindex = (int)pow(2, L2cBits);
    int L2raisedtbits = (int)pow(2, L2tBits);
    // int blockoffset = hex & (raisedoffset - 1);
    int setindex = (address >> offset) & (raisedindex - 1);
    int L1tag = (address >> (offset + cbits)) & (raisedtbits - 1);
    // int L2blockoffset = hex & (L2raisedoffset - 1);
    L2setindex = (address >> L2offset) & (L2raisedindex - 1);
    int L2tag = (address >> (L2offset + L2cBits)) & (L2raisedtbits - 1);
    
    Item* storage = malloc(sizeof(Item));
    storage->tag = L1tag;
    storage->L2tag = L2tag;
    
    // free(tag);

    Queue* QueueA = &array[setindex];

    Queue* QueueB = &L2array[L2setindex];
    // printf("set %d of L2\n", L2indexNum);
    // printf("queue size = %d", QueueB->size);
    cacheReadWrite(in, QueueA, QueueB, storage, L1tag, cPolicy, associativity, L2tag);
    free(storage);
}

bool twoCheck(int n)
{
    if ((n && (!(n & (n - 1)))) == 0)
    {
        return false;
    }

    return true;
}

bool getnums(int argc, char *argv[])
{
    if (argc != 9)
    {
        return 1;
    }
    else
    {
        cSize = atoi(argv[1]);
        bSize = atoi(argv[4]);
        L2cSize = atoi(argv[5]);


        strcpy(cPolicy, argv[3]);
        strcpy(L2cPolicy, argv[7]);
        strcpy(associativity, argv[2]);
        strcpy(L2associativity, argv[6]);
        // printf("cPolicy = %s, associativity = %s, L2associativity = %s, L2cPolicy = %s", cPolicy, associativity, L2associativity, L2cPolicy);
        if (strcmp(associativity, "direct") == 0)
        {
            // printf("cSize %d bSize %d\n", cSize, bSize);
            nSets = cSize / bSize;
            assoc = 1;
        }
        else if (strncmp(associativity, "assoc:", 6) == 0)
        {
            // printf("in here\n");
            char *tok = strtok(associativity, ":");
            tok = strtok(NULL, ":");

            int n = atoi(tok);

            assoc = n;
            nSets = (cSize / (bSize * assoc));

            if (!twoCheck(n))
            {
                return false;
            }
        }
        else
        {
            // printf("imhere\n");
            nSets = 1;
            assoc = cSize / bSize;
        }

        if (!twoCheck(cSize) || !twoCheck(bSize))
        {
            return false;
        }

        offset = log(bSize) / log(2);
        cbits = log(nSets) / log(2);
        y = cbits + 1;
        tBits = 48 - cbits - offset;
        x = tBits + 1;
        // printf("cbits is %d offset is %d tbits is %d nsets is %d\n", cbits, offset, tBits, nSets);

        if (strcmp(L2associativity, "direct") == 0)
        {
            L2nSets = L2cSize / bSize;
            L2assoc = 1;
        }
        else if (strncmp(L2associativity, "assoc:", 6) == 0)
        {
            // printf("in here\n");
            char *L2tok = strtok(L2associativity, ":");
            L2tok = strtok(NULL, ":");

            int L2n = atoi(L2tok);

            L2assoc = L2n;
            L2nSets = (L2cSize / (bSize * L2assoc));

            if (!twoCheck(L2n))
            {
                return false;
            }
        }
        else
        {
            // printf("imhere\n");
            L2nSets = 1;
            L2assoc = L2cSize / bSize;
        }

        if (!twoCheck(L2cSize) || !twoCheck(bSize))
        {
            return false;
        }

        L2offset = log(bSize) / log(2);
        L2cBits = log(L2nSets) / log(2);
        L2y = L2cBits + 1;
        L2tBits = 48 - L2cBits - L2offset;
        L2x = L2tBits + 1;
    }
    return true;
}

void freequeue(Queue* queue) {
    // printf("freeing q\n");
    for (int i = 0; i < nSets; i++)
        {
            free(&queue[i].items->tag);
            // free(queue[i].items);
            // free(&queue[i]);
        }
    free(queue);
}

void freequeueL2(Queue* queue) {
    // printf("freeing q\n");
    for (int i = 0; i < L2nSets; i++)
        {
            free(&queue[i].items->tag);
            // free(queue[i].items);
            // free(&queue[i]);
        }
        
    free(queue);
}

int main(int argc, char *argv[])
{
    if (getnums(argc, argv))
    {
        FILE *file;

        file = fopen(argv[8], "r");

        if (file == NULL)
        {
            printf("file not found\n");
            return EXIT_SUCCESS;
        }

        Queue *myqueue = createQueue(assoc, nSets, 0);
        Queue *L2queue =  createQueue(L2assoc, L2nSets, 0);
        // printf("%d\n", myqueue[0]->size);
        char in[5];
        // char name[20];
        unsigned long long int address;
        while (fscanf(file, "%s %llx", in, &address) != EOF)
        {
            Fifo(address, in, myqueue, L2queue);
        }

        printf("memread:%d\n", mRead);
        printf("memwrite:%d\n", mWrite);
        printf("l1cachehit:%d\n", cHit);
        printf("l1cachemiss:%d\n", cMiss);
        printf("l2cachehit:%d\n", L2cHit);
        printf("l2cachemiss:%d\n", L2cMiss);

        freequeue(myqueue);
        freequeueL2(L2queue);
        fclose(file);
    }
    else
    {
        printf("error\n");
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}
