/* file name: Tbintree.c */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

/*	�w�q��Ƶ��c
rbit = 1 , rchild�����`����, rbit =0 , rchild���޽u
lbit = 1 , lchild�����`����, lbit =0 , lchild���޽u */

struct tbintree {
	int number;
	int rbit; 	
	int lbit;
	struct tbintree *lchild;
	struct tbintree *rchild;
} *root, *ptr, *newnode;

struct tbintree *newtbintree();   /*�t�m�O�Ш��*/

void insert();
void delete_node();
void inorder_show( struct tbintree *);
void insert_right( struct tbintree *, struct tbintree *);
void insert_left(struct tbintree *, struct tbintree * );

struct tbintree * insucc( struct tbintree * );
struct tbintree * inpred( struct tbintree * );

void main()
{
	char *menu_prompt =
		"=== Thread Binary Search Program ==\n"
		"  1. Insert\n"
		"  2. Delete\n"
		"  3. Show\n"
		"  4. Quit\n"
		"Please input a number : ";
	char menusele;
	/* ��l�� */
	/* �޽u�G����ڸ`�I������: root->number����data */
	/* �k�쵲�û����V���� : rbit =1 */
	root = newtbintree();
	root->lchild = root;
	root->rchild = root;
	root->lbit = 0;
	root->rbit = 1;
	do{
		printf("%s", menu_prompt);
		menusele = toupper(getche());
		puts("");
		switch ( menusele )	{
			case '1' :
				insert();
				break;
			case '2' :
				delete_node();
				break;
			case '3' :
				inorder_show( root );
				break;
			case '4' :
				puts("Bye Bye ^_^");
				exit(1);
			default :
				puts("Invalid choice !!");
		}
	} while ( 1 );
}

/*���J�`�I���*/
void insert()
{
	newnode = newtbintree();
	printf("Enter a number to insert : ");
	scanf("%d", &newnode->number);
	if ( root->lchild == root ){
		insert_left( root, newnode );
		puts("node insert ok!");
	}
	else{
		ptr = root->lchild;
		while ( ptr->number != newnode->number ){
			/*  �p�s�`�I�p��ثe�`�I�Blbit��0 (lchild���޽u)
				�h���J�ثe�`�I����, �_�hptr�����j�M  */
			if ( newnode->number < ptr->number ){
				if ( ptr->lbit == 0 ){
					insert_left( ptr, newnode);
					break;
				}
				else
					ptr = ptr->lchild;
			}
			/*  �p�s�`�I�j��ثe�`�I�Brbit��0 (rchild���޽u)
				�h���J�ثe�`�I�k��, �_�hptr���k�j�M  */
			else if ( newnode->number > ptr->number){
				if ( ptr->rbit == 0 ){
					insert_right( ptr , newnode);
					break;
				}
				else
					ptr = ptr->rchild;
			}
		}
		if ( ptr->number == newnode->number )
		{
			puts("Number existed ...!");
			return;
		}
		else
			puts("node insert ok!");
	}
}

/*�R���`�I���*/
void delete_node()
{
	struct tbintree *ptr_parent;
	struct tbintree *ptr_pred, *ptr_succ;
	int num;

	/*�޽u�G����qroot->lchild�}�l����*/
	if ( root->lchild == root ){
		puts("No Data!");
		return;
	}
	printf("Enter a number u want to delete : ");
	scanf("%d", &num);
	ptr_parent = root;
	ptr = root->lchild;
	while ( ptr->number != num ){   /*�j�M�G���𪽨���`�I*/
		ptr_parent = ptr;
		if ( num < ptr->number ){ /*�pnum�Ȥp��ثe�`�I�Blbit��1*/
			if ( ptr->lbit == 1 ) /*(lchild�����`����)�h�����j�M*/
				ptr = ptr->lchild;
			else /*�_�h(lchild���޽u)�Y�䤣��`�I*/
				break;
		}
		else if ( num > ptr->number){
			if ( ptr->rbit == 1 )
				ptr = ptr->rchild;
			else
				break;
		}
	}
	if ( ptr->number != num ){
		puts("Not found number!");
		return;
	}
	printf("Deleting number %d...\n", ptr->number);

	/* �R���𸭸`�I*/
	if ( ptr->lbit == 0 && ptr->rbit == 0 ){
		if ( ptr_parent == root ){    /*�R���Ĥ@�Ӹ`�I*/
			ptr_parent->lchild = root;
			ptr_parent->lbit = 0;
		}  /*�R�����`�I*/
		else  if ( ptr->number < ptr_parent->number ){
			ptr_parent->lchild = ptr->lchild;
			ptr_parent->lbit  = 0;
		}
		else{                   /*�R���k�`�I*/
			ptr_parent->rchild = ptr->rchild;
			ptr_parent->rbit = 0;
		}
		free(ptr);
	}

	/* �R���������׸`�I�A��ܥ��k�U���`�I */
	else if ( ptr->lbit == 1 && ptr->rbit == 1 ){
		/*�Dptr���e��̸`�I, �N�k�l�𴡤J�e��̥k��*/
		ptr_pred = inpred ( ptr);
		ptr_pred->rchild = ptr->rchild;
		ptr_pred->rbit = ptr->rbit;
		ptr_parent->rchild = ptr->lchild; 
		free(ptr);
	}
	else   /*�R���@����׸`�I*/{
		if ( ptr_parent == root ){ /*�R���Ĥ@�`�I*/
			if ( ptr->lbit == 1 ){
				ptr_pred = inpred(ptr);
				root->lchild = ptr->lchild;
				ptr_pred->rchild = root;
			}
			else{
				ptr_succ = insucc(ptr);
				root->lchild = ptr->rchild;
				ptr_succ->lchild = root;
			}
		}
		else{
			/* ��R�����`�I�bptr_parent������� */
			if ( ptr->number < ptr_parent->number ){
				 /* ��ptr�`�I�u���k�䪺�`�I�� */
				if (ptr->rbit == 1 && ptr->lbit == 0) {
					ptr->rchild->lchild = ptr->lchild;
					ptr->rchild->lbit = ptr->lbit;
					ptr_parent->lchild = ptr->rchild; 
					ptr_parent->lbit = ptr->rbit;
				}
				 /* ��ptr�`�I�u�����䪺�`�I�� */
				else if (ptr->rbit == 0 && ptr->lbit == 1){
					ptr->lchild->rchild = ptr->rchild;
					ptr->lchild->rbit = ptr->rchild->rbit;
					ptr_parent->lchild = ptr->lchild; 
					ptr_parent->lbit = ptr->lbit;
				}
			}
			/* ��R�����`�I�bptr_parent���k��� */
			else{
				/* ��ptr�`�I�u�����䪺�`�I�� */
				if (ptr->lbit == 1 && ptr->rbit == 0) {
					ptr_parent->rchild = ptr->lchild;
					ptr_parent->rbit = ptr->lbit;
					ptr->lchild->rchild = ptr->rchild;
					ptr->lchild->rbit = ptr->rbit;
				}
				/* ��ptr�`�I�u���k�䪺�`�I�� */
				else if (ptr->lbit == 0 && ptr->rbit == 1){
					ptr_parent->rchild = ptr->rchild;
					ptr_parent->rbit = ptr->rbit;					
					ptr->rchild->lchild = ptr_parent;
					ptr->rchild->lbit = ptr->lbit; 
				}
			}
			free(ptr);
		}
	}
}

/* �[�J�`�I��k���� */
/* �ǤJ�Ѽ�: */
/* 1. node_parent ���s�`�I�����`�I */
/* 2. node �����s�W���`�I */
void insert_right( struct tbintree *node_parent, struct tbintree *node)
{
	struct tbintree *w;

	node->rchild = node_parent->rchild;
	node->rbit = node_parent->rbit;
	node->lchild = node_parent;
	node->lbit = 0;
	node_parent->rchild = node;
	node_parent->rbit = 1;
	if ( node->rbit == 1 ){   /*node���U�٦�tree*/
		w = insucc( node );
		w->lchild = node;
	}
}

/* �[�J�`�I�󥪤��� */
/* �ǤJ�Ѽ�: */
/* 1. node_parent ���s�`�I�����`�I */
/* 2. node �����s�W���`�I */
void insert_left( struct tbintree *node_parent, struct tbintree *node)
{
	struct tbintree *w;

	node->lchild = node_parent->lchild;
	node->lbit = node_parent->lbit;
	node->rchild = node_parent;
	node->rbit = 0;
	node_parent->lchild = node;
	node_parent->lbit = 1;
	if ( node->lbit == 1 ){    /*node ���U�٦�tree*/
		w = inpred( node );
		w->rchild = node;
	}
}

/* ���ǰl����ܸ`�I��� */
void	inorder_show( struct tbintree *node)
{
	if ( node->lchild == root ){
		puts("No Data!");
		return;
	}
	puts("Inorder Displaying Thread Binary Search Tree");
	ptr = root;
	do{
		ptr = insucc( ptr );
		if ( ptr != root )
			printf("%-5d", ptr->number);
	} while ( ptr != root );
	puts("\n--------------------------------------------");
}

/* �l�ܫ��~�̨�� */
struct tbintree * insucc( struct tbintree *node )
{
	struct tbintree *succ;

	succ = node->rchild;
	if ( node->rbit == 1 )
		while ( succ->lbit == 1 )
			succ = succ->lchild;
	return succ;
}

/* �l�ܫe��̨�� */
struct tbintree *inpred( struct tbintree *node )
{
	struct tbintree *pred;

	pred = node->lchild;
	if ( node->rbit == 1 )
		while ( pred->rbit == 1 )
			pred = pred->rchild;
	return pred;
}

/* �t�m�O������ */
struct tbintree *newtbintree()
{
	struct tbintree *temp;

	temp = (struct tbintree *)malloc(sizeof(struct tbintree));
	temp->lchild = NULL;
	temp->rchild = NULL;
	return temp;
}

