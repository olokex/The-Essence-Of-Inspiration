/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: ast.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "my_malloc.h"
#include "token.h"
#include <string.h>


//#define DEBUG

#ifdef DEBUG
#define D
#else
#define D for(;0;)
#endif

void statement_list_insert(ast_node_t *node, ast_node_t *statement_list)
{
        D       printf("Inserting statement into statement list.\n");
        ast_node_t *tmp;
        if (statement_list->rptr == NULL)
        {
                statement_list->rptr = node;
                return;
        }
        tmp = statement_list;
        while((tmp->rptr) != NULL)
                tmp = tmp->rptr;
        tmp->rptr = node;
        return;
}

ast_node_t *substr_builtin(ast_node_t *paramlist)
{
	ast_node_t *substr = ast_node_create(o_substr, t_null, NULL, paramlist, NULL);
	return substr;
}

ast_node_t *chr_builtin(ast_node_t *paramlist)
{
	ast_node_t *chr = ast_node_create(o_chr, t_null, NULL, paramlist, NULL);
	return chr;
}

ast_node_t *ord_builtin(ast_node_t *paramlist)
{
	ast_node_t *ord = ast_node_create(o_ord, t_null, NULL, paramlist, NULL);
	return ord;
}

ast_node_t *print_builtin(ast_node_t *paramlist)
{
	char *nil = my_malloc(sizeof(char) * 4);
	strcpy(nil, "nil");
	ast_node_t *print = ast_node_create(o_print, t_null, nil, paramlist, NULL);
	char *nilatnil = my_malloc(sizeof(char) * 8);
	strcpy(nilatnil, "nil@nil");
	print->gvalue = nilatnil;
	return print;
}

ast_node_t *function_call(char *name, ast_node_t *paramlist)
{
        ast_node_t *function = ast_node_create(o_function_call, t_null, name, paramlist, NULL);
        return function;
}


ast_node_t *function_define(char *name, ast_node_t *paramlist, ast_node_t *statement_list)
{
        ast_node_t *function = ast_node_create(o_function_define, t_null, name, paramlist, statement_list);
        return function;
}


ast_node_t *function_param_constant(token_t token)
{
        ast_node_t *param = constant_declare(token);
        param->opcode = o_function_param;
        return param;
}

ast_node_t *function_param_identifier(char *id)
{
        ast_node_t *param = ast_node_create(o_function_param, t_null, id, NULL, NULL);
        return param;
}

ast_node_t *strlen_builtin(ast_node_t *paramlist)
{
	ast_node_t *strlen_node = ast_node_create(o_strlen, t_null, NULL, paramlist, NULL);
	return strlen_node;
}

ast_node_t *if_statement(ast_node_t *expression, ast_node_t *st_list_true, ast_node_t *st_list_false)
{
        ast_node_t *statements_node = ast_node_create(o_if_else, t_null, NULL, st_list_true, st_list_false);
        ast_node_t *if_statement = ast_node_create(o_if_cond, t_null, NULL, expression, statements_node);
        return if_statement;
}

ast_node_t *id_assign(ast_node_t *id, ast_node_t *value)
{
        ast_node_t *assign = ast_node_create(o_var_assign, value->t_type, NULL, id, value);
        return assign;
}

ast_node_t *id_reference(token_t t)
{
        ast_node_t *ref = ast_node_create(o_var, t_null, t.value, NULL, NULL);
        return ref;
}

ast_node_t *id_declare(token_t token)
{
        ast_node_t *id_new = ast_node_create(o_var_declare, t_null, token.value, NULL, NULL);
        return id_new;
}

ast_node_t *while_statement(ast_node_t *expression, ast_node_t *statement_list)
{
        ast_node_t *node = ast_node_create(o_while, t_null, NULL, expression, statement_list);
        return node;
}

ast_node_t *constant_declare(token_t token)
{
        unsigned datatype = tkn_to_datatype(token);
        ast_node_t *constant = NULL;


	if (datatype == t_null)
        {
        
		char *value = my_malloc(sizeof(char) * 4);

		strcpy(value, "nil");
		constant = ast_node_create(o_constant, datatype, value, NULL, NULL);
                return constant;
        }
        else
        {
                constant = ast_node_create(o_constant, datatype, token.value, NULL, NULL);
                return constant;
        }

}

unsigned tkn_to_datatype(token_t token)
{
        switch (token.type)
        {
                case tkn_integer:
                        return t_integer;
                case tkn_double:
                        return t_double;
                case tkn_string:
                        return t_string;
                case tkn_key_nil:
                        return t_null;
        }
}



ast_node_t *statement_make(ast_node_t *root)
{
        ast_node_t *statement = ast_node_create(o_statement, t_null, NULL, root, NULL);
        return statement;
}

ast_node_t *statement_list_init()
{
        ast_node_t *list = ast_node_create(o_statement_list, t_null, NULL, NULL, NULL);
        return list;
}

ast_node_t *program_node(ast_node_t *fnc_defs, ast_node_t *main_statement_list)
{		
        ast_node_t *program_tree = ast_node_create(o_program, t_null, NULL, fnc_defs, main_statement_list);
        return program_tree;
}



ast_node_t *ast_node_create(opcode_t opcode, datatype_t t_type, void *value, ast_node_t *lptr, ast_node_t *rptr)
{
	ast_node_t *node = (ast_node_t *)my_malloc(sizeof(ast_node_t));
	node->opcode = opcode;
	node->t_type = t_type;
	node->value = value;
	node->lptr = lptr;
	node->rptr = rptr;
	node->gvalue = NULL;
	return node;
}

const char *opcode_str[] = {
        ENUM_OPCODE(MAKE_STRING)
};

const char *datatype_str[] = {
	ENUM_DATATYPE(MAKE_STRING)
};

#define RIGHT 1
#define LEFT 0
// for debugging
void ast_print_fnc(ast_node_t *root, int side)
{
	if (root == NULL)
		return;
	printf("\n\n");
	if (side == LEFT)
		printf("This is left node - ");
	else
		printf("This is right node - ");
	printf("AST opcode: %s\nAST value: %s\n", opcode_str[root->opcode], (char *)root->value);

	printf("Node leaves:\n");
	if (root->rptr == NULL)
                printf("Right node - null\n");
        if (root->lptr == NULL)
                printf("Left node - null\n");


	ast_print_fnc(root->lptr, LEFT);
        ast_print_fnc(root->rptr, RIGHT);	
	
}




