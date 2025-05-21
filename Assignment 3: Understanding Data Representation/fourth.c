#include<stdlib.h>
#include<stdio.h>

double power(double base, int power){ //power > 0
  double result = 1;
  if(power == 0){
    return result;
  }
  if(power > 0){
    for(int i = 0; i < power; i++){
      result = result * base;
    }
  }else{
    power = -power;
    for(int i = 0; i < power; i++){
      result = result / base;
    }
  }
  return result;
}

int findLargest(long long n){
  int num = 0;
  while(n > 0){
    n = n/2;
    num ++;
  }
  return num;
}

void IntegralPart(double n, int* binary, int num){
  if(num == 0){
    return;
  }
  for(int i = num-1; i >= 0; i--){
    if(n <= 0){
      binary[num-1-i] = 0;
    }else if(n >= power(2,i)){
      binary[num-1-i] = 1; 
      n = n - power(2,i);
    }else{
      binary[num-1-i] = 0;
    }
  }
}

void FractionPart(double n, int* m, int total, int num){
  for(int i = 0; i < total-num; i++){
    if(n <= 0){
      m[num+i] = 0;
    }else if(n >= power(2,-i-1)){
      m[num+i] = 1;
      n = n - power(2,-i-1);
    }else{
      m[num+i] = 0;
    }
  }
}

int Zero(double n, int* m, int total){
  int E = 0;
  int i = 0;
  int dot = 1;
  while(i < total){
    if(n <= 0){ //filling other places
      m[i] = 0; i++; dot++;
    }else if(n < power(2,-dot)){ //adding zeros
      if(m[0] != 1){ // if m[0] != 1, jump this dot, goto next digit
        dot++; E--;
      }else{
        m[i] = 0; i++; dot++;
      }
    }else if(n >= power(2,-dot)){
      m[i] = 1; n = n - power(2,-dot); i++; dot++;
    }
  }
  E--;
  return E;
}


void printbinary(double n, int bit){
  long long integral = (long long)n;
  double fraction = n - (double)integral;
  int E = 0;
  int num = findLargest(integral);

  int total = 1 + bit;
  if(num > total){
    total = num;
  }
  int* binary = malloc(total * sizeof(int));
  if(integral > 0){ // integral > 0
    E = num - 1;
    IntegralPart(n,binary,num);
    FractionPart(fraction,binary,total,num);
  }else if(integral == 0){ // integral == 0
    E = Zero(fraction,binary,total);
  }

  for(int i = 0; i < bit+1; i++){
    printf("%d",binary[i]);
    if(i == 0){
      printf(".");
    }
  }  
  free(binary);

  printf(" %d\n",E);
}

int main(int argc, char* argv[argc+1]){
  FILE* f = fopen(argv[1],"r");
  double n;
  int bit;
  
  while(fscanf(f, "%lf %d\n", &n, &bit) != EOF){
    printbinary(n,bit);
  }
  fclose(f);
  
  return EXIT_SUCCESS;
}

