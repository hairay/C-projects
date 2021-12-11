/* file name: Intopost.c*/

#include <stdio.h>
#define MAX 20
void infix_to_postfix(char [], int);  /* �Ѥ������Ǩ�� */
int compare(char, char);  /* �����ӹB��l��� */

/* �b���Ǫ�ܪk��C�μȦs���|���A�B��l���u�����Ǫ�A���u���Ȭ�INDEX/2 */
char infix_priority[9] = {'#', ')', '+', '-', '*', '/', '^', ' ', '('}; 
char stack_priority[8] = {'#', '(', '+', '-', '*', '/', '^', ' '};
void main(void)
{
	int rear = -1;
	char infix_q[MAX];  /* �x�s�ϥΪ̿�J���Ǧ�����C */
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
	infix_q[rear] = '#';  /* ���C�����ɥ[�J#�������Ÿ� */
	printf("Postfix expression: ");
	infix_to_postfix(infix_q, rear);
	printf("\n");
}

void infix_to_postfix(char infix_q[], int rear)
{
	int top = 0, ctr;
	char stack_t[MAX];  /* �ΥH�x�s�٤�����X���B��l */
	stack_t[top] = '#'; /* ����|�̩��U�[�J#�������Ÿ� */
	for(ctr = 0; ctr <= rear; ctr++){
		switch(infix_q[ctr]){
			/* ��J��)�A�h����X���|���B��l�A������|����( */
			case ')':
				while(stack_t[top] != '(')
					printf("%c", stack_t[top--]);
				top--;
				break;
			/* ��J��#�A�h�N���|���٥���X���B��l��X */
			case '#':
				while(stack_t[top] != '#')
					printf("%c", stack_t[top--]);
				break;
			/* ��J���B��l�A�Y�p��TOP�b���|���ҫ��B��l�A�h�N���|
			   �ҫ��B��l��X�A�Y�j�󵥩�TOP�b���|���ҫ��B��l�A�h
			   �N��J���B��l��J���| */
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
			/* ��J���B�⤸�A�h������X */
			default :
				printf("%c", infix_q[ctr]);
				break;
		}
	}
}

/*�����B��l�u���v�A�Y��J�B��l�p����|���B��l�A�h�Ǧ^�Ȭ�1�A�_�h��0 */
int compare(char stack_o, char infix_o)
{
	int index_s = 0, index_i = 0;
	while(stack_priority[index_s] != stack_o)
		index_s++;
	while(infix_priority[index_i] != infix_o)
		index_i++;
	return index_s/2 >= index_i/2 ? 1 : 0;
}
