/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: symtable.h
 *
 */

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

typedef int* symtable_value_t;

/* uzel stromu */
                                                                                                            
typedef struct symtable_node {
	char *key;			                                                      /* klÃ­Ä */
	symtable_value_t value;                                            /* uÅ¾iteÄnÃ½ obsah uzlu */
	struct symtable_node * lptr;                                    /* levÃ½ podstrom */
	struct symtable_node * rptr;                                   /* pravÃ½ podstrom */
} symtable_node_t;	

/* prototypy funkcÃ­ */


/**
* inicializuje root prvek v tabulce symbolu
* @return pointer na root uzel tabulky
*/
symtable_node_t *symtable_init();

/**
* vyhleda prvek v tabulce podle klice a obsah uzlu zkopiruje do content
* @param ptr_root pointer na root uzel
* @param key klic podle ktereho hledame v tabulce
* @param content ukazatel kam se zkopiruje obsah nalezeneho uzlu
* @return true pokud byl uzel nalezen, jinak false
*/
bool symtable_search(symtable_node_t *ptr_root, char *key, symtable_value_t content);

/**
* vlozi obsah contentu do uzlu s danym klicem 
* pokud je uzel plny, obsah se prepise
* @param ptr_root pointer na root uzel
* @param key klic podle ktereho vyhledavame
* @param content obsah ktery se zkopiruje do uzlu
*/
void symtable_insert(symtable_node_t **ptr_root, char *key, symtable_value_t content);


#endif
