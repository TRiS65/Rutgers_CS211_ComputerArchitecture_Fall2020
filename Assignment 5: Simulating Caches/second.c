#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int isPowerOf2(int value){ //return 0 if it is power of 2
    return (value & (value-1));
}

//return the max age's index
int FindMax(int start, int A, long long **cache){
    int max = start;
    for(int i = 1; i < A; i++){
        if(cache[start+i][2] > cache[max][2]){
            max = start+i;
        }
    }
    return max;
}

//return the index of hit
int FindHit(long long set, long long tag, int A, long long **cache, int mode){
    int start = set*A;
    int result = -1;
    for(int i = 0; i < A; i++){
        if(cache[start+i][0] == 1){
            cache[start+i][2] += 1; // age ++ if valid
            if(cache[start+i][1] == tag){
                if(mode == 1){ //lru -> age = 0 -> recently used
                    cache[start+i][2] = 0;
                }
                result = start+i; 
            }
        }
    }
    return result; // hit = index, miss = -1
}

//store address to L1, return address that been replaced
long long StoreToL1(long long address, int b, int s1, int A1, long long **L1){
    long long result = -1;
    long long set = (address >> b) & ((1 << s1) - 1);
    long long tag = address >> (b+s1);
    int start = set*A1;
    int place = -1;
    for(int i = 0; i < A1; i++){
        if(L1[start+i][0] == 0){
            place = start + i;
            break;
        }
    }
    if(place == -1){
        place = FindMax(start,A1,L1);
        result = L1[place][3];
    }
    L1[place][0] = 1;
    L1[place][1] = tag;
    L1[place][2] = 0;
    L1[place][3] = address;

    return result;
}

//store address to L2, no return
void StoreToL2(long long address, int b, int s2, int A2, long long **L2){
    long long set = (address >> b) & ((1 << s2) - 1);
    long long tag = address >> (b+s2);
    int place = -1;
    int start = set*A2;
    for(int i = 0; i < A2; i++){
        if(L2[start+i][0] == 0){
            place = start + i;
            break;
        }
    }
    if(place == -1){ // L2 is full
        place = FindMax(start,A2,L2);
    }
    L2[place][0] = 1;
    L2[place][1] = tag;
    L2[place][2] = 0;
    L2[place][3] = address;
}

int main(int argc, char* argv[argc+1]){
    if(argc != 9){printf("error");return EXIT_SUCCESS;}
    int C1 = atoi(argv[1]);
    int C2 = atoi(argv[5]);
    int B = atoi(argv[4]);
    if(isPowerOf2(C1) != 0 || isPowerOf2(C2) != 0 || isPowerOf2(B) != 0){
        printf("error");return EXIT_SUCCESS;}

    int A1, A2; // # of blocks
    int S1, S2; // # of sets
    int b = log2(B); // # block offset bits (L1 and L2 are same)
    int s1, s2; // # set index bits
    //for L1
    if(argv[2][0] == 'd'){
        A1 = 1; S1 = C1/B; s1 = log2(S1);
    }else if(argv[2][5] == ':'){
        A1 = atoi(&(argv[2][6])); S1 = C1/(A1*B); s1 = log2(S1);
    }else{
        A1 = C1/B; S1 = 1; s1 = 0;
    }
    //for L2 
    if(argv[6][0] == 'd'){
        A2 = 1; S2 = C2/B; s2 = log2(S2);
    }else if(argv[6][5] == ':'){
        A2 = atoi(&(argv[6][6])); S2 = C2/(A2*B); s2 = log2(S2);
    }else{
        A2 = C2/B; S2 = 1; s2 = 0;
    }

    int mode1, mode2;
    //mode in L1
    if(argv[3][0] == 'f'){
        mode1 = 0; // fifo
    }else{
        mode1 = 1; // lru
    }
    //mode in L2
    if(argv[7][0] == 'f'){
        mode2 = 0; // fifo
    }else{
        mode2 = 1; // lru
    }

    FILE* f = fopen(argv[8],"r");
    if(!f){printf("error");return EXIT_SUCCESS;}

    //cache: valid, tag, age, original address
    long long ** L1 = malloc(S1*A1 * sizeof(long long *));
    for(int i = 0; i < S1*A1; i++){
        L1[i] = malloc(4 * sizeof(long long));
        for(int j = 0; j < 4; j++){
            L1[i][j] = 0;
        }
    }
    long long ** L2 = malloc(S2*A2 * sizeof(long long *));
    for(int i = 0; i < S2*A2; i++){
        L2[i] = malloc(4 * sizeof(long long));
        for(int j = 0; j < 4; j++){
            L2[i][j] = 0;
        }
    }

    int read = 0, write = 0, l1hit = 0, l1miss = 0, l2hit = 0, l2miss = 0;
    char operation;
    long long address;
    while(fscanf(f, "%c %llx\n", &operation, &address) != EOF){
        if(operation == 'W'){
            write += 1;
        }
        long long set1 = (address >> b) & ((1 << s1) - 1);
        long long tag1 = address >> (b+s1);
        long long set2 = (address >> b) & ((1 << s2) - 1);
        long long tag2 = address >> (b+s2);
        if(FindHit(set1,tag1,A1,L1,mode1) != -1){
            l1hit += 1;
        }else if(FindHit(set2,tag2,A2,L2,mode2) != -1){
            l1miss += 1;
            l2hit += 1;
            //move hit from L2 to L1
            int index = FindHit(set2,tag2,A2,L2,mode2);
            long long sub = StoreToL1(L2[index][3],b,s1,A1,L1);
            L2[index][0] = 0;
            if(sub != -1){
                StoreToL2(sub,b,s2,A2,L2);
            }
        }else{
            l1miss += 1;
            l2miss += 1;
            read += 1;
            //memory to L1, L1 to L2
            long long sub = StoreToL1(address,b,s1,A1,L1);
            if(sub != -1){
                StoreToL2(sub,b,s2,A2,L2);
            }
        }
    }
    fclose(f);
    
    printf("memread:%d\n", read);
    printf("memwrite:%d\n", write);
    printf("l1cachehit:%d\n", l1hit);
    printf("l1cachemiss:%d\n", l1miss);
    printf("l2cachehit:%d\n", l2hit);
    printf("l2cachemiss:%d\n", l2miss);

    for(int i = 0; i < S1*A1; i++){
        free(L1[i]);
    }free(L1);
    for(int i = 0; i < S2*A2; i++){
        free(L2[i]);
    }free(L2);

    return EXIT_SUCCESS;
}
