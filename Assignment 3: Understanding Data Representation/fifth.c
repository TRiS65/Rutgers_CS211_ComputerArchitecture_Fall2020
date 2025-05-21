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

int IntegralPart(double a, int* m, int num, int bit){
  int E = num-1;
  if(num == 0){
    return E;
  }
  double n = a;
  int end = 0; 
  if(num > bit){
    end = num-bit;
  }
  for(int i = num-1; i >= end; i--){
    if(n <= 0){
      m[num-1-i] = 0;
    }else if(n >= power(2,i)){
      m[num-1-i] = 1; 
      n = n - power(2,i);
    }else{
      m[num-1-i] = 0;
    }
  }
  //if need rounding
  if(num > bit-1){
    double diffdown = n;
    double diffup = diffdown-1;
    if(diffup < 0){ diffup = -diffup;}
    long long even = (long long)a % 2;
    if(diffup < diffdown || (diffup == diffdown && even == 1)){ //+1
      for(int i = bit-1; i >= 0; i--){
        if(m[i] == 1){ m[i] = 0; }
        else{ m[i] = 1; break; }
        if(i == 0){
          E++;
        }
      }
    }
  }
  
  return E;
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
  //rounding if n > 0;
  if (n > 0 && num < total){
    double diffup = n - power(2,-total+num);
    if(diffup < 0){ diffup = -diffup; }
    double diffdown = n;
    if(diffup < diffdown || (diffup == diffdown && m[total-1] == 1)){ //+1
      for(int i = total-1; i >= 0; i--){
        if(m[i] == 1){ m[i] = 0;}
        else{ m[i] = 1; return;}
      }
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
  //rounding if n > 0;
  if (n > 0){
    double diffup = n - power(2,-dot);
    if(diffup < 0){diffup = -diffup;}
    double diffdown = n;
    if(diffup < diffdown || (diffup == diffdown && m[total-1] == 1)){ //+1
      for(int i = total-1; i >= 0; i--){
        if(m[i] == 0){
          m[i] = 1;
          break;
        }else{
          m[i] = 0;
        }
      }
    }
  }
  return E;
}

void fillarr(int* binary, int n, int bit){
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

void printIEEE(double n, int bit, int exp, int frac){
  //print sign
  if(n < 0){
    printf("1");
    n = -n;
  }else{
    printf("0");
  }
  long long integral = (long long)n;
  double fraction = n - (double)integral;
  int E = 0;
  int num = findLargest(integral);
  int total = 1 + frac; // total = array's length of frac + 1
  int* m = malloc(total * sizeof(int));

  //check denomalized, E = 0
  double bias = power(2,exp-1)-1;
  double minNor = power(2,1-bias);
  if(n >= minNor){ // normalized
    if(integral > 0){ // integral > 0
      E = IntegralPart(n,m,num,total);
      FractionPart(fraction,m,total,num);
    }else if(integral == 0){ // integral == 0
      E = Zero(fraction,m,total);
    }
    E = E + bias;
  }else{ // denormalized
    E = Zero(fraction,m,total);
    E = 0;
  }

  //printing exponent part
  int* exponent = malloc(exp * sizeof(int));
  fillarr(exponent,E,exp);
  for(int i = 0; i < exp; i ++){
    printf("%d",exponent[i]);
  }
  free(exponent);

  //printing fraction part
  int start = 1; 
  int end = frac+1;
  if(n<minNor){
    start = 0;
    end = frac;
  }
  for(int i = start; i < end; i++){
    printf("%d",m[i]);
  }
  printf("\n");
  free(m);

}

int main(int argc, char* argv[argc+1]){
  FILE* f = fopen(argv[1],"r");
  double n ;
  int total ;
  int exp ;
  int frac ;
  
  while(fscanf(f, "%lf %d %d %d\n", &n, &total, &exp, &frac) != EOF){
    printIEEE(n,total,exp,frac);
  }
  fclose(f);
  
  return EXIT_SUCCESS;
}

