#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int HexLength(long long n){
  int length = 0;
  while(n > 0){
    n = n / 16;
    length ++;
  }
  return length;
}

void Hex(long long n, int* binary, int length){
  long long reminder;
  int size = 4*length;
  for(int i = 1; i <= length; i++){
    reminder = n % 16;
    for(int j = 3; j >= 0; j--){
      if(reminder >= (int)pow(2,j)){
        binary[(size-4*i)+3-j] = 1;
        reminder -= (int)pow(2,j);
      }else{
        binary[(size-4*i)+3-j] = 0;
      }
    }
    n = n / 16;
  }
}

void adjust(int* binary, int* hex, int total, int length){
  for(int i = 1; i <= total; i++){
    if(length-i >= 0){
      binary[total-i] = hex[length-i];
    }else{
      binary[total-i] = 0;
    }
  }
  for(int i = 0; i< total; i++){
  }
}

void converting(int* binary, int exponent, int fraction, int bit){
  double exp = 0;
  for(int i = 0; i < exponent; i++){
    if(binary[i+1] == 1){
      exp += pow(2,exponent-1-i);
    }
  }
  double m = 1; 
  if(exp == 0){ m = 0; }
  for(int i = 0; i < fraction; i++){
    if(binary[exponent+1+i] == 1){
      m += pow(2,-i-1);
    }
  }

  double bias = pow(2,exponent-1)-1;
  double E;
  if(exp == 0){ E = pow(2,1-bias); }
  else{ E = pow(2,exp-bias); }

  double result = m * E;
  if(binary[0] == 1){ result = -result; }

  printf("%.*f\n",bit,result);
}

void printing(long long n, int total, int exp, int frac, int bit){
  int length = HexLength(n);
  int* hex = malloc(4*length * sizeof(int));
  Hex(n,hex,length);
  //adjusting length
  int* binary = malloc(total * sizeof(int));
  adjust(binary,hex,total,length*4);
  
  //converting
  converting(binary,exp,frac,bit);
  free(hex);
  free(binary);
}

int main(int argc, char* argv[argc+1]){
  FILE* f = fopen(argv[1],"r");
  int total;
  int exp;
  int frac;
  long long n;
  int bit;

  while(fscanf(f, "%d %d %d %llx %d\n", &total,&exp,&frac,&n, &bit) != EOF){
    printing(n, total, exp, frac, bit);
  }
  fclose(f);
  
  return EXIT_SUCCESS;
}