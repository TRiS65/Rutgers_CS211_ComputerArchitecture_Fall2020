#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct node{
  char name[20];
  int edge;
  struct node* next;
};

struct node* allocate(char* v, int e){
  struct node* temp = malloc(sizeof(struct node));
  strcpy(temp->name,v);
  temp->edge = e;
  temp->next = 0;
  return temp;
}

int findHead(struct node** gph,int n, char* v){
  for(int i = 0; i < n; i++){
    if(strcmp(gph[i]->name,v) == 0){
      return i;
    }
  }
  printf("Head %s not found\n", v);
  return n;
}

void insert(struct node* head,char* vertex,int edge){
  if(head == 0){
    printf("insert failed\n");
    return;
  }
  struct node* temp = allocate(vertex,edge);
  if(head->next == 0){
    head->next = temp;
    return;
  }
  struct node* prev = head; 
  struct node* curr = prev->next;
  while(curr != 0 && strcmp(vertex,curr->name) > 0){
    prev = curr;    
    curr = curr->next;
  }
  if(curr == 0){ /*add in the end(vertex > all)*/
    prev->next = temp; 
  }else if(strcmp(curr->name,vertex) == 0){ /*without dupicate( == )*/
    free(temp);
    return;
  }else{ /*add before curr(vertex < curr)*/
    prev->next = temp;
    temp->next = curr;
  }

  return;
}

/*creat a empty boolean array with all F == 1 */
void arrayStart(int* arr, int n){
  for(int i = 0; i < n; i++){
    arr[i] = 1;
  }
}

void DFS(int h,struct node** gph,int n, int* visted){
  if(visted[h] == 0){
    return;
  }
  struct node* head = gph[h];
  visted[h] = 0;
  
  for(struct node* adj = head->next; adj != 0; adj = adj->next){
    char* nxt = adj->name;
    int num = findHead(gph,n,nxt);
    if(visted[num] == 1){
      printf(" %s",nxt);
      DFS(num,gph,n,visted);
    }
  }
}

void printDFS(struct node** gph,int n){
  int* visted = malloc(n*sizeof(int));
  arrayStart(visted,n); 
  for(int i = 0; i < n; i++){
    if(i == 0){
      printf("%s", gph[0]->name);
      DFS(i,gph,n,visted);
      continue;
    }
    if(visted[i] == 1){
      printf(" %s", gph[i]->name);
      DFS(i,gph,n,visted);
    }
  }
  printf("\n");
  free(visted);
}

void freeNodes(struct node* head){
  struct node* curr = head;
  while(head->next != 0){
    head = head->next;
    free(curr);
    curr = head;
  }
  free(head);
}
void freeGph(struct node** gph, int n){
  for(int i = 0; i < n; i++){
    freeNodes(gph[i]);
  }
  free(gph);
}

int main(int argc, char* argv[argc+1]){
  /*address file1 - graph*/
  /*add vertex*/
  FILE* f1 = fopen(argv[1],"r");
  int n;
  fscanf(f1, "%d\n", &n);
  struct node** gph = malloc(n * sizeof(struct node*));
  char vertex[20];
  for(int i = 0; i < n; i++){
    fscanf(f1, "%s\n", vertex);
    gph[i] = allocate(vertex,0);
  }
  //change order of gph
  struct node* temp = 0;
  for(int i = 0; i < n-1; i++){
    for(int j = i+1; j < n; j++){
      if(strcmp(gph[i]->name,gph[j]->name)>0){
        temp = gph[i];
        gph[i] = gph[j];
        gph[j] = temp;
      }
    }
  }

  /*add edges*/
  char v1[20];
  char v2[20];
  int edge;
  while(fscanf(f1, "%s %s %d\n", v1, v2, &edge) != EOF){
    struct node* head = gph[findHead(gph,n,v1)];
    insert(head,v2,edge);
  }
  fclose(f1);

  printDFS(gph,n);

  freeGph(gph,n);
  return EXIT_SUCCESS;
}
