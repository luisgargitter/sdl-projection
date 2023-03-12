#include <stdint.h>
#ifndef TYPES_H
	#include "types.h"
#endif
#ifndef STACK_T
#define STACK_T
/**
 * @brief Element on the stack
 * 
 * Used a union so that we can use the same stack implementation for all kinds of data
 */
typedef union
{
	int32_t i;
	surface_t surf;
} stack_item;

typedef struct
{
	int32_t size;		///< Size of the stack in item count
	int32_t num_items;	///< Number of stored items
	stack_item* items;	///< Pointer to the first item
} stack; //Did not use stack_t because it was already defined?

stack_item st_pop(stack* st);
void st_push(stack* st, stack_item item);

stack st_create(int init_size);
void st_free(stack* st);

/// @brief Sets the current stack to all zeroes
void st_clear(stack* st);
#endif