/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: expression.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "token.h"
#include "stack.h"
#include "my_malloc.h"
#include "ast.h"
#include "list.h"
#include "symtable.h"
#include "error.h"

unsigned precedence_result(unsigned op);

int is_operator(token_t t);

int is_operand(token_t t);

void token_stack_push(stack_t *stack, token_t tkn);

void token_print(token_t t);

list_t *infix_to_postfix(list_t *tkn_source, symtable_node_t *local_table);

void reduce_terminals(list_t *tokenlist);

ast_node_t *make_tree(list_t *tokenlist);

void generate_code(ast_node_t *tree);

int datatype_cast(ast_node_t *a, ast_node_t *b);

ast_node_t *expression(list_t *tkn_source, symtable_node_t *local_table);

ast_node_t *expression(list_t *tkn_source, symtable_node_t *local_table)
{
	list_t *tokenlist = infix_to_postfix(tkn_source, local_table); // nejdrive prevede tokeny z infixu na postfix a ulozi do listu

        list_insert_first(tokenlist, (token_t) { .type = tkn_null, .value = NULL }); // null token jako oznaceni konce vyrazu
        reduce_terminals(tokenlist); // udela z terminalu koncove uzly
        ast_node_t *tree = make_tree(tokenlist); // postavi AST
	// for debugging
//	generate_code(tree);	
	return tree;
}


void generate_code(ast_node_t *tree)
{
	if (tree == NULL)
	{
		return;
	}
	generate_code(tree->lptr);
	generate_code(tree->rptr);
	switch (tree->opcode)
	{
		case o_constant:
			printf("PUSH %s\n", (char *)tree->value);
			break;
		case o_var:
			printf("PUSH %s\n", (char *)tree->value);
			break;
		case o_sum:
			printf("ADD\n");
			break;
		case o_sub:
			printf("SUB\n");
			break;
		case o_mul:
			printf("MUL\n");
			break;
		case o_div:
			printf("DIV\n");
			break;
		case o_eq:
			printf("IS EQUAL\n");
			break;
		case o_neq:
                        printf("IS NOT EQUAL\n");
                        break;
		case o_lt:
                        printf("IS LESS\n");
                        break;
		case o_leq:
                        printf("IS LESS OR EQUAL\n");
                        break;
		case o_gt:
                        printf("IS GREATER\n");
                        break;
		case o_geq:
                        printf("IS GREATER OR EQUAL\n");
                        break;
		default:
			printf("ERR - INVALID OPCODE: %s\n", opcode_str[tree->opcode]);
			break;
	}
}


void reduce_terminals(list_t *tokenlist)
{
	list_activate_last(tokenlist);
	while (list_get_active(tokenlist).type != tkn_null)
        {		
                token_t t = list_get_active(tokenlist);
		if (is_operand(t))
		{
			ast_node_t *node;
			switch (t.type)
			{         
			        case tkn_integer:
				case tkn_double:
				case tkn_string:
				case tkn_key_nil:
				{
					node = constant_declare(t);
					break;
				}
				case tkn_identifier:
				{
					node = id_reference(t);
					break;
				}
			}

			tokenlist->active->token.value = node;
			tokenlist->active->token.type = tkn_node;
        	}
		list_active_previous(tokenlist);
	}
}

int datatype_cast(ast_node_t *a, ast_node_t *b)
{
	if (a->t_type == t_integer && b->t_type == tkn_integer) return t_integer;
	else if (a->t_type == t_integer && b->t_type == t_double) return t_double;
	else if (a->t_type == t_double && b->t_type == t_integer) return t_double;
	else if (a->t_type == t_double && b->t_type == tkn_double) return t_double;
	return t_null;
}

ast_node_t *make_tree(list_t *tokenlist)
{
	list_activate_last(tokenlist);
	while (list_get_active(tokenlist).type != tkn_null)
	{
		ast_node_t *a; // operand
		ast_node_t *b; // operand
		ast_node_t *node;
		token_t op;
		while ((op = list_get_active(tokenlist)).type == tkn_node)
		{
			list_active_previous(tokenlist);
		}
		if (op.type == tkn_null)
		{
			break;
		}
		a = tokenlist->active->next->token.value;
		b = tokenlist->active->next->next->token.value;

		switch (op.type)
		{
			case tkn_plus:
			{
				
				node = ast_node_create(o_sum, t_null, NULL, b, a);
				break;
			}
			case tkn_minus:
			{
				node = ast_node_create(o_sub, t_null, NULL, b, a);
				break;
			}
			case tkn_star:
			{
				node = ast_node_create(o_mul, t_null, NULL, b, a);
				break;
			}
			case tkn_slash:
			{
				node = ast_node_create(o_div, t_null, NULL, b, a);
				break;
			}
			case tkn_equal_2:
			{
				node = ast_node_create(o_eq, t_boolean, NULL, b, a);
				break;
			}
			case tkn_not_equal:
                        {
                                node = ast_node_create(o_neq, t_boolean, NULL, b, a);
                                break;
                        }
			case tkn_less:
                        {
                                node = ast_node_create(o_lt, t_boolean, NULL, b, a);
                                break;
                        }
			case tkn_less_or_equal:
                        {
                                node = ast_node_create(o_leq, t_boolean, NULL, b, a);
                                break;
                        }
			case tkn_more:
                        {
                                node = ast_node_create(o_gt, t_boolean, NULL, b, a);
                                break;
                        }
			case tkn_more_or_equal:
                        {
                                node = ast_node_create(o_geq, t_boolean, NULL, b, a);
                                break;
                        }
		}
				tokenlist->active->token.type = tkn_node;
                                tokenlist->active->token.value = node;
                                list_delete_more(tokenlist, 2);
	}
	list_active_next(tokenlist); // presun na root uzel
	ast_node_t *tree = list_get_active(tokenlist).value;
	return tree;
}

list_t *infix_to_postfix(list_t *tkn_source, symtable_node_t *local_table)
{
	stack_t *stack = init_stack();
	list_t *toklist = init_list();
	token_t *t;
	token_t a;
	int i = 1; //debug
	while ((a = list_get_active(tkn_source)).type != tkn_new_line && a.type != tkn_end_of_file && a.type != tkn_key_do && a.type != tkn_key_then) // dokud nenalezneme na konec radku
	{
		if (is_operand(a)) // token je operand
		{
			if (a.type == tkn_identifier)
			{
				symtable_value_t idc = my_malloc(sizeof(symtable_value_t));
				if (symtable_search(local_table, a.value, idc) == false)
					semantic_error("undefined variable in expression\n", 0);
			}
			list_insert_first(toklist, a);
		}
		else if (a.type == tkn_left_bracket) // leva zavorka
		{
			token_stack_push(stack, a);
		}
		else if (a.type == tkn_right_bracket) // prava zavorka
		{
			while ((((token_t *)stack_top(stack))->type) != tkn_left_bracket) // popujeme ze stacku dokud nenarazime na levou zavorku
			{	
				t = (token_t *)stack_pop(stack);
				list_insert_first(toklist, *t);
			}
			stack_pop(stack); // pop leve zavorky
		}
		else if (is_operator(a))
		{

			while (!stack_is_empty(stack) && (precedence_result(a.type) <= precedence_result(((token_t *)stack_top(stack))->type)))
			{
				t = (token_t *)stack_pop(stack);
				list_insert_first(toklist, *t);
			}
			token_stack_push(stack, a); // push tokenu na stack
		}
		else
		{
			fprintf(stderr, "Neplatny typ tokenu na vstupu: %s\n", tokentype_str[a.type]);
			exit(4);
		}	
		list_active_next(tkn_source);	
	}
	//popneme zbytek operatoru ze stacku
	while ((t = (token_t *)stack_pop(stack)) != NULL)
	{
		list_insert_first(toklist, *t);
	}

        return toklist;
}

void token_print(token_t t)
{
	if (t.type == tkn_identifier)
	{
		printf("Tokentype: %s | Value: %s\n", tokentype_str[t.type], (char *)t.value);
	}
	else
	{
		printf("Tokentype: %s\n", tokentype_str[t.type]);
	}
}

int is_operator(token_t t)
{
	switch (t.type)
	{
		case tkn_star:
		case tkn_slash:
		case tkn_plus:
		case tkn_minus:
		case tkn_more:
		case tkn_more_or_equal:
		case tkn_less:
		case tkn_less_or_equal:
		case tkn_equal_2:
		case tkn_not_equal:
			return 1;
		default:
			return 0;
	}
}

unsigned precedence_result(unsigned op)
{
	switch (op)
	{
		case tkn_star:
		case tkn_slash:
			return 3;
		case tkn_plus:
		case tkn_minus:
			return 2;
		case tkn_more:
		case tkn_more_or_equal:
		case tkn_less:
		case tkn_less_or_equal:
			return 1;
		default:
			return 0;
	}
}

int is_operand(token_t t)
{
	switch (t.type)
	{
		case tkn_identifier:
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			return 1;
		default:
			return 0;
	}
}

void token_stack_push(stack_t *stack, token_t tkn)
{
        token_t *t = my_malloc(sizeof(token_t));
        memcpy(t, &tkn, sizeof(token_t));
        stack_push(stack, t);
}



