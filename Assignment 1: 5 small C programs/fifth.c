#include<stdlib.h>
#include<stdio.h>
void freeMatrix(int**,int);
void printMatrix(int**,int);
int det(int**,int);

void freeMatrix(int** mat, int n){
  for(int i=0;i<n;i++){
    free(mat[i]);
  }
  free(mat);
}

void printMatrix(int** mat, int n){
  for(int i = 0; i < n; i++){
    printf("%d",mat[i][0]);
    for(int j = 1; j < n; j++){
      printf("\t%d",mat[i][j]);
    }
    printf("\n");
  }
}

int det(int** mat, int n){
  int result=0;
  /*situation of n = 1 or 2*/
  if(n == 1){
    result = mat[0][0];
    return result;
  }
  else if(n == 2){
    result = mat[0][0]*mat[1][1]-mat[1][0]*mat[0][1];
    return result;
  }
  /*n > 2*/   
  int m = n-1;
  
  for(int i=0; i<n; i++){
    /*creat a m x m matrix*/
    int** seg = malloc(m * sizeof(int*));
    for(int j = 0; j < m; j++){
      seg[j] = malloc(m * sizeof(int));
    }
    
    int row = 1;
    for(int j = 0;j < m;j++){
          int col = 0;
      for(int k = 0; k < m; k++){
	if(col == i){col += 1;}
	seg[j][k] = mat[row][col];
	col = col+1;
      }
      row = row+1;
    }
    int temp = det(seg,m);
    if(i % 2 == 1){
      temp = -temp;
    }
    result += mat[0][i]*temp;
    freeMatrix(seg,m);
  }
  return result;
}

int main(int argc, char* argv[argc+1]){
  
  if(argc < 2){
    printf("error");
    return EXIT_SUCCESS;
  }
  FILE* f=fopen(argv[1],"r");
  
  int n;
  fscanf(f,"%d\n",&n);
  
  int** mat = malloc(n * sizeof(int*));
  for(int i = 0; i < n; i++){
    mat[i] = malloc(n * sizeof(int));
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      fscanf(f,"%d",&mat[i][j]);
    }
    fscanf(f,"\n");
  }
  fclose(f);
  
  int result = det(mat,n);
	
  printf("%d\n",result);

  freeMatrix(mat,n);
  
  return EXIT_SUCCESS;
}
