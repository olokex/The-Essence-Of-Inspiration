/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: symtable.c
 *
 */

#include "symtable.h"
#include "my_malloc.h"

symtable_node_t *symtable_init()
{
	return NULL;
}	

bool symtable_search(symtable_node_t *ptr_root, char *key, symtable_value_t content)
{		
	if(ptr_root == NULL)
		return false;//pokud sme dosli na konec
	if(strcmp(ptr_root->key, key) == 0)
	{//nalezen
		*content = *ptr_root->value;
		return true;
	}
	//pokud jeste hledame
	if(strcmp(key, ptr_root->key) < 0)//je mensi jdeme doleva
		return symtable_search(ptr_root->lptr,key,content);
	else//vetsi --> doprava
		return symtable_search(ptr_root->rptr,key,content);
} 


void symtable_insert(symtable_node_t **ptr_root, char *key, symtable_value_t content)
{		
	if (*ptr_root==NULL)
	{//pokud jsme dosli na konec, vytvoreni listu
		(*ptr_root) = my_malloc(sizeof(symtable_node_t));		
		//ptr_root->key=key;
		(*ptr_root)->key = my_malloc(strlen(key) * sizeof(char) + 1);
		strcpy((*ptr_root)->key,key);
		(*ptr_root)->value=content;
		(*ptr_root)->lptr=NULL;
		(*ptr_root)->rptr=NULL;
	}
	else if(strcmp((*ptr_root)->key, key) == 0)
		(*ptr_root)->value=content;
	else if(strcmp(key, (*ptr_root)->key) < 0)//je mensi jdeme doleva
		symtable_insert(&(*ptr_root)->lptr,key,content);
	else //vetsi --> doprava
		symtable_insert(&(*ptr_root)->rptr,key,content);
}




