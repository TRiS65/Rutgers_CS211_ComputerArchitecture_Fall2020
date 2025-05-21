#include<stdlib.h>
#include<stdio.h>

struct node{
  int value;
  struct node* next;
};

struct node* allocate(int);
struct node* allocate(int val){
  struct node* temp = malloc(sizeof(struct node));
  temp->value = val;
  temp->next = 0;
  return temp;
}

struct node* enqueue(struct node*,int);
struct node* enqueue(struct node* head, int val){
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

struct node* push(struct node*,int);
struct node* push(struct node* head, int val){
  struct node* temp = allocate(val);
  
  /*empty*/
  if(head == 0 ){
    return temp;
  }
  /* add at the front*/
  temp->next = head;
  
  return temp;
  
}

struct node* pop(struct node*);
struct node* pop(struct node* head){
  if(head == 0){
    return head;
  }

  struct node* temp = head->next;
  free(head);
  return temp;
  
}

void printNode(struct node* head){  
  if(head == 0){
    printf("EMPTY\n");
    return;
  }
  struct node* curr = head;
  printf("%d",curr->value);
  curr = curr->next;
  while(curr != 0){
    printf(" %d",curr->value);
    curr = curr->next;
  }
  printf("\n");
}

void freeMalloc(struct node* head){
  struct node* curr = head;
  while(head->next != 0){
    head = head->next;
    free(curr);
    curr = head;
  }
  free(head);
}

int main(int argc, char* argv[argc+1]){
    if(argc < 2){
    printf("error");
    return EXIT_SUCCESS;
  }
  
  struct node* head = 0;
  
  FILE* f=fopen(argv[1],"r");
  int val;
  char oper[10];
  while(fscanf(f,"%s",oper)!=EOF){
    if(oper[1]=='O'){
      fscanf(f,"\n");
      head = pop(head);
    }else{
      fscanf(f," %d\n",&val);
      if(oper[0]=='P'){
	head = push(head,val);
      }else{
	head = enqueue(head,val);
      }
    }
    printNode(head);
  }
  fclose(f);

  if(head != 0){
    freeMalloc(head);
  }
  return EXIT_SUCCESS;
}
