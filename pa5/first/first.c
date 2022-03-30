#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int miss, hit, i ,j, reads=0, writes=0;
unsigned long int count;
struct node** cache;
char *cachePolicy;
typedef struct node{
    unsigned long long int tag;
    int valid;
    unsigned long int time;//?
}node;

struct node** makeCache(int setNum, int assoc){
    cache = (node**)malloc(sizeof(node*)*setNum);
    for (i = 0;i < setNum; i++){
        cache[i]= (node*)malloc(sizeof(node)*assoc);
    }
    for(i = 0; i<setNum; i++){
        for(j = 0; j < assoc; j++){
            cache[i][j].valid = 0;
        }
    }
    return cache;
};

void work(unsigned long long int tagIndex, unsigned long long int setIndex, int assoc){
    int min = 0;
    for(i = 0; i < assoc; i++){
        if(cache[setIndex][i].valid == 0){
            miss++;
            reads++;
            count++;
            cache[setIndex][i].valid = 1;
            cache[setIndex][i].tag = tagIndex;
            cache[setIndex][i].time = count;
            return;
        } else {
            if(strcmp(cachePolicy, "f") == 0 && cache[setIndex][i].tag == tagIndex){
                hit++;
                return;
            }
            if(strcmp(cachePolicy, "l") == 0 && cache[setIndex][i].tag == tagIndex){
                hit++;
                count++;
                cache[setIndex][i].time = count;
                return;
            }
            if(i == (assoc-1)){
                miss++;
                reads++;
                min = 0;
                for(j = 0; j < assoc; j++){
                    if(cache[setIndex][j].time<=cache[setIndex][min].time){
                        min=j;
                    }
                }
                cache[setIndex][min].valid = 1;
                cache[setIndex][min].tag = tagIndex;
                count++;
                cache[setIndex][min].time = count;
                return;
            }
        }
    }
    return;
}

void freecache(node** cache, int setNum, int assocN){
    for(i = 0; i < setNum; i++){
        free(cache[i]);
    }
    free(cache);
    return;
};

int main(int argc, char* argv[]){
    
    if(argc !=6){
        printf("Error, wrong amount of arguments");
        return EXIT_SUCCESS;
    }

    int cSize = atoi(argv[1]);
    if(ceil(log2(cSize))!=floor(log2(cSize))){
        return EXIT_SUCCESS;
    }

    if(strcmp(argv[3], "fifo")!=0 &&strcmp(argv[3],"lru")!=0){
        return EXIT_SUCCESS;
    }

    if(strcmp(argv[3], "fifo")==0){
        cachePolicy ="f";
    }
    if(strcmp(argv[3], "lru")==0){
        cachePolicy ="l";
    }
    
    int bSize = atoi(argv[4]);
    if(ceil(log2(bSize))!=floor(log2(bSize))){
        return EXIT_SUCCESS;
    }

    char associativityTest[6] = "assoc:";
    int assoc = 0;
    int numOfSets=0;
    
    if(strcmp(argv[2], "direct") == 0){
            assoc = 1;
            numOfSets = cSize/bSize;
    }
    else if(strcmp(argv[2], "assoc") == 0){
            assoc = cSize/bSize;
            numOfSets = 1;
    }
    else{
        for(int i = 0; i < 6; i++){
            if(associativityTest[i] != argv[2][i]){
                perror("Wrong associativity");
                return (-1);
            }
        }
        sscanf(argv[2], "assoc:%d", &assoc);
        if(ceil(log2(assoc))!=floor(log2(assoc))){
                return EXIT_SUCCESS;
            }
            else{
                numOfSets = cSize/bSize/assoc;
            } 
    }
 
    FILE* file = fopen(argv[5],"r");
    if(file == NULL){
        return EXIT_SUCCESS;
    }

    int bBits = log2(bSize);
    int sBits = log2(numOfSets);
    cache = makeCache(numOfSets, assoc);
    unsigned long long int setmask =((1 << sBits) - 1);
    unsigned long long int setIndex;
    unsigned long long int tagIndex;

    //char* PC; 
    char letter;
    unsigned long long int address = 0;

    while(fscanf(file, "%c %llx", &letter, &address) != EOF){

        // if(strcmp(PC,"#eof") == 0){
        //     break;
        // }
        // else{
        setIndex = (address >> bBits) & setmask;
        tagIndex = address >> (bBits + sBits);
            if(letter =='W'){
                writes++;
                work(tagIndex,setIndex,assoc);
            }
            else if(letter =='R'){
                work(tagIndex,setIndex,assoc);
            }
            
        //}
    }

    
    printf("memread:%d\n", reads);
    printf("memwrite:%d\n", writes);
    printf("cachehit:%d\n", hit);
    printf("cachemiss:%d\n", miss);
    
    freecache(cache, numOfSets, assoc);
    fclose(file);
    return EXIT_SUCCESS;
}