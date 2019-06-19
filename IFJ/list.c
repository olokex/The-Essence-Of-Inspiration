/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: list.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "my_malloc.h"

list_t *init_list()
{
	list_t *new = my_malloc(sizeof(list_t));
	new->head = NULL;
	new->tail = NULL;
	return new;

}

void node_delete(list_node_t *node)
{
	switch(node->token.type)
	{
		case tkn_identifier:
		case tkn_integer:
		case tkn_double:
		case tkn_string:
//			free(node->token.value);
			break;
		default:
			break;
	}
//	free(node);
}



bool list_is_empty(list_t *list)
{
	return list->head ? true : false;
}

void free_list(list_t *list)
{	
	list_node_t *remove = NULL;
	list_node_t *tmp = list->head;
	while (tmp)
	{
		remove = tmp;
		tmp = tmp->next;
		node_delete(remove);
	}
	//free(list);
}


list_node_t *list_create_node(token_t t)
{
	list_node_t *new = my_malloc(sizeof(list_node_t));
	new->next = NULL;
	new->previous = NULL;
	new->token = t;
	return new;
}


void list_delete_first(list_t *list)
{ 
	if (!list->head) return;

	if (!list->head->next)
	{
//		free(list->head);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		list_node_t *tmp = list->head->next;
//		free(list->head);
		tmp->previous = NULL;
		list->head = tmp;
	}

}

void list_delete_active(list_t *list)
{
	if (list->active == list->head && list->active == list->tail)
	{
//		free(list->active);
		list->head = NULL;
		list->head = NULL;
		list->tail = NULL;
	}
	else if (list->active == list->head)
	{
		list->active = list->head->next;
		list->active->previous = NULL;
		list->head = list->active;
	}
	else if (list->active == list->tail)
	{
		list->active = list->tail->previous;
		list->active->next = NULL;
		list->tail = list->active;
	}
	else
	{
		list_node_t *tmp = list->active;
		tmp->previous = list->active;
                tmp->next->previous = list->active;
		tmp->previous->next = tmp->next;
  //              free(tmp);
	}
}

void list_delete_last(list_t *list)
{
	if (!list->head) return;
	
	if (!list->tail->previous)
	{
//		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
	}
	else
	{
		list_node_t *tmp = list->tail->previous;
//		free(list->tail);
		tmp->next = NULL;
		list->tail = tmp;
	}
}


void list_insert_first(list_t *list, token_t t)
{
	list_node_t *node = list_create_node(t);
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->head->previous = node;
		node->next = list->head;
		list->head = node;
	}
}


void list_insert_last(list_t *list, token_t t)
{
	list_node_t *node = list_create_node(t);
	if (list->head == NULL)
        {
                list->head = node;
                list->tail = node;
        }
	else
	{
		list->tail->next = node;
		node->previous = list->tail;
		list->tail = node;
	}
}

void list_insert_active(list_t *list, token_t t)
{
	list_node_t *node = list_create_node(t);
	node->token = t;
	node->previous = list->active;

	if (list->active == list->tail)
	{
		list->tail = node;
	}
	else
	{
		node->next = list->active->next;
		node->next->previous = node;
	}
	list->active->next = node;
}

bool list_is_active(list_t *list)
{
	return list->active ? true : false;
}

void list_activate_first(list_t *list)
{
	list->active = list->head;
}

void list_activate_last(list_t *list)
{
	list->active = list->tail;
}

token_t list_get_active(list_t *list)
{
	return list->active->token;
}

token_t list_get_after_active(list_t *list)
{
	return list->active->next->token;
}

token_t list_get_last(list_t *list)
{
	return list->tail->token;
}

bool list_active_previous(list_t *list)
{
        if ((list->active = list->active->previous) != NULL)
                return true;
        return false;

}

bool list_active_next(list_t *list)
{
        if ((list->active = list->active->next) != NULL)
                return true;
        return false;
}

void list_delete_more(list_t *list, unsigned n)
{
        list_node_t *tmp = list->active->next;
        list_node_t *remove;
        while (tmp && n > 0)
        {
                remove = tmp;
                tmp = tmp->next;
  //            free(remove);
                remove = NULL;
                n--;
        }
        if (tmp)
        {
                tmp->previous = list->active;
                list->active->next = tmp;
        }
        else
        {
                list->tail = list->active;
                list->active->next = NULL;
        }
}

void list_print(list_t *list)
{
	list_node_t *tmp = list->head;
	while (tmp)
	{
		printf("Token value: %s\n", tokentype_str[tmp->token.type]);
		tmp = tmp->next;
	}
}


