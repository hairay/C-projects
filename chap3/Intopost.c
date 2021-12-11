/* file name: Intopost.c*/

#include <stdio.h>
#define MAX 20
void infix_to_postfix(char [], int);  /* 由中序轉後序函數 */
int compare(char, char);  /* 比較兩個運算子函數 */

/* 在中序表示法佇列及暫存堆疊中，運算子的優先順序表，其優先值為INDEX/2 */
char infix_priority[9] = {'#', ')', '+', '-', '*', '/', '^', ' ', '('}; 
char stack_priority[8] = {'#', '(', '+', '-', '*', '/', '^', ' '};
void main(void)
{
	int rear = -1;
	char infix_q[MAX];  /* 儲存使用者輸入中序式的佇列 */
	printf("*********************************\n");
	printf("      -- Usable operator --\n");
	printf(" ^: Exponentiation\n");
	printf(" *: Multiply      /: Divide\n");
	printf(" +: Add           -: Subtraction\n");
	printf(" (: Left Brace    ): Right Brace\n");
	printf("*********************************\n");
	printf("Please enter infix expression: ");
	while(infix_q[rear] != '\n')
		infix_q[++rear] = getchar();
	infix_q[rear] = '#';  /* 於佇列結束時加入#為結束符號 */
	printf("Postfix expression: ");
	infix_to_postfix(infix_q, rear);
	printf("\n");
}

void infix_to_postfix(char infix_q[], int rear)
{
	int top = 0, ctr;
	char stack_t[MAX];  /* 用以儲存還不必輸出的運算子 */
	stack_t[top] = '#'; /* 於堆疊最底下加入#為結束符號 */
	for(ctr = 0; ctr <= rear; ctr++){
		switch(infix_q[ctr]){
			/* 輸入為)，則內輸出堆疊內運算子，直到堆疊內為( */
			case ')':
				while(stack_t[top] != '(')
					printf("%c", stack_t[top--]);
				top--;
				break;
			/* 輸入為#，則將堆疊內還未輸出的運算子輸出 */
			case '#':
				while(stack_t[top] != '#')
					printf("%c", stack_t[top--]);
				break;
			/* 輸入為運算子，若小於TOP在堆疊中所指運算子，則將堆疊
			   所指運算子輸出，若大於等於TOP在堆疊中所指運算子，則
			   將輸入之運算子放入堆疊 */
			case '(':
			case '^':
			case '*':
			case '/':
			case '+':
			case '-':
				while(compare(stack_t[top], infix_q[ctr]))
					printf("%c", stack_t[top--]);
				stack_t[++top] = infix_q[ctr];
				break;
			/* 輸入為運算元，則直接輸出 */
			default :
				printf("%c", infix_q[ctr]);
				break;
		}
	}
}

/*比較兩運算子優先權，若輸入運算子小於堆疊中運算子，則傳回值為1，否則為0 */
int compare(char stack_o, char infix_o)
{
	int index_s = 0, index_i = 0;
	while(stack_priority[index_s] != stack_o)
		index_s++;
	while(infix_priority[index_i] != infix_o)
		index_i++;
	return index_s/2 >= index_i/2 ? 1 : 0;
}
