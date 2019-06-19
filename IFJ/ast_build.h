/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: ast_build.h
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
#include "ast.h"


ast_node_t *ast_chr(symtable_node_t **tabulka);

ast_node_t *ast_ord(symtable_node_t **tabulka);

ast_node_t *ast_substr(symtable_node_t **tabulka);

ast_node_t *ast_print(symtable_node_t **tabulka);

ast_node_t *ast_length(symtable_node_t **tabulka);

ast_node_t *ast_inputi(symtable_node_t **tabulka);

ast_node_t *ast_inputf(symtable_node_t **tabulka);

ast_node_t *ast_inputs(symtable_node_t **tabulka);

ast_node_t *ast_build(symtable_node_t *all_def_funkce, list_t * all_tokens_came);

ast_node_t *ast_if_came(symtable_node_t **tabulka, unsigned int zanoreni);

ast_node_t *ast_while_came(symtable_node_t **tabulka, unsigned int zanoreni);

ast_node_t *ast_id_came(symtable_node_t **tabulka, token_t came, unsigned int zanoreni);

ast_node_t *ast_assign(token_t came, symtable_node_t **tabulka, unsigned int zanoreni);

ast_node_t *ast_call_func(token_t came, symtable_node_t **tabulka, unsigned int zanoreni);

void ast_def_func(symtable_node_t **main_tabulka);

ast_node_t *ast_inside_commands_end(symtable_node_t **local_tabulka, unsigned int zanoreni);


#endif // !_AST_BUILD_H_
