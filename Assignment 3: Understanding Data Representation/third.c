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

void fillSigned(int* binary, long long n, int bit){
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

void fillUnsigned(int* binary, long long n, int bit){
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

void ReadBinary(int* binary, int bit, char dest){
  long long result = 0;
  if(binary[0] == 1){
    result = power(2,bit-1);
    if(dest == 's'){
      result = -result;
    }
  }
  for(int i = bit-2; i >= 0; i--){
    if(binary[bit-1-i] == 1){
      result = result + power(2,i);
    }
  }
  printf("%lld\n",result);
}

void DecimalToBinary(long long n, int bit, char src, char dest){
  int* binary = malloc(bit * sizeof(int));
  long long max;
  long long min;
  if(src == 'u'){
    max = power(2,bit);
    min = 0;
    if(n >= max){
      n = n % max;
    }else if(n < 0){
      n = 0;
    }
    fillUnsigned(binary,n,bit);
  }else{
    max = power(2,bit-1)-1;
    min = -power(2,bit-1);
    if(n > max){
      n = max;
    }else if(n < min){
      n = min;
    }
    fillSigned(binary,n,bit);
  }
  ReadBinary(binary,bit,dest);
  free(binary);
}

int main(int argc, char* argv[argc+1]){
  FILE* f = fopen(argv[1],"r");
  long long n;
  int bit;
  char src;
  char dest;
  while(fscanf(f, "%lld %d %c %c\n", &n, &bit, &src, &dest) != EOF){
    DecimalToBinary(n,bit,src,dest);
  }
  fclose(f);
  return EXIT_SUCCESS;
}