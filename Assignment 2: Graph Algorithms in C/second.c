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
int outDegree(struct node* head){
  int degree = 0;
  if(head == 0){
    return degree;
  }
  struct node* curr = head;
  while(curr->next != 0){
    curr = curr->next;
    degree += 1;
  }
  return degree;
}
int inDegree(struct node** gph, int n, char* vertex){
  int degree = 0;
  for(int i = 0; i < n; i++){
    struct node* curr = gph[i]->next;
    while(curr != 0){
      if(strcmp(curr->name, vertex) == 0){
        degree += 1;
      }
      curr = curr->next;
    }
  }
  return degree;
}
void printNode(struct node* head){  
  if(head == 0){
    printf("empty\n");
    return;
  }
  if(head->next == 0){
    printf("\n");
    return;
  }
  struct node* curr = head->next;
  printf("%s", curr->name);
  while(curr->next != 0){
    printf(" %s", curr->next->name);
    curr = curr->next;
  }
  printf("\n");
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

  /*add edges*/
  char v1[20];
  char v2[20];
  int edge;
  while(fscanf(f1, "%s %s %d\n", v1, v2, &edge) != EOF){
    struct node* head = gph[findHead(gph,n,v1)];
    insert(head,v2,edge);
  }
  fclose(f1);

  /*address file2 - query*/

  FILE* f2 = fopen(argv[2],"r");
  char query;
  while(fscanf(f2, "%c %s\n", &query, vertex) != EOF){
    if(query == 'i'){
      int degree = inDegree(gph,n,vertex);
      printf("%d\n",degree);
    }else{
      struct node* head = gph[findHead(gph,n,vertex)];
      if(query == 'o'){
	      int degree = outDegree(head);
	      printf("%d\n", degree);
      }else if(query == 'a'){
	      printNode(head);
      }
    }
  }
  fclose(f2);

  freeGph(gph,n);
  return EXIT_SUCCESS;
}
