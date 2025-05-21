#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>

//structures and creating
struct Variable{
    char name[20];
    int ID;
    int value;
    struct Variable* next;
};
struct Variable* CreateVar(struct Variable* head, char* name, int val){
    struct Variable* temp = malloc(sizeof(struct Variable));
    strcpy(temp->name,name);
    temp->value = val;
    temp->next = NULL;
    if(head == NULL){
        temp->ID = 0;
        head = temp;
        return head;
    }
    struct Variable* ptr = head;
    while (ptr->next != NULL){
        ptr = ptr->next;
    }temp->ID = ptr->ID +1;
    ptr->next = temp;
    return head;
}
struct Gate{
    char name[20];
    int numIN; //number of inputs
    char IN1[20];
    char IN2[20];
    char OUT[20];
    char Multi[100]; //for decoder and multiplexers
    struct Gate* next;
};
struct Gate* CreateGate(struct Gate* head, char* name, int num, char* input1, char* input2, char* output, char* multi){
    struct Gate* temp = malloc(sizeof(struct Gate));
    strcpy(temp->name, name);
    temp->numIN = num;
    strcpy(temp->IN1,input1);
    strcpy(temp->IN2,input2);
    strcpy(temp->OUT,output);
    strcpy(temp->Multi,multi);
    temp->next = NULL;
    if(head == NULL){
        head = temp;
        return head;
    }
    struct Gate* ptr = head;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    ptr->next = temp;
    return head;
}

void freeVar(struct Variable* head){
  struct Variable* curr = head;
  while(head->next != NULL){
    head = head->next;
    free(curr);
    curr = head;
  }
  free(head);
}
void freeGate(struct Gate* head){
  struct Gate* curr = head;
  while(head->next != NULL){
    head = head->next;
    free(curr);
    curr = head;
  }
  free(head);
}
void freeMatrix(int** matrix, int n){
    for(int i = 0; i < n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

//finding input value and fill output
int findIN(struct Variable* head, char* name){
    if(name[0] == '0'){
        return 0;
    }else if(name[0] == '1'){
        return 1;
    }
    struct Variable* ptr = head;
//    printf("\n%s: ",name);
    while(ptr != NULL){
//        printf("(%s)->",ptr->name);
        if(strcmp(ptr->name,name) == 0){
            return ptr->value;
        }
        ptr = ptr->next;
    }
    printf(":input not found\n");
    return -1;
}
void fillOUT(struct Variable* IN, struct Variable* OUT, char* name, int value){
    struct Variable* ptr = OUT;
    while(ptr != NULL){
        if(strcmp(ptr->name,name) == 0){
            ptr->value = value;
            return;
        }
        ptr = ptr->next;
    }//not find in output linklist
    ptr = IN;
    while(ptr != NULL){
        if(strcmp(ptr->name,name) == 0){
            ptr->value = value;
            return;
        }
        ptr = ptr->next;
    }//not find in IN linklist (as temp1)
    IN = CreateVar(IN,name,value);
}

void FillGrey(int grey, struct Variable* head, int n){ //n:# of inputs
    struct Variable* ptr = head;
    for(int i = n-1; i >= 0; i--){
        ptr->value = (grey >> i) % 2;
        ptr = ptr->next;
    }
}

//gate funtions
int NotGate(int IN){
    if(IN == 0){
        return 1;
    }else{
        return 0;
    }
    
}
int AndGate(int IN1, int IN2){
    return IN1&IN2;
}
int OrGate(int IN1, int IN2){
    return IN1|IN2;
}
int XorGate(int IN1, int IN2){
    return IN1^IN2;
}
void Decoder(struct Variable* IN, struct Variable* OUT, char* multi, int n){ //n:#of input
    int* input = malloc(n * sizeof(int));
    char copy[100];
    strcpy(copy,multi);
    char* name = strtok(copy," ");
    for(int i = 0; i < n; i++){
        input[i] = findIN(IN,name);
        name = strtok(NULL," ");
    }
    int result = 0;
    for(int i = 0; i < n; i++){
        if(input[i] == 1){
            result += pow(2,n-i-1);
        }
    }
    int m = pow(2,n);

    //start a grey code
    int* grey = malloc(m * sizeof(int));
    grey[0]=0; grey[1]=1; grey[2]=3; grey[3]=2;
    for(int i = 2; i < n; i++){
        int num = pow(2,i);
        int place = num-1;
        while(place >= 0){
            grey[num] = grey[place] + pow(2,i);
            num ++;
            place --;
        }
    }

    for(int i = 0; i < m; i++){
        if(grey[i] == result){
            fillOUT(IN,OUT,name,1);
        }else{
            fillOUT(IN,OUT,name,0);
        }
        name = strtok(NULL," ");
    }
    
    free(grey);
    free(input);
}
void Multiplexer(struct Variable* IN, struct Variable* OUT, char* multi, int n){ //n:#of selecters
    int m = pow(2,n);//# of inputs
    int* input = malloc(m * sizeof(int));
    char copy[100];
    strcpy(copy,multi);
    char* name = strtok(copy," ");
    for(int i = 0; i < m; i++){
//        printf("%s ",name);
        input[i] = findIN(IN,name);
        name = strtok(NULL," ");
    }
    int* selecter = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        selecter[i] = findIN(IN,name);
        name = strtok(NULL," ");
    }
    //decode grey
    for(int i = 1; i < n; i++){
        selecter[i] = XorGate(selecter[i-1],selecter[i]);
    }
    int result = 0;
    for(int i = 0; i < n; i++){
        if(selecter[i] == 1){
            result += pow(2,n-i-1);
        }
    }

    fillOUT(IN,OUT,name,input[result]);
    free(input);
    free(selecter);
}

//malipulate gates
void ReadGates(struct Gate* G, struct Variable* IN, struct Variable* OUT, int** test){
    int row = 0;
    struct Gate* ptr = G;
    int input1;
    int input2;
//    printf("gate is: ");
    while(ptr != NULL){
        if(ptr->name[0] == 'N'){
            input1 = findIN(IN,ptr->IN1);
            fillOUT(IN,OUT,ptr->OUT,NotGate(input1));
        }else if(ptr->name[0] == 'A'){
            input1 = findIN(IN,ptr->IN1);
            input2 = findIN(IN,ptr->IN2);
            fillOUT(IN,OUT,ptr->OUT,AndGate(input1,input2));
        }else if(ptr->name[0] == 'O'){
            input1 = findIN(IN,ptr->IN1);
            input2 = findIN(IN,ptr->IN2);
            fillOUT(IN,OUT,ptr->OUT,OrGate(input1,input2));
        }else if(ptr->name[0] == 'X'){
            input1 = findIN(IN,ptr->IN1);
            input2 = findIN(IN,ptr->IN2);
            fillOUT(IN,OUT,ptr->OUT,XorGate(input1,input2));
        }else if(ptr->name[0] == 'D'){
            Decoder(IN,OUT,ptr->Multi,ptr->numIN);
        }else if(ptr->name[0] == 'M'){
            Multiplexer(IN,OUT,ptr->Multi,ptr->numIN);
        }else{ //unknown gates
            if(test[row][0] == 1){ //OR
//                printf("(or)");
                input1 = findIN(IN,ptr->IN1);
                input2 = findIN(IN,ptr->IN2);
                fillOUT(IN,OUT,ptr->OUT,OrGate(input1,input2));
            }else if(test[row][1] == 1){ //AND
//                printf("(and)");
                input1 = findIN(IN,ptr->IN1);
                input2 = findIN(IN,ptr->IN2);
                fillOUT(IN,OUT,ptr->OUT,AndGate(input1,input2));
            }else if(test[row][2] == 1){ //XOR
//                printf("(xor)");
                input1 = findIN(IN,ptr->IN1);
                input2 = findIN(IN,ptr->IN2);
                fillOUT(IN,OUT,ptr->OUT,XorGate(input1,input2));
            }
            row++;
        }
//        printf("%s -> ",ptr->name);
        ptr = ptr->next;
    }
//    printf(" next\n");
}
int ValidGuess(struct Gate* G, struct Variable* IN, struct Variable* OUT, int** test, int* grey, int** OutputVal, int NumOfOutput, int NumOfInput){
    int times = pow(2,NumOfInput);
    struct Variable* ptr = NULL;
    for(int i = 0; i < times; i++){
        FillGrey(grey[i],IN,NumOfInput);//grey[i]
//        printf("\nfor i = %d, times = %d, ",i, times);
        ReadGates(G,IN,OUT,test);
        ptr = OUT;
        for(int j = 0; j < NumOfOutput; j++){
//            printf("(%dvs%d)",ptr->value, OutputVal[j][i]);
            if(ptr->value != OutputVal[j][i]){
                return 0;
            }
            ptr = ptr->next;
        }
    }
    return 1;
}
int ValidGates(struct Gate* G, struct Variable* IN, struct Variable* OUT, int** test, int* grey, int** OutputVal, int NumOfOutput, int NumOfInput, int unknown, int row, int col){
//    printf("row = %d, col = %d\n",row,col);
    int result = ValidGuess(G,IN,OUT,test,grey,OutputVal,NumOfOutput,NumOfInput);
//    printf(" running\n");
    if(row == unknown){
        
        return result;
    }

    //check below [r][0]
    result = ValidGates(G,IN,OUT,test,grey,OutputVal,NumOfOutput,NumOfInput,unknown,row+1,col);
    if(result == 1){
        return 1;
    }
    test[row][0] = 0;
    col = (col+1)%3;
    //check below [r][1]
    result = ValidGates(G,IN,OUT,test,grey,OutputVal,NumOfOutput,NumOfInput,unknown,row+1,col);
    if(result == 1){
        return 1;
    }
    test[row][1] = 0;
    col = (col+1)%3;
    //check below [r][2]
    result = ValidGates(G,IN,OUT,test,grey,OutputVal,NumOfOutput,NumOfInput,unknown,row+1,col);
    if(result == 1){
        return 1;
    }
    //not found in whole row
//    if(col == 2){
        col = 0;
        test[row][0] = 1;
        test[row][1] = 1;
//    }
    return 0;
}

//other functions
int isDecoder(int num){
    for(int i = 1; i < num/2;i++){
        int result = i + pow(2,i);
        if(num == result){
            return i;
        }else if(num < result){
            return 0;
        }
    }
    return 0;
}
int isMultiplexer(int num){
    for(int i = 1; i < num/2;i++){
        int result = i + pow(2,i) + 1;
        if(num == result){
            return i;
        }else if(num < result){
            return 0;
        }
    }
    return 0;
}

void fillResult(int unknown, struct Gate* result, int** test){
    struct Gate* ptr = result;
    int i = 0;
    while(i < unknown){
        if(ptr->name[0] == 'G'){
            if(test[i][0] == 1){
                strcpy(ptr->name,"OR");
            }else if(test[i][1] == 1){
                strcpy(ptr->name,"AND");
            }else{
                strcpy(ptr->name,"XOR");
            }
            i++;
        }
        ptr = ptr->next;
    }
}

void printVariable(struct Variable* head){
    if(head == NULL){printf("empty\n");return;}
    struct Variable* curr = head;
    while(curr != NULL){
        printf("%s ",curr->name);
        curr = curr->next;
    }
    printf("\n");
}
void printGate(struct Gate* head, int total){
    if(head == NULL){printf("empty\n");return;}
    struct Gate* ptr = head;
    for(int i = 1; i <= total; i++){
        printf("G%d %s\n",i,ptr->name);
        ptr = ptr->next;
    }
}

int main(int argc, char* argv[argc+1]){
    FILE* f = fopen(argv[1],"r");
    if(f == NULL){printf("failed open");return 0;}
    int NumOfInput,NumOfOutput,Combin,num,place;
    struct Variable* IN = NULL;
    struct Variable* OUT = NULL;
    int** OutputVal;
    char name[20];
    char oper[20];
    //fill IN,OUT, OutputVal matrix
    for(int i = 0; i < 3; i++){
        fscanf(f,"%s",oper);
        if(oper[0] == 'I'){
            fscanf(f,"%d",&NumOfInput);
            for(int j = 0; j < NumOfInput; j++){
                fscanf(f,"%s",name);
                IN = CreateVar(IN,name,0);
            }
            fscanf(f,"\n");
        }else if(oper[0] == 'O' && oper[8] == 'R'){
            fscanf(f,"%d",&NumOfOutput);
            OutputVal = malloc(NumOfOutput * sizeof(int*));
            for(int j = 0; j < NumOfOutput; j++){
                fscanf(f,"%s",name);
                OUT = CreateVar(OUT,name,0);
            }
            fscanf(f,"\n");
        }else{
            char output[1000];
            for(int j = 0; j < NumOfOutput; j++){
                fscanf(f,"%s",name);
                fscanf(f," %[^\n]\n",output);
                Combin = (strlen(output)+1)/2;
                OutputVal[j] = malloc(Combin * sizeof(int));
                num = 0;
                place = 0;
                while(output[num] != '\0'){
                    if(output[num] != ' '){
                        OutputVal[j][place] = output[num] - '0';
                        place++;
                    }
                    num++;
                }
            }
        }
    } 

    //fill the Gate
    struct Gate* G = NULL;
    struct Gate* result = NULL; //unknown gates
    //for 1 or 2 inputs
    char GateInput1[20];
    char GateInput2[20];
    char GateOutput[20];
    char multi[100];
    int total = 0; // total # of result gates
    int unknown = 0; // # of OR AND XOR
    while(fscanf(f,"%s",oper) != EOF){
        if(oper[0] == 'N'){ //NOT gate
            fscanf(f,"%s %s\n",GateInput1, GateOutput);
            G = CreateGate(G,oper,1,GateInput1,GateInput2,GateOutput,multi);
        }else if(oper[0] == 'A' || oper[0] == 'O' || oper[0] == 'X'){ //AND OR XOR gates
            fscanf(f,"%s %s %s\n",GateInput1, GateInput2,GateOutput);
            G = CreateGate(G,oper,2,GateInput1,GateInput2,GateOutput,multi);
        }else if(oper[0] == 'D'){ //DECODER gate
            fscanf(f,"%d %[^\n]\n",&num,multi);
            G = CreateGate(G,oper,num,GateInput1,GateInput2,GateOutput,multi);
        }else if(oper[0] == 'M'){ //MULTIPLEXER gate
            fscanf(f,"%d %[^\n]\n",&num,multi);
            num = log2(num);
            G = CreateGate(G,oper,num,GateInput1,GateInput2,GateOutput,multi);

        }else if(oper[0] == 'G'){ // need to define
            total ++;
            fscanf(f,"%d",&num);
            if(num == 2){//not
                fscanf(f,"%s %s\n",GateInput1, GateOutput);
                G = CreateGate(G,"NOT",1,GateInput1,GateInput2,GateOutput,multi);
                result = CreateGate(result,"NOT",1,GateInput1,GateInput2,GateOutput,multi);
            }else if(num == 3){//and or xor
                unknown ++;
                fscanf(f,"%s %s %s\n",GateInput1, GateInput2,GateOutput);
                G = CreateGate(G,oper,2,GateInput1,GateInput2,GateOutput,multi);
                result = CreateGate(result,oper,2,GateInput1,GateInput2,GateOutput,multi);
            }else{
                if(isDecoder(num) != 0){//decoder
                    num = isDecoder(num);
                    fscanf(f," %[^\n]\n",multi);
                    G = CreateGate(G,"DECODER",num,GateInput1,GateInput2,GateOutput,multi);
                    result = CreateGate(result,"DECODER",num,GateInput1,GateInput2,GateOutput,multi);
                }else{
                    num = isMultiplexer(num);
                    fscanf(f," %[^\n]\n",multi);
                    G = CreateGate(G,"MULTIPLEXER",num,GateInput1,GateInput2,GateOutput,multi);
                    result = CreateGate(result,"MULTIPLEXER",num,GateInput1,GateInput2,GateOutput,multi);
                }
            }
        }
    }
    //OR, AND, XOR
    int** test;
    if(unknown != 0){
        test = malloc(unknown * sizeof(int*));
        for(int i = 0; i < unknown; i++){
            test[i] = malloc(3 * sizeof(int));
            test[i][0] = 1; //OR
            test[i][1] = 1; //AND
            test[i][2] = 1; //XOR
        }
    }
    
//    printGate(result,total);

    //grey code (not binary)
    Combin = pow(2,NumOfInput);
    int* grey = malloc(Combin * sizeof(int));
    grey[0]=0; grey[1]=1; grey[2]=3; grey[3]=2;
    for(int i = 2; i < NumOfInput; i++){
        num = pow(2,i);
        place = num-1;
        while(place >= 0){
            grey[num] = grey[place] + pow(2,i);
            num ++;
            place --;
        }
    }

    //fill test

    int answer = ValidGates(G,IN,OUT,test,grey,OutputVal,NumOfOutput,NumOfInput,unknown,0,0);
//    int answer = ValidGuess(G,IN,OUT,test,grey,OutputVal,NumOfOutput,NumOfInput);

    if(answer == 0){
        printf("INVALID\n");
    }else{
        fillResult(unknown,result,test);
        printGate(result,total);
    }


    fclose(f);
    freeVar(IN);
    freeVar(OUT);
    freeGate(G);
    freeGate(result);
    freeMatrix(OutputVal,NumOfOutput);
    if(unknown != 0){freeMatrix(test,unknown);}
    free(grey);
    return EXIT_SUCCESS;
}



