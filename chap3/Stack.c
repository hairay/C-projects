/* file name : Stack.c */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define MAX 100
void push_f(void);   /* �s�W��� */
void pop_f(void);    /* �R����� */
void list_f(void);   /* ��X��� */
char item[MAX][20];
int top = -1;          /* top���_�l�Ȭ�-1 */

void main(void)
{
	char option;
	while(1){
		printf("\n *****************************\n");
		printf("        <1> insert (push)\n");
		printf("        <2> delete (pop)\n");
		printf("        <3> list\n");
		printf("        <4> quit\n");
		printf(" *****************************\n");
		printf(" Please enter your choice...");
		option = getche();
		switch(option)	{
			case '1':
				push_f();
				break;
			case '2':
				pop_f();
				break;
			case '3':
				list_f();
				break;
			case '4':
				exit(0);
		}
	}
}

void push_f(void)
{
	if(top >= MAX-1)   /* ����|�w���A�h��ܿ��~ */
		printf("\n\nStack is full !\n");
	else{
		top++;
		printf("\n\n Please enter item to insert: ");
		gets(item[top]);
	}
}

void pop_f(void)
{
	if(top < 0)  /* ����|�S����Ʀs�b�A�h��ܿ��~ */
		printf("\n\n No item, stack is empty !\n");
	else{
		printf("\n\n Item %s deleted\n", item[top]);
		top--;
	}
}

void list_f(void)
{
	int count = 0, i;
	if(top < 0)
		printf("\n\n No item, stack is empty\n");
	else{
		printf("\n\n  ITEM\n");
		printf(" ------------------\n");
		for(i = 0; i <= top; i++){
			printf("  %-20s\n", item[i]);
			count++;
			if(count % 20 == 0) getch();
		}
		printf(" ------------------\n");
		printf("  Total item: %d\n", count);
		getch();
	}
}
