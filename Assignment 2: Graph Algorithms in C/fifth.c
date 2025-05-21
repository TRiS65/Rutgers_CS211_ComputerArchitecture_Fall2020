#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<limits.h>

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

/*Stack: push-addEnd; pop-deleteEnd*/
struct node* push(struct node* head, char* val){
  struct node* temp = allocate(val,0);
  /*empty*/
  if(head == 0 ){
    return temp;
  }
  /* add in the end*/
  struct node* curr = head; 
  while(curr->next != 0){
    curr = curr->next;
  }
  curr->next = temp;
  return head;
}
char* pop(struct node* head){
  if(head == 0){
    printf("stack empty, pop failed");
    return 0;
  }
  struct node* curr = head; 
  while(curr->next != 0){
    curr = curr->next;
  }
  return curr->name;
}
struct node* deleteEnd(struct node* head){
  if(head == 0){
    printf("stack empty, delete failed");
    return 0;
  }
  if(head->next == 0){
    free(head);
    return 0;
  }
  struct node* curr = head; 
  struct node* prev = 0;
  while(curr->next != 0){
    prev = curr;
    curr = curr->next;
  }
  prev->next = 0;
  free(curr);
  return head;
}

/*creat a empty boolean array with all F == 1 */
void arrayStart(int* arr, int n){
  for(int i = 0; i < n; i++){
    arr[i] = 1;
  }
}

/*check cycle*/
int isCycleUtil(int v, int n, struct node** gph, int* visted, int* recStack){
  if(recStack[v] == 0){
    return 0;
  }
  if(visted[v] == 0){
    return 1;
  }
  visted[v] = 0;
  recStack[v] = 0;

  struct node* head = gph[v];
  for(struct node* adj = head->next; adj != 0; adj = adj->next){
    int nxt = findHead(gph,n,adj->name);
    if(isCycleUtil(nxt,n,gph,visted,recStack) == 0){
      return 0;
    }
  }
  recStack[v] = 1;
  return 1; 
}
int isCycle(struct node** gph,int n){
  int* visted = malloc(n*sizeof(int));
  int* recStack = malloc(n*sizeof(int));
  arrayStart(visted,n);
  arrayStart(recStack,n);
  for(int i = 0; i < n; i++){
    if(isCycleUtil(i,n,gph,visted,recStack) == 0){
      free(visted);
      free(recStack);
      return 0;
    }
  }
  free(visted);
  free(recStack);
  return 1;
}

struct node* Topological(int h,struct node** gph,int n, int* visted, struct node* stack){
  struct node* head = gph[h];
  visted[h] = 0;
  for(struct node* adj = head->next; adj != 0; adj = adj->next){
    char* nxt = adj->name;
    int num = findHead(gph,n,nxt);
    if(visted[num] == 1){
      stack = Topological(num,gph,n,visted,stack);
    }
  }
  stack = push(stack, head->name);
  return stack;
}

void shortestPath(struct node** gph,int n, char* src){
  int srchead = findHead(gph,n,src);
  /*topological sorting, fill the stack*/
  int* visted = malloc(n*sizeof(int));
  arrayStart(visted,n); 
  struct node* stack = 0;
  stack = Topological(srchead,gph,n,visted,stack);
  for(int i = 0; i < n; i++){
    if(visted[i] == 1){
      stack = Topological(i,gph,n,visted,stack);
    }
  }
  /*start the distance[] with INF and dis[src] = 0 */
  int* distance = malloc(n*sizeof(int));
  for(int i = 0; i < n; i++){
    distance[i] = INT_MAX;
  }distance[srchead] = 0;

  /*process vertices*/
  while(stack != 0){
    char u[20];
    strcpy(u,pop(stack));
    stack = deleteEnd(stack);
    int order = findHead(gph,n,u);
    if(distance[order] != INT_MAX){
      struct node* unext = gph[order];
      while(unext->next != 0){
        unext = unext->next;
        int NextOrder = findHead(gph,n,unext->name);
        if(distance[NextOrder] > distance[order] + unext->edge){
          distance[NextOrder] = distance[order] + unext->edge;
        }
      }
    }
  }
  for(int i = 0; i < n; i++){
    printf("%s ",gph[i]->name);
    if(distance[i] == INT_MAX){
      printf("INF\n");
    }else{
      printf("%d\n",distance[i]);
    }
  }
  free(visted);
  free(distance);
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
  if(isCycle(gph,n) == 0){ /* True == 0 */
    printf("CYCLE\n");
    return EXIT_SUCCESS;
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

  /*address file2 - query*/
  FILE* f2 = fopen(argv[2],"r");
  char query[20];
  printf("\n");
  while(fscanf(f2, "%s\n", query) != EOF){
    shortestPath(gph,n,query);
    printf("\n");
  }

  freeGph(gph,n);
  return EXIT_SUCCESS;
}
