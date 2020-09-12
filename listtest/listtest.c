#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "arch.h"
#include "list.h"

#define MAX_SCSI_DBG_CHARS  160
#define NOTE_MSG_LEVEL 0x00000008

struct foo
{
    int info;
    struct list_head list_member;
};

void DbgPrintf(Uint32 msgLevel, const char *fmt, ... )
{   
	va_list varg;
	char where[MAX_SCSI_DBG_CHARS];

	va_start( varg, fmt );
	vsnprintf(where, MAX_SCSI_DBG_CHARS, fmt, varg);
	va_end( varg );
	printf("%s", (char *)where);
	return;
    
}
void add_node(int arg, struct list_head *head)
{
    struct foo *fooPtr = (struct foo *)malloc(sizeof(struct foo));
    assert(fooPtr != NULL);

    fooPtr->info = arg;
    list_add(&fooPtr->list_member, head);
}

void display(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;

    list_for_each(iter, head)
    {
        objPtr = list_entry(iter, struct foo, list_member);
        DbgPrintf(NOTE_MSG_LEVEL, "%d ", objPtr->info);
    }
    DbgPrintf(NOTE_MSG_LEVEL, "\n");
}

void display_reverse(struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;

    list_for_each_prev(iter, head)
    {
        objPtr = list_entry(iter, struct foo, list_member);
        DbgPrintf(NOTE_MSG_LEVEL, "%d ", objPtr->info);
    }
    DbgPrintf(NOTE_MSG_LEVEL, "\n");
}

void delete_all(struct list_head *head)
{
    struct list_head *iter, *q;
    struct foo *objPtr;

//redo:
    list_for_each_safe(iter, q, head)
    {
        objPtr = list_entry(iter, struct foo, list_member);
        list_del(&objPtr->list_member);
        free(objPtr);
        //goto redo;
    }
}

int find_first_and_delete(int arg, struct list_head *head)
{
    struct list_head *iter;
    struct foo *objPtr;

    list_for_each(iter, head)
    {
        objPtr = list_entry(iter, struct foo, list_member);
        if (objPtr->info == arg)
        {
            list_del(&objPtr->list_member);
            free(objPtr);
            return 1;
        }
    }

    return 0;
}

void ListTest(void)
{
    LIST_HEAD(fooHead);

    add_node(10, &fooHead);
    add_node(20, &fooHead);
    add_node(25, &fooHead);
    add_node(30, &fooHead);

    display(&fooHead);
    display_reverse(&fooHead);
    find_first_and_delete(20, &fooHead);
    display(&fooHead);
    display_reverse(&fooHead);
    delete_all(&fooHead);
    display(&fooHead);
}

int main(int argc, char** argv)
{
    int ret = 0;    
    ListTest();

    return ret;
}
