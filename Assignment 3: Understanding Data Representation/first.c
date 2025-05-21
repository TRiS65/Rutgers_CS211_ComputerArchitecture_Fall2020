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
  for(int i = bit-1; i >= 0; i--){
    if(n == 0){
      binary[bit-1-i] = 0;
    }else if(n >= power(2,i)){
      binary[bit-1-i] = 1; 
      n = n - power(2,i);
    }else{
      binary[bit-1-i] = 0;
    }
  }
  if(n != 0){
    printf("filling failed\n");
  }
}

void printbinary(long long n, int bit){
  long long max = power(2,bit); // max = 2^bit;
  if(n >= max){
    n = n % max;
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