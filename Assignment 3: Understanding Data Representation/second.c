#include<stdlib.h>
#include<stdio.h>

long long power(long long base, int power){
  long long result = 1;
  if(power == 0){
    return result;
  }
  for(int i = 0; i < power; i++){
    result = result * base;
  }
  return result;
}

void fillarr(int* binary, long long n, int bit){
  long long curr = 0;
  if(n < 0){
    binary[0] = 1;
    curr = -power(2,bit-1);
  }else{
    binary[0] = 0;
  }
  for(int i = bit-2; i >= 0; i--){
    if(curr == n){
      binary[bit-1-i] = 0;
    }else if(curr <= n){
      if(curr+power(2,i) <= n){
        binary[bit-1-i] = 1;
        curr = curr+power(2,i);
      }else{
        binary[bit-1-i] = 0;
      }
    }else{
      printf("filling failed");
    }
  }
}

void printbinary(long long n, int bit){
  long long max = power(2,bit-1)-1; // max = 2^bit;
  long long min = -power(2,bit-1);
  if(n > max){
    n = max;
  }else if(n < min){
    n = min;
  }
  int* binary = malloc(bit * sizeof(int));
  fillarr(binary,n,bit);
  for(int i = 0; i < bit; i++){
    printf("%d",binary[i]);
  }
  printf("\n");
  free(binary);
}

int main(int argc, char* argv[argc+1]){
  FILE* f = fopen(argv[1],"r");
  long long n;
  int bit;
  while(fscanf(f, "%lld %d\n", &n, &bit) != EOF){
    printbinary(n,bit);
  }
  fclose(f);
  return EXIT_SUCCESS;
}