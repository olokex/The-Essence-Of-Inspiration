/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: ast.h
 *
 */
#ifndef _AST_H_
#define _AST_H_

#include <stdio.h>
#include <stdbool.h>
#include "token.h"


// vypsat vsechny operace ktere se budou provadet

#define ENUM_OPCODE(a)\
        a(o_var)\
        a(o_sum)\
        a(o_sub)\
        a(o_mul)\
        a(o_div)\
        a(o_null)\
        a(o_concat)\
        a(o_constant)\
        a(o_var_declare)\
        a(o_var_assign)\
        a(o_function_param)\
        a(o_function_define)\
        a(o_function_call)\
        a(o_eq)\
        a(o_neq)\
        a(o_lt)\
        a(o_gt)\
        a(o_geq)\
        a(o_leq)\
        a(o_print)\
       	a(o_strlen)\
	a(o_inputs)\
	a(o_inputi)\
	a(o_inputf)\
	a(o_chr)\
	a(o_ord)\
	a(o_substr)\
	a(o_while)\
        a(o_if_cond)\
        a(o_if_else)\
        a(o_program)\
        a(o_statement)\
        a(o_statement_list)

typedef enum {
	ENUM_OPCODE(MAKE_ENUM)
} opcode_t;

extern const char *opcode_str[];

// datove typy

#define ENUM_DATATYPE(a)\
	a(t_integer)\
	a(t_double)\
	a(t_string)\
	a(t_boolean)\
	a(t_null)\
	a(t_id)

typedef enum {
	ENUM_DATATYPE(MAKE_ENUM)
} datatype_t;

extern const char *datatype_str[];

// uzel abstraktniho datoveho stromu
typedef struct ast_node {
	opcode_t opcode;
	datatype_t t_type;
	void *value;
	void *gvalue;
	struct ast_node *lptr;
	struct ast_node *rptr;
} ast_node_t;


void statement_list_insert(ast_node_t *node, ast_node_t *statement_list);

void paramlist_insert(ast_node_t *paramlist, ast_node_t *param);

ast_node_t *substr_builtin(ast_node_t *paramlist);

ast_node_t *ord_builtin(ast_node_t *paramlist);

ast_node_t *chr_builtin(ast_node_t *paramlist);

ast_node_t *statement_list_init();

ast_node_t *function_call(char *name, ast_node_t *paramlist);

ast_node_t *function_define(char *name, ast_node_t *paramlist, ast_node_t *statement_list);

ast_node_t *function_param_constant(token_t token);

ast_node_t *if_statement(ast_node_t *expression, ast_node_t *st_list_true, ast_node_t *st_list_false);

ast_node_t *while_statement(ast_node_t *expression, ast_node_t *statement_list);

ast_node_t *id_reference(token_t t);

ast_node_t *function_param_identifier(char *id);

ast_node_t *id_assign(ast_node_t *id, ast_node_t *value);

ast_node_t *strlen_builtin(ast_node_t *paramlist);

ast_node_t *statement_make(ast_node_t *root);

ast_node_t *id_declare(token_t token);

ast_node_t *constant_declare(token_t token);

unsigned tkn_to_datatype(token_t token);

ast_node_t *program_node(ast_node_t *fnc_defs, ast_node_t *main_statement_list);

ast_node_t *print_builtin(ast_node_t *paramlist);

#endif

/**
* vytvori novy uzel pro AST
* @param opcode id provadene operace
* @param t_type datovy typ se kterym uzel pracuje (int/double/string/null)
* @param value hodnota datoveho typu
* @param lptr levy navazujici uzel
* @param rptr pravy navazujici uzel
* @return pointer na nove vytvoreny uzel
*/
ast_node_t *ast_node_create(opcode_t opcode, datatype_t t_type, void *value, ast_node_t *lptr, ast_node_t *rptr);

void ast_print_fnc(ast_node_t *root, int side);




