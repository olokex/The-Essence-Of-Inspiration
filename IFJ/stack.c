/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: stack.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "utils.h"
#include "my_malloc.h"

stack_t *init_stack()
{
	stack_t *stack = (stack_t *)my_malloc(sizeof(stack_t));
	stack->top = NULL;
	return stack;
}

void stack_push(stack_t *stack, void *value)
{
	if (!stack)
	{
		return;
	}
	stack_item_t *new_item = (stack_item_t *)my_malloc(sizeof(stack_item_t));
	new_item->value = value;

	if (stack_is_empty(stack))
	{
		stack->top = new_item;		
		new_item->next = NULL;
	}
	else 
	{
		new_item->next = stack->top;
		stack->top = new_item;
	}
}

bool stack_is_empty(stack_t *stack)
{
	return (stack->top) ? false : true;
}

void *stack_top(stack_t *stack)
{
	return (stack_is_empty(stack)) ? NULL : stack->top->value;
}

void *stack_pop(stack_t *stack)
{
        if(stack_is_empty(stack))
                return NULL;

        stack_item_t *tmp = stack->top;
        if (stack->top->next)
        {
                stack->top = stack->top->next;
        }
        else
        {
                stack->top = NULL;
        }
        void *data = tmp->value;
        //free(tmp);
        return data;
}


void free_stack(stack_t *stack)
{
	if (!stack)
	{
		return;
	}
	stack_item_t *item = NULL;

	while (stack->top != NULL)
	{
		item = stack->top;
		stack->top = stack->top->next;
		//free(item);
	}
	//free(stack);
}


void item_print(stack_item_t *item)
{
	if (item)
	{
		printf("%d\n", *((int *)item->value));
	}
}

//for debugging purposes only
void stack_print(stack_t *stack)
{
	if (!stack)
	{
		return;
	}
	stack_item_t *tmp = stack->top;

	while (tmp)
	{
		item_print(tmp);
		tmp = tmp->next;
	}
}

