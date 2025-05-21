#include<stdlib.h>
#include<stdio.h>

int isPrime(int n){
  if(n < 2){
    return 0;
  }
  for(int i = 2; i < n; i++){
    if(n % i == 0){
      return 0;
    }
  }
  return 1;
}

int isTwin(int n){
  if(isPrime(n)==1 && (isPrime(n-2)==1 || isPrime(n+2)==1)){
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[argc+1]){
  FILE* f=fopen(argv[1],"r");	
  int num;
  while(fscanf(f,"%d\n",&num)!= EOF){
    if(isTwin(num)==1){
      printf("yes\n");
    }else{
      printf("no\n");
    }
  }
  fclose(f);
  
  return EXIT_SUCCESS;
}
