/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: list.h
 *
 */
#ifndef _LIST_H_
#define _LIST_H_

#include "token.h"
#include <stdbool.h>

typedef struct node {
	token_t token;
	struct node *next;
	struct node *previous;
} list_node_t;

typedef struct {
	list_node_t *head;
	list_node_t *tail;
	list_node_t *active;
} list_t;


list_t *init_list();

void free_list(list_t *list);

void list_insert_first(list_t *list, token_t t);

void list_delete_first(list_t *list);

void list_delete_active(list_t *list);

void list_delete_last(list_t *list);

bool list_is_empty(list_t *list);

void list_insert_last(list_t *list, token_t t); 

void list_insert_active(list_t *list, token_t t);

bool list_is_active(list_t *list);

void list_activate_first(list_t *list);

void list_activate_last(list_t *list);

token_t list_get_active(list_t *list);

token_t list_get_after_active(list_t *list);

token_t list_get_last(list_t *list);

bool list_active_previous(list_t *list);

bool list_active_next(list_t *list);

/**
* deletes more nodes, starts from list->tail
* @param n number of nodes to delete
*/
void list_delete_more(list_t *list, unsigned n);

//for debugging
void list_print(list_t *list);

#endif

