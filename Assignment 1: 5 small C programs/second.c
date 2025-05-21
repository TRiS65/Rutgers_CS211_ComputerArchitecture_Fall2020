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

struct node* insert(struct node*,int);
struct node* insert(struct node* head, int val){
  struct node* temp = allocate(val);
  
  /*empty*/
  if(head == 0 ){
    return temp;
  }
  if(head->value == val){
    free(temp);
    return head;
  }
  if(head->value > val){
    temp->next = head;
    return temp;
  }
  /*pointers*/
  struct node* prev = head;
  struct node* curr = prev->next;
  
  while(curr != 0 && curr->value < val){
    prev = curr;
    curr = curr->next;
  }
  /*add in the end*/
  if(curr == 0){
    prev->next = temp; 
  }
  /*without dupicate(curr.value = val)*/
  else if(curr->value == val){
    free(temp);
    return head;
  }
  /*add before curr(curr.value > val)*/
  else{
    prev->next = temp;
    temp->next = curr;
  }
  
  return head;
  
}

struct node* delete(struct node*,int);
struct node* delete(struct node* head, int val){
  if(head == 0){
    return head;
  }
  if(head->value == val){
    struct node* temp = head->next;
    free(head);
    return temp;
  }

  struct node* prev = head;
  struct node* curr = prev->next;
  
  while(curr != 0 && curr->value < val){
    prev = curr;
    curr = curr->next;
  }
  if(curr->value == val){
    prev->next = curr->next;
    free(curr);
  }
  
  return head;  
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
  while(fscanf(f,"%s %d\n",oper,&val)!=EOF){
    if(oper[0]=='I'){
      head = insert(head,val);
    }else{
      head = delete(head,val);
    }
    printNode(head);
  }
  fclose(f);

  if(head != 0){
    freeMalloc(head);
  }
  return EXIT_SUCCESS;
}
