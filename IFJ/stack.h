/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: stack.h
 *
 */


#ifndef _STACK_H_
#define _STACK_H_


#include <stdbool.h>


//jeden item stacku
typedef struct stack_item {
	void *value;
	struct stack_item *next;
} stack_item_t;

// stack
typedef struct {
	stack_item_t *top;
} stack_t;


/**
* inicializuje stack
* @return pointer na novy stack
*/
stack_t *init_stack();


/**
* uvolni pamet alokovanou pro dany stack
* @param stack pointer na stack
*/
void free_stack(stack_t *stack);

/**
* pushne hodnotu na stack
* @param stack pointer na stack
* @param value hodnota kterou chceme pushnout
*/
void stack_push(stack_t *stack, void *value);


/**
* popne hodnotu ze stacku
* @param stack ptr na stack
* @return ptr na popnutou hodnotu
*/
void *stack_pop(stack_t *stack);

/**
* zkontroluje zda je stack prazdny
* @param stack ptr na stack
* @return true pokud je stack prazdny, jinak false
*/
bool stack_is_empty(stack_t *stack);

/**
* urci hodnotu na vrcholu stacku
* @param stack ptr na stack
* @return ptr na hodnotu na vrcholu stacku, pokud je stack prazdny vraci NULL
*/
void *stack_top(stack_t *stack);

/**
* vytiskne obsah zasobniku
* @param stack ptr na stack
*/
void stack_print(stack_t *stack);

#endif
