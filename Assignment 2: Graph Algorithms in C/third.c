#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct node{
    char name[20];
    struct node* next;
};

struct node* allocate(char* v){
    struct node* temp = malloc(sizeof(struct node));
    strcpy(temp->name,v);
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

void insert(struct node* head,char* vertex){
  if(head == 0){
    printf("insert failed\n");
    return;
  }
  struct node* temp = allocate(vertex);
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

struct node* enqueue(struct node* head, char* val){
  struct node* temp = allocate(val);
  
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

struct node* deleteFront(struct node* head){
  if(head == 0){
    return head;
  }
  struct node* temp = head->next;
  free(head);
  return temp;
}

char* dequeue(struct node* head){
  if(head == 0){
    printf("queue empty");
    return 0;
  }
  return head->name;
}

/*creat a empty boolean array with all F == 1 */
void arrayStart(int* arr, int n){
  for(int i = 0; i < n; i++){
    arr[i] = 1;
  }
}

void BFS(int h,struct node** gph,int n, int* visted, struct node* queue){
  struct node* head = gph[h];
  visted[h] = 0;
  printf("%s",head->name);
  queue = enqueue(queue,head->name);

  while(queue != 0){
    char* v = dequeue(queue);
    int rank = findHead(gph,n,v);
    queue = deleteFront(queue);
    for(struct node* adj = gph[rank]->next; adj != 0; adj = adj->next){
      char* nxt = adj->name;
      int num = findHead(gph,n,nxt);
      if(visted[num] == 1){
        printf(" %s",nxt);
        visted[num] = 0;
        queue = enqueue(queue,nxt);
      }
    }
  }
}

void printBFS(int h,struct node** gph,int n){
  int* visted = malloc(n*sizeof(int));
  arrayStart(visted,n);
  struct node* queue = 0;
  BFS(h,gph,n,visted,queue);  
  for(int i = 0; i < n; i++){
    if(visted[i] == 1){
      BFS(i,gph,n,visted,queue);
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
        gph[i] = allocate(vertex);
    }

    /*add edges*/
    char v1[20];
    char v2[20];
    while(fscanf(f1, "%s %s\n", v1, v2) != EOF){
        struct node* h1 = gph[findHead(gph,n,v1)];
        struct node* h2 = gph[findHead(gph,n,v2)];
        insert(h1,v2);
        insert(h2,v1);
    }
    fclose(f1);

    /*address file2 - query*/
  FILE* f2 = fopen(argv[2],"r");
  char query[20];
  while(fscanf(f2, "%s\n", query)!=EOF){
    int head = findHead(gph,n,query);
    printBFS(head,gph,n);
  }
  fclose(f2);

  freeGph(gph,n);
  return EXIT_SUCCESS;
}
