#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Estrutura para representar um nó da árvore
typedef struct Node {
    char opr;
    double num;
    struct Node *left;
    struct Node *right;
} Node;

// Função para criar um novo nó
Node* createNode(char opr, double num) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->opr = opr;
    node->num = num;
    node->left = NULL;
    node->right = NULL;
    return node;
}
void print(char a[], int min, int max){
   printf("\n");
   for(int c=min; c<=max; c++){
       printf("%c", a[c]);
   }
   printf("\n");
}

int charInt(char a[], int min, int max){
  int num = 0, i = min;
  while (i<=max) {
            num = num * 10 + (a[i] - '0');
            i++; 
  }
  return num;
}

int partses(char a[], int i){
	int pilha=1;
	while(pilha>0){
		i++;
		if(a[i]=='('){pilha++;
		}else if(a[i]==')'){pilha--;}
	}
	return i;
}

Node* buildTree(char expression[], int min, int max) {
   Node *root = NULL;

   int ver = 0, verp=0, c=0, temp=0;
   //print(expression, min, max);
   while (((expression[min] == '(') && (expression[max] == ')') && (c == 0))) {
    verp++;
    c = min;
    while (verp > 0) {
        c++;
        if (expression[c] == ')') {
            verp--;
        } else if (expression[c] == '(') {
            verp++;
        }
    }
    
    if ((verp == 0) && (c == max)) {
        min++;
        max--;
    } else {
        c = 1;
    }
    //print(expression, min, max);
   }
    
    for(c=min; (c<=max) && (ver!=1); c++){
       if(expression[c]=='('){
           verp++;
           while(verp>0){
               c++;
               if(expression[c]==')'){ 
                   verp--;
               }else if(expression[c]=='('){
                   verp++;
               }               
           }
       } else if((expression[c]=='+')||(expression[c]=='-')) {
		if(c==min){ver=100;temp=c+1;
                }else{root = createNode(expression[c], 0);temp = c;ver = 1;}          
       } else if((((expression[c]=='*')&&(expression[c+1]!='*'))||(expression[c]=='/'))){
           temp = c;
           ver = 0;
       } else if((expression[c]=='*')&&(expression[c+1]=='*')){
	   if((temp==0)||(ver>=2)){temp = c; ver = 2;}
	   c++;
       }else if((expression[c]=='s')&&(expression[c+2]=='r')){
           c=c+3;  
           if((temp==0)||(ver>=3)){temp = c;ver = 3;}
       }

    }
    if(temp>0){	
        int i = 1;
        if(ver==0){
		root = createNode(expression[temp], 0); 
		c=temp;
	}else if(ver==2){
		root = createNode('^', 0); i=2;
        }else if(ver==3){
		root = createNode('r', 0); verp=0;
		for(c=temp;c<=max; c++){
			if(expression[c]=='('){
				if(verp<1){min=c+1;verp++;
				}else{c=partses(expression, c);}
			}else if(expression[c]==','){temp=c;
			}else if(expression[c]==')'){max=c-1;verp--;}
                }
        }else if(ver==100){
		temp--;
		printf("\ntemp %d[%c]\n",temp, expression[temp]);
		root = createNode(expression[temp], 0); 
		expression[temp]='0';temp++; i=0;
        }
	printf("\ntemp left %d[%c]\n",temp-1, expression[temp-1]);root->left = buildTree(expression, min, temp-1);
    	printf("\ntemp right %d[%c]\n",temp+i, expression[temp+i]);root->right = buildTree(expression, temp+i, max);
	
    }else{
		print(expression, min, max);
                double a = charInt(expression, min, max);
		root = createNode('\0', a);
    }

    return root;
}


double evaluateTree(Node *root) {
    if (root == NULL) {
        return 0;
    }

    if (root->left == NULL && root->right == NULL) {
        return root->num;
    }

    double leftValue = evaluateTree(root->left);
    double rightValue = evaluateTree(root->right);
    printf("\n [%.5lf %c %.5lf]\n", leftValue, root->opr, rightValue);
    switch (root->opr) {
        case '+': return leftValue + rightValue;
        case '-': return leftValue - rightValue;
        case '*': return leftValue * rightValue;
        case '/': return leftValue / rightValue;
        case '^': return pow(leftValue,  rightValue);
        case 'r': return pow(leftValue,  (1/rightValue));
        default: return 0;
    }
}

int main() {
    char expression[100];
    int x=0;
    do{  
	 fflush(stdin);
   	 printf("Digite a expressao: ");
   	 fgets(expression, sizeof(expression), stdin);
   	 size_t length = strlen(expression);
   	 if (expression[length - 1] == '\n') {
   	     expression[length - 1] = '\0';
  	 }
   	 int index = 0;
   	 Node *root = buildTree(expression, 0, strlen(expression)-1);
	
   	 double result = evaluateTree(root);
	
    	 printf("Resultado: %.5lf\n", result);
         printf("deseja continuar?\n0 - n\noutros - s\n");
         scanf("%d", &x);
    }while(x!=0);
    return 0;
}
