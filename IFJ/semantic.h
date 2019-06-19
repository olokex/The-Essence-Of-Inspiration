/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: semantic.h
 *
 */
#ifndef _SEMANTIC_H_
#define _SEMANTIC_H_

#define INSIDE_DEF 25 //some number
#define NOT_INSIDE_DEF 26 //some number
#define CAN_BE_RETURN 27 //some number
#define ASSIGN 28 //some number

#include "semantic.h"
#include "symtable.h"
#include "error.h"
#include "list.h"
#include "syntax.h"
#include  "scanner.h"
#include "my_malloc.h"

/*
* @brief semanticka kontrola vestavene funkce chr
* @param ukazatel na lokalni tabulku
*/

void sem_chr(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce ord
* @param ukazatel na lokalni tabulku
*/
void sem_ord(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce substr
* @param ukazatel na lokalni tabulku
*/
void sem_substr(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce print
* @param ukazatel na lokalni tabulku
*/
void sem_print(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce length
* @param ukazatel na lokalni tabulku
*/
void sem_length(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce inputi
* @param ukazatel na lokalni tabulku
*/
void sem_inputi(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce inputf
* @param ukazatel na lokalni tabulku
*/
void sem_inputf(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola vestavene funkce inputs
* @param ukazatel na lokalni tabulku
*/
void sem_inputs(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola celeho dokumentu, ktera postupne vola ostatni fce
* @param ukazatel na globalni predvyplnenou tabulku vsech funkci ktere budou definovane
* @param ukazatel na celkove pole tokenu ktere predala syntaxe
*/
void semantic_check(symtable_node_t *all_def_funkce, list_t * all_tokens_came);
/*
* @brief semanticka kontrola ve fazi pokud prisel if
* @param ukazatel na lokalni tabulku
* @param zanoreni, pokud jsme ve funkci ci nejsme
*/
void semantic_if_came(symtable_node_t **tabulka, unsigned int zanoreni);
/*
* @brief semanticka kontrola ve fazi pokud prisel while
* @param ukazatel na lokalni tabulku
* @param zanoreni, pokud jsme ve funkci ci nejsme
*/
void semantic_while_came(symtable_node_t **tabulka, unsigned int zanoreni);
/*
* @brief semanticka kontrola bool vyrazu
* @param ukazatel na lokalni tabulku
*/
void semantic_expr_bool(symtable_node_t **tabulka);
/*
* @brief semanticka kontrola aritmetickeho vyrazu
* @param ukazatel na lokalni tabulku
* @param pokud se bude jednat o prirazeni ci nikoliv
*/
void semantic_expr_aritmetic(symtable_node_t **tabulka, unsigned int type);
/*
* @brief semanticka kontrola po te co prisel nejaky identifikator
* @param ukazatel na lokalni tabulku
* @param token ktery prisel pred zavolanim teto funkce
* @param zanoreni, pokud jsme ve funkci ci nejsme
*/
void semantic_id_came(symtable_node_t **tabulka, token_t came, unsigned int zanoreni);
/*
* @brief semanticka kontrola prirazeni do promenne
* @param ukazatel na lokalni tabulku
* @param token do ktereho probehne prirazeni
* @param zanoreni, pokud jsme ve funkci ci nejsme
*/
void semantic_assign(token_t came, symtable_node_t **tabulka, unsigned int zanoreni);
/*
* @brief semanticka kontrola volani uzivatelske funkce
* @param token ktery prisel pred zavolanim teto funkce
* @param ukazatel na lokalni tabulku
* @param zanoreni, pokud jsme ve funkci ci nejsme
*/
void semantic_call_func(token_t came, symtable_node_t **tabulka, unsigned int zanoreni);
/*
* @brief semanticka kontrola definice fce
* @param ukazatel na lokalni tabulku (v tomto pripade tabulka hlavniho tela)
*/
void semantic_def_func(symtable_node_t **main_tabulka);
/*
* @brief semanticka kontrola prikazu ktere se muzou provest uvnitr def fce nebo ifu nebo whilu
* @param ukazatel na lokalni tabulku
* @param zanoreni pokud se nacjhazime v def fce nebo ne
*/
void semantic_inside_commands_end(symtable_node_t **local_tabulka, unsigned int zanoreni);


#endif // !_SEMANTIC_H_