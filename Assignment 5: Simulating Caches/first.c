#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int isPowerOf2(int value){ //return 0 if it is power of 2
    return (value & (value-1));
}

int FindHit(long long set, long long tag, int A, long long **cache, int mode){
    int start = set*A;
    int result = 0;
    for(int i = 0; i < A; i++){
        if(cache[start+i][0] == 1){
            cache[start+i][2] += 1; // age ++ if valid
            if(cache[start+i][1] == tag){
                if(mode == 1){ //lru -> age = 0
                    cache[start+i][2] = 0;
                }
                result = 1; 
            }
        }else{ // else are all not valid
            break;
        }
    }
    return result; // hit = 1, miss = 0
}

int replace(int start, int A, long long **cache){
    int max = start;
    for(int i = 1; i < A; i++){
        if(cache[start+i][2] > cache[max][2]){
            max = start+i;
        }
    }
    return max;
}

void store(long long set, long long tag, int A, long long **cache){
    int start = set*A;
    int place = -1;
    for(int i = 0; i < A; i++){
        if(cache[start+i][0] == 0){
            place = start + i;
            break;
        }
    }
    if(place == -1){
        place = replace(start,A,cache);
    }
    cache[place][0] = 1;
    cache[place][1] = tag;
    cache[place][2] = 0;
}

int main(int argc, char* argv[argc+1]){
    if(argc != 6){printf("error");return EXIT_SUCCESS;}

    int C = atoi(argv[1]);
    if(isPowerOf2(C) != 0){printf("error");return EXIT_SUCCESS;}
    int B = atoi(argv[4]);
    if(isPowerOf2(B) != 0){printf("error");return EXIT_SUCCESS;}
    int A; // # of blocks
    int S; // # of sets
    int b; // # block offset bits
    int s; // # set index bits
    if(argv[2][0] == 'd'){
        A = 1;
        S = C/B;
        b = log2(B);
        s = log2(S);
    }else if(argv[2][5] == ':'){
        A = atoi(&(argv[2][6]));
        S = C/(A*B);
        b = log2(B);
        s = log2(S);
    }else{
        A = C/B;
        S = 1;
        b = log2(B);
        s = 0;
    }
    int mode;
    if(argv[3][0] == 'f'){
        mode = 0; // fifo
    }else{
        mode = 1; // lru
    }

    FILE* f = fopen(argv[5],"r");
    if(!f){printf("error");return EXIT_SUCCESS;}


    //cache: valid, tag, age
    long long ** cache = malloc(S*A * sizeof(long long *));
    for(int i = 0; i < S*A; i++){
        cache[i] = malloc(3 * sizeof(long long));
        for(int j = 0; j < 3; j++){
            cache[i][j] = 0;
        }
    }
    int read = 0, write = 0, hit = 0, miss = 0;
    char operation;
    long long address;
    while(fscanf(f, "%c %llx\n", &operation, &address) != EOF){
        if(operation == 'W'){
            write += 1;
        }
        long long set = (address >> b) & ((1 << s) - 1);
        long long tag = address >> (b+s);
        if(FindHit(set,tag,A,cache,mode) == 1){
            hit += 1;
        }else{
            miss += 1;
            read += 1;
            store(set,tag,A,cache);
        }
    }
    fclose(f);
    
    printf("memread:%d\n", read);
    printf("memwrite:%d\n", write);
    printf("cachehit:%d\n", hit);
    printf("cachemiss:%d\n", miss);

    for(int i = 0; i < S*A; i++){
        free(cache[i]);
    }
    free(cache);
    return EXIT_SUCCESS;
}