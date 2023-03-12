#include <stdlib.h>
#include "stack.h"

stack st_create(int init_size)
{
	stack st;
	st.size = init_size;
	st.num_items = 0;
	st.items = (stack_item*)malloc(st.size * sizeof(stack_item));
	return st;
}

void st_free(stack* st)
{
	free(st->items);
}

void st_clear(stack* st)
{
	memset(st->items, 0, st->size * sizeof(stack_item));
}

static void st_resize(stack* st, int new_size)
{
	st->size = new_size;
	stack_item* new_items = malloc(st->size * sizeof(stack_item));
	memcpy(new_items, st->items, st->num_items*sizeof(stack_item));
	free(st->items);
	st->items = new_items;
}

stack_item st_pop(stack* st)
{
	stack_item toReturn;
	if(st->num_items <= 0) //TODO: proper error handling
	{ 
		toReturn.i = -1;
		return toReturn;
	} 
	toReturn = st->items[st->num_items-1];
	st->num_items--;
	if(st->num_items <= (st->size)/2)
	{
		st_resize(st, st->num_items); //Resize if half or less of the stack is used
	}
	return toReturn;
}

void st_push(stack* st, stack_item item)
{
	if(st->num_items >= st->size)
	{
		st_resize(st, (st->size) * 2); //Double the size if stack is already full
	}
	st->items[st->num_items] = item;
	st->num_items++;
}