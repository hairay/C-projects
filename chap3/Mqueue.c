/* file name: Mqueue.c */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define MAX   20
#define UNIT   4
void enqueue_f(int number1);   /* 新增函數 */
void dequeue_f(int number2);   /* 刪除函數 */
void list_f(int number3);      /* 輸出函數 */

char MQ[MAX][20];
int f[UNIT], r[UNIT];

void main(void)
{
	char option;
	int  number1, number2, number3;
	int  j;
 
	for (j=0; j<=(UNIT-1); j++){
	  r[j] = j*(MAX/UNIT)-1;
	  f[j] = j*(MAX/UNIT);
	}
    
	printf(" The total space of Queue are %d !!\n", MAX);
	printf(" The total number of Queue are %d !!\n", UNIT);
    printf(" There is %d Queue !!", MAX/UNIT);   
 
	while(1){
		printf("\n *****************************\n");
		printf("        <1> insert (enquue)\n");
		printf("        <2> delete (dequeue)\n");
		printf("        <3> list\n");
		printf("        <4> quit\n");
		printf(" *****************************\n");
		printf(" Please enter your choice...");
		option = getche();
		switch(option){
			case '1':
				printf("\n Please enter the number of queue you want to insert ?");
				scanf("%d",&number1);
				if (number1 > UNIT){
				  printf(" Sorry, the number is out of Queue index !!");
				  break;
				}
				else
				  enqueue_f(number1);
				break;
			case '2':
				printf("\n Please enter the number of queue you want to delete ?");
				scanf("%d",&number2);
				if (number2 > UNIT){
				  printf(" Sorry, the number is out of Queue index !!");
				  break;
				}
				else
				  dequeue_f(number2);
				break;
			case '3':
				printf("\n Please enter the number of queue you want to print ?");
				scanf("%d",&number3);
				if (number3 > UNIT){
				  printf(" Sorry, the number is out of Queue index !!");
				  break;
				}
				else
				  list_f(number3);
				break;
			case '4':
				printf("\n ");
				exit(0);
		}
	}
}

void enqueue_f(int number1)
{
	if(r[number1-1] == f[number1]-1) {  /* 當堆疊已滿，則顯示錯誤 */
		printf(" Queue%d is full !\n",number1);
		printf(" Please choose another Queue to insert !!");
	}
	else {
		printf(" Please enter item to insert: ");
		scanf("%s",&MQ[++r[number1-1]]);
		printf(" You insert %s to #%d queue", MQ[r[number1-1]], number1 );
	}
}

void dequeue_f(int number2)
{
	if(r[number2-1] == f[number2-1])  /* 當堆疊沒有資料存在，則顯示錯誤 */
		printf(" No item, queue%d is empty !",number2);
	else{
		printf(" Item %s in Queue %d is deleted", MQ[f[number2-1]], number2);
		f[number2-1]++;
	}
}

void list_f(int number3)
{
	int count = 0, i, start_number = 0;
	if ((r[number3-1]) == f[number3-1]-1)
		printf(" No item, queue is empty!");
	else{
		start_number = (number3-1)*(MAX/UNIT);
		printf("\n  ITEM\n");
		printf(" ------------------\n");
		for(i = f[number3-1] ; i <= r[number3-1]; i++){
			printf("  %-20s\n", MQ[i]);
			count++;
			if(count % 20 == 0) getch();
		}
		printf(" ------------------\n");
		printf(" Total item: %d\n", count);
		getch();
	}
}
