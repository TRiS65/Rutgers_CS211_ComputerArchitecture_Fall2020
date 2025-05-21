#include<stdlib.h>
#include<stdio.h>
void printMatrix(int** mat, int n){
  for(int i = 0; i < n; i++){
    printf("%d",mat[i][0]);
    for(int j = 1; j < n; j++){
      printf("\t%d",mat[i][j]);
    }
    printf("\n");
  }
}

void freeMatrix(int** mat, int n){
  for(int i=0;i<n;i++){
    free(mat[i]);
  }
  free(mat);
}

int main(int argc, char* argv[argc+1]){
  
  int n = atoi(argv[1]);
  
  if(n % 2 == 0){
    printf("error");
    return EXIT_SUCCESS;
  }
   
  int** mat = malloc(n * sizeof(int*));
  for(int i = 0; i < n; i++){
    mat[i] = malloc(n * sizeof(int));
  }
  
  int col = n/2;
  int row = 0;
  for(int i = 1; i <= n*n; i++){
    mat[row][col] = i;
    int a = (row - 1) >= 0 ? row-1 : n-1;
    int b = (col + 1) % n;
    if(mat[a][b] > 0){ a = (row+1) % n; b = col;}
    row = a;
    col = b;
  }
   
  printMatrix(mat,n);
  freeMatrix(mat,n);
  
  return EXIT_SUCCESS;
}
