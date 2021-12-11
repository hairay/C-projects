/* file name: Tbintree.c */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

/*	定義資料結構
rbit = 1 , rchild為正常指標, rbit =0 , rchild為引線
lbit = 1 , lchild為正常指標, lbit =0 , lchild為引線 */

struct tbintree {
	int number;
	int rbit; 	
	int lbit;
	struct tbintree *lchild;
	struct tbintree *rchild;
} *root, *ptr, *newnode;

struct tbintree *newtbintree();   /*配置記憶函數*/

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
	/* 初始化 */
	/* 引線二元樹根節點不放資料: root->number不放data */
	/* 右鏈結永遠指向本身 : rbit =1 */
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

/*插入節點函數*/
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
			/*  如新節點小於目前節點且lbit為0 (lchild為引線)
				則插入目前節點左方, 否則ptr往左搜尋  */
			if ( newnode->number < ptr->number ){
				if ( ptr->lbit == 0 ){
					insert_left( ptr, newnode);
					break;
				}
				else
					ptr = ptr->lchild;
			}
			/*  如新節點大於目前節點且rbit為0 (rchild為引線)
				則插入目前節點右方, 否則ptr往右搜尋  */
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

/*刪除節點函數*/
void delete_node()
{
	struct tbintree *ptr_parent;
	struct tbintree *ptr_pred, *ptr_succ;
	int num;

	/*引線二元樹從root->lchild開始放資料*/
	if ( root->lchild == root ){
		puts("No Data!");
		return;
	}
	printf("Enter a number u want to delete : ");
	scanf("%d", &num);
	ptr_parent = root;
	ptr = root->lchild;
	while ( ptr->number != num ){   /*搜尋二元樹直到找到節點*/
		ptr_parent = ptr;
		if ( num < ptr->number ){ /*如num值小於目前節點且lbit為1*/
			if ( ptr->lbit == 1 ) /*(lchild為正常指標)則往左搜尋*/
				ptr = ptr->lchild;
			else /*否則(lchild為引線)即找不到節點*/
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

	/* 刪除樹葉節點*/
	if ( ptr->lbit == 0 && ptr->rbit == 0 ){
		if ( ptr_parent == root ){    /*刪除第一個節點*/
			ptr_parent->lchild = root;
			ptr_parent->lbit = 0;
		}  /*刪除左節點*/
		else  if ( ptr->number < ptr_parent->number ){
			ptr_parent->lchild = ptr->lchild;
			ptr_parent->lbit  = 0;
		}
		else{                   /*刪除右節點*/
			ptr_parent->rchild = ptr->rchild;
			ptr_parent->rbit = 0;
		}
		free(ptr);
	}

	/* 刪除有兩分支度節點，表示左右各有節點 */
	else if ( ptr->lbit == 1 && ptr->rbit == 1 ){
		/*求ptr的前行者節點, 將右子樹插入前行者右方*/
		ptr_pred = inpred ( ptr);
		ptr_pred->rchild = ptr->rchild;
		ptr_pred->rbit = ptr->rbit;
		ptr_parent->rchild = ptr->lchild; 
		free(ptr);
	}
	else   /*刪除一分支度節點*/{
		if ( ptr_parent == root ){ /*刪除第一節點*/
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
			/* 當刪除的節點在ptr_parent的左邊時 */
			if ( ptr->number < ptr_parent->number ){
				 /* 當ptr節點只有右邊的節點時 */
				if (ptr->rbit == 1 && ptr->lbit == 0) {
					ptr->rchild->lchild = ptr->lchild;
					ptr->rchild->lbit = ptr->lbit;
					ptr_parent->lchild = ptr->rchild; 
					ptr_parent->lbit = ptr->rbit;
				}
				 /* 當ptr節點只有左邊的節點時 */
				else if (ptr->rbit == 0 && ptr->lbit == 1){
					ptr->lchild->rchild = ptr->rchild;
					ptr->lchild->rbit = ptr->rchild->rbit;
					ptr_parent->lchild = ptr->lchild; 
					ptr_parent->lbit = ptr->lbit;
				}
			}
			/* 當刪除的節點在ptr_parent的右邊時 */
			else{
				/* 當ptr節點只有左邊的節點時 */
				if (ptr->lbit == 1 && ptr->rbit == 0) {
					ptr_parent->rchild = ptr->lchild;
					ptr_parent->rbit = ptr->lbit;
					ptr->lchild->rchild = ptr->rchild;
					ptr->lchild->rbit = ptr->rbit;
				}
				/* 當ptr節點只有右邊的節點時 */
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

/* 加入節點於右方函數 */
/* 傳入參數: */
/* 1. node_parent 為新節點之父節點 */
/* 2. node 為欲新增之節點 */
void insert_right( struct tbintree *node_parent, struct tbintree *node)
{
	struct tbintree *w;

	node->rchild = node_parent->rchild;
	node->rbit = node_parent->rbit;
	node->lchild = node_parent;
	node->lbit = 0;
	node_parent->rchild = node;
	node_parent->rbit = 1;
	if ( node->rbit == 1 ){   /*node底下還有tree*/
		w = insucc( node );
		w->lchild = node;
	}
}

/* 加入節點於左方函數 */
/* 傳入參數: */
/* 1. node_parent 為新節點之父節點 */
/* 2. node 為欲新增之節點 */
void insert_left( struct tbintree *node_parent, struct tbintree *node)
{
	struct tbintree *w;

	node->lchild = node_parent->lchild;
	node->lbit = node_parent->lbit;
	node->rchild = node_parent;
	node->rbit = 0;
	node_parent->lchild = node;
	node_parent->lbit = 1;
	if ( node->lbit == 1 ){    /*node 底下還有tree*/
		w = inpred( node );
		w->rchild = node;
	}
}

/* 中序追蹤顯示節點函數 */
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

/* 追蹤後繼者函數 */
struct tbintree * insucc( struct tbintree *node )
{
	struct tbintree *succ;

	succ = node->rchild;
	if ( node->rbit == 1 )
		while ( succ->lbit == 1 )
			succ = succ->lchild;
	return succ;
}

/* 追蹤前行者函數 */
struct tbintree *inpred( struct tbintree *node )
{
	struct tbintree *pred;

	pred = node->lchild;
	if ( node->rbit == 1 )
		while ( pred->rbit == 1 )
			pred = pred->rchild;
	return pred;
}

/* 配置記憶體函數 */
struct tbintree *newtbintree()
{
	struct tbintree *temp;

	temp = (struct tbintree *)malloc(sizeof(struct tbintree));
	temp->lchild = NULL;
	temp->rchild = NULL;
	return temp;
}

