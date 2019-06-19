/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: generate.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "generate.h"
#include "my_malloc.h"


// ramce
#define TF 0
#define LF 1
#define GF 2

void generate(ast_node_t *root);

void process(ast_node_t *node);

void process_constant(ast_node_t *node);

void process_var(ast_node_t *node, unsigned frame);

void process_expression(ast_node_t *node);

void process_print(ast_node_t *node);

void add_expression(ast_node_t *node);

void sub_expression(ast_node_t *node);

void mul_expression(ast_node_t *node);

void div_expression(ast_node_t *node);

void write_constant(ast_node_t *node);

void process_inputs(ast_node_t *node);

void process_inputi(ast_node_t *node);

void process_inputf(ast_node_t *node);

void process_chr(ast_node_t *node);

void stmt_declare_vars(ast_node_t *node);

void process_substr(ast_node_t *node);

void process_fnc_define(ast_node_t *node);

void process_if_stmt(ast_node_t *node);

void process_while_stmt(ast_node_t *node);

// bool funkce
void process_expr_var(ast_node_t *root); // zavola se pokud je ve vyrazu pouze 1 promenna a nic vic

void process_leq(ast_node_t *node);

void process_geq(ast_node_t *root);

void process_gt(ast_node_t *root);

void process_lt(ast_node_t *root);

void process_eq(ast_node_t *root);

void process_neq(ast_node_t *root);


void define_globals(ast_node_t *root);

char *unique_return_id();

static int label_cnt = 0;

static int return_id = 0;

static const char *tmp_bool = "gf@$tmp_bool";

static const char *global_retval = "gf@$global_retval";

// definice vsech globalnich promennych potrebnych pro dany program (vyrazy...)
void define_globals(ast_node_t *root)
{
	if (root == NULL)
		return;
	define_globals(root->lptr);
	define_globals(root->rptr);
	char *ret_id;
	switch (root->opcode)
	{
		case o_program:
			printf("defvar %s\ndefvar %s\n", tmp_bool, global_retval);
			return_id = 0; // vynulovani unikatniho id
			break;
		case o_sum:	
		case o_sub:
		case o_mul:
		case o_div:
		case o_inputi:
		case o_inputf:
		case o_inputs:
		case o_chr:
		case o_ord:
		case o_substr:
		case o_strlen:
			ret_id = unique_return_id();
			printf("defvar %s\n", ret_id);
			break;
		default:
			break;
	}
}



char *unique_return_id()
{
	int length = snprintf(NULL, 0, "%d", return_id);
	char *label_cnt_str = my_malloc(length + 1);
	snprintf(label_cnt_str, length + 1, "%d", return_id);
	char *ret = my_malloc(length + 13);
	strcpy(ret, "gf@$retvalue");
	strcat(ret, label_cnt_str);
	return_id++;
	return ret;
}


void generate(ast_node_t *root)
{
	process(root);
}


//funkce print
void process_print(ast_node_t *node)
{
	if (node->lptr->rptr == NULL) // nejsou parametry
		return;
	ast_node_t *tmp = node->lptr->rptr; // ukazuje na prvni parametr
	while (tmp != NULL)
	{
//		printf("debug - print node value: %s, datatype: %s\n", (char *)tmp->value, datatype_str[tmp->t_type]);

		printf("write ");
		write_constant(tmp);
		tmp = tmp->rptr;
	}
	printf("move %s nil@nil\n", global_retval);
}




// pro vypis pri move, print, atp ...
void write_constant(ast_node_t *node) {
	char *end_ptr_f;
	char *end_ptr_i;

	int i;
	double d;

	switch (node->t_type)
	{
		case t_integer:
			i = strtol(node->value, &end_ptr_i, 10);
			printf("int@%d\n", i);
			break;	
		case t_double:
			d = strtod(node->value, &end_ptr_f);
			printf("float@%a\n", d);
			break;
		case t_string:
			printf("string@");
			for (int i = 0; i < strlen(node->value); i++)
			{
				printf("\\%03d", ((char *)node->value)[i]);
			}
			printf("\n");
			break;	
		case t_null:
		case t_id:
			if (node->gvalue == NULL) // pro print funkci
			{
				process_var(node, LF);
			}
			printf("%s\n", (char*)node->gvalue);
		break;

	}
}






// prida ke konstante jeji prefix
void process_constant(ast_node_t *node)
{
	char *datatype = my_malloc(sizeof(char) * strlen(node->value) + 8);
	memset(datatype, 0, sizeof(char) * strlen(node->value) + 8);
	switch (node->t_type)
	{
		case t_integer:
			strcpy(datatype, "int@");
			break;
		case t_double:
			strcpy(datatype, "float@");
			break;
		case t_string:
			strcpy(datatype, "string@");
			break;
		case t_null:
			strcpy(datatype, "nil@");
			break;
		default: 
			return;
	}
	node->gvalue = strcat(datatype, (char *)node->value);
}

//prida k id prefix LF@
void process_var(ast_node_t *node, unsigned frame)
{
	char *variable = my_malloc(sizeof(char) * strlen(node->value) + 4);
	memset(variable, 0, sizeof(char) * strlen(node->value) + 4);
	switch (frame)
	{
		case TF:
			strcpy(variable, "TF@");
			break;
		case LF:
			strcpy(variable, "LF@");
			break;
		case GF:
			strcpy(variable, "GF@");
			break;
	}
	node->gvalue = strcat(variable, node->value);
}

//zpracuje vyraz a vysledek ulozi do gvalue nejvyssiho uzlu
void process_expression(ast_node_t *node)
{
	if (node == NULL)
		return;
	switch (node->opcode)
	{
		case o_sum:
		        process_expression(node->lptr);
		        process_expression(node->rptr);
			add_expression(node);
			break;
		case o_sub:
		        process_expression(node->lptr);
		        process_expression(node->rptr);
			sub_expression(node);
			break;
		case o_mul:
		        process_expression(node->lptr);
		        process_expression(node->rptr);
			mul_expression(node);
			break;
		case o_div:
			process_expression(node->lptr);
		        process_expression(node->rptr);
			div_expression(node);
			break;
		case o_constant:
			process_constant(node);
			break;
		case o_var:
			process_var(node, LF);
			break;
		default:
			break;
	}
}

void div_expression(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	printf("createframe\n"
	
	"defvar tf@a\n"
	"defvar tf@b\n"
	"move tf@a "); // a
	write_constant(node->lptr);
	printf("move tf@b "); // b
	write_constant(node->rptr);
	printf("pushframe\n"
	"defvar lf@ab$retval\n"
	"defvar lf@a$type\n"
	"defvar lf@b$type\n"
	"type lf@a$type lf@a\n"
	"type lf@b$type lf@b\n"
	"\n"
	"jumpifeq ab$same_type%u lf@a$type lf@b$type\n"
	"jumpifeq a$is_int%u lf@a$type string@int\n"
	"jumpifeq b$is_int%u lf@b$type string@int\n"
	"exit int@4\n"
	"\n"
	"# pretypovani na float\n"
	"label a$is_int%u\n"
	"jumpifeq b$convert_to_float%u lf@b$type string@float\n"
	"exit int@4\n"
	"\n"
	"label b$convert_to_float%u\n"
	"int2float lf@a lf@a\n"
	"jump ab$div%u\n"
	"\n"
	"label b$is_int%u\n"
	"jumpifeq a$convert_to_float%u lf@a$type string@float\n"
	"exit int@4\n"
	"\n"
	"label a$convert_to_float%u\n"
	"int2float lf@b lf@b\n"
	"#jump ab$same_type\n"
	"jump ab$div%u\n"
	"\n"
	"# spravne minus pokud jsou oba stejne\n"
	"label ab$same_type%u\n"
	"jumpifeq ab$idiv%u lf@a$type string@int\n"
	"jumpifeq ab$div%u lf@a$type string@float\n"
	"# semanticka chyba nekompatibility typu\n"
	"exit int@4\n"
	"\n"
	"# deleni pro float\n"
	"label ab$div%u\n"
	"jumpifeq ab$zero_division%u float@0x0p+0 lf@b\n"
	"div lf@ab$retval lf@a lf@b\n"
	"jump ab$end%u\n"
	"\n"
	"# deleni pro int\n"
	"label ab$idiv%u\n"
	"jumpifeq ab$zero_divison%u int@0 lf@b\n"
	"idiv lf@ab$retval lf@a lf@b\n"
	"jump ab$end%u\n"
	"\n"
	"# deleni nulou\n"
	"label ab$zero_divison%u\n"
	"exit int@9\n"
	"\n"
	"# konec a ulozeni do retval\n"
	"label ab$end%u\n"
	"move %s lf@ab$retval\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval);
label_cnt++;
node->gvalue = (char *)tmp_retval;
}


void mul_expression(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	
	printf("createframe\n"

	"defvar tf@a\n"
	"defvar tf@b\n"
	
	
        "move tf@a "); // a
	write_constant(node->lptr);
	printf("move tf@b ");
	write_constant(node->rptr);
	printf("pushframe\n"
	"defvar lf@ab$retval\n"
	"defvar lf@a$type\n"
	"defvar lf@b$type\n"
	"\n"
	"type lf@a$type lf@a\n"
	"type lf@b$type lf@b\n"
	"\n"
	"jumpifeq ab$same_type%u lf@a$type lf@b$type\n"
	"jumpifeq a$is_int%u lf@a$type string@int\n"
	"jumpifeq b$is_int%u lf@b$type string@int\n"
	"exit int@4\n"
	"\n"
	"# pretypovani na float\n"
	"label a$is_int%u\n"
	"jumpifeq b$convert_to_float%u lf@b$type string@float\n"
	"exit int@4\n"
	"\n"
	"label b$convert_to_float%u\n"
	"int2float lf@a lf@a\n"
	"#jump ab$same_type\n"
	"jump ab$mul%u\n"
	"\n"
	"label b$is_int%u\n"
	"jumpifeq a$convert_to_float%u lf@a$type string@float\n"
	"exit int@4\n"
	"\n"
	"label a$convert_to_float%u\n"
	"int2float lf@b lf@b\n"
	"#jump ab$same_type\n"
	"jump ab$mul%u\n"
	"\n"
	"# spravne minus pokud jsou oba stejne\n"
	"label ab$same_type%u\n"
	"jumpifeq ab$mul%u lf@a$type string@int\n"
	"jumpifeq ab$mul%u lf@a$type string@float\n"
	"# semanticka chyba nekompatibility typu\n"
	"exit int@4\n"
	"\n"
	"# nasobeni pro float\n"
	"label ab$mul%u\n"
	"mul lf@ab$retval lf@a lf@b\n"
	"#jump ab$end\n"
	"\n"
	"# konec a ulozeni do retval\n"
	"label ab$end%u\n"
	"move %s lf@ab$retval\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval);
label_cnt++;
node->gvalue = (char *)tmp_retval;
}

void sub_expression(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	printf("createframe\n"
	
	"defvar tf@a\n"
	"defvar tf@b\n"
	
        "move tf@a "); // a
        write_constant(node->lptr);
        printf("move tf@b ");
        write_constant(node->rptr);

        printf("pushframe\n"
	"defvar lf@ab$retval\n"
	"defvar lf@a$type\n"
	"defvar lf@b$type\n"
	"\n"
	"type lf@a$type lf@a\n"
	"type lf@b$type lf@b\n"
	"\n"
	"jumpifeq ab$same_type%u lf@a$type lf@b$type\n"
	"jumpifeq a$is_int%u lf@a$type string@int\n"
	"jumpifeq b$is_int%u lf@b$type string@int\n"
	"exit int@4\n"
	"\n"
	"# pretypovani na float\n"
	"label a$is_int%u\n"
	"jumpifeq b$convert_to_float%u lf@b$type string@float\n"
	"exit int@4\n"
	"\n"
	"label b$convert_to_float%u\n"
	"int2float lf@a lf@a\n"
	"#jump ab$same_type\n"
	"jump ab$sub%u\n"
	"\n"
	"label b$is_int%u\n"
	"jumpifeq a$convert_to_float%u lf@a$type string@float\n"
	"exit int@4\n"
	"\n"
	"label a$convert_to_float%u\n"
	"int2float lf@b lf@b\n"
	"jump ab$sub%u\n"
	"\n"
	"# spravne minus pokud jsou oba stejne\n"
	"label ab$same_type%u\n"
	"jumpifeq ab$sub%u lf@a$type string@int\n"
	"jumpifeq ab$sub%u lf@a$type string@float\n"
	"# semanticka chyba nekompatibility typu\n"
	"exit int@4\n"
	"\n"
	"# odecteni pro int nebo float\n"
	"label ab$sub%u\n"
	"sub lf@ab$retval lf@a lf@b\n"
	"jump ab$end%u\n"
	"\n"
	"# konec a ulozeni do retval\n"
	"label ab$end%u\n"
	"move %s lf@ab$retval\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval);
	label_cnt++;
	node->gvalue = (char *)tmp_retval;
}
	
void add_expression(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	printf("createframe\n"
	
	"defvar tf@a\n"
	"defvar tf@b\n"
	
	"\n"
        "move tf@a "); // a
	write_constant(node->lptr);
	printf("move tf@b ");
	write_constant(node->rptr);
	printf("pushframe\n"
	"defvar lf@ab$retval\n"
	"defvar lf@a$type\n"
	"defvar lf@b$type\n"
	"\n"
	"type lf@a$type lf@a\n"
	"type lf@b$type lf@b\n"
	"\n"
	"jumpifeq ab$same_type%u lf@a$type lf@b$type\n"
	"jumpifeq a$is_int%u lf@a$type string@int\n"
	"jumpifeq b$is_int%u lf@b$type string@int\n"
	"exit int@4\n"
	"\n"
	"# pretypovani na float\n"
	"label a$is_int%u\n"
	"	jumpifeq b$convert_to_float%u lf@b$type string@float\n"
	"	exit int@4\n"
	"\n"
	"	label b$convert_to_float%u\n"
	"		int2float lf@a lf@a\n"
	"		jump ab$add%u\n"
	"\n"
	"label b$is_int%u\n"
	"	jumpifeq a$convert_to_float%u lf@a$type string@float\n"
	"	exit int@4\n"
	"\n"
	"	label a$convert_to_float%u\n"
	"		int2float lf@b lf@b\n"
	"		#jump ab$same_type\n"
	"		jump ab$add%u\n"
	"\n"
	"\n"
	"# spravne secteni pokud jsou oba stejne\n"
	"label ab$same_type%u\n"
	"	jumpifeq ab$con%u lf@a$type string@string\n"
	"	jumpifeq ab$add%u lf@a$type string@int\n"
	"	jumpifeq ab$add%u lf@a$type string@float\n"
	"	# semanticka chyba nekompatibility typu\n"
	"	exit int@4\n"
	"\n"
	"# secte stringu\n"
	"label ab$con%u\n"
	"	concat lf@ab$retval lf@a lf@b\n"
	"	jump ab$end%u\n"
	"\n"
	"# secteni pro int nebo float\n"
	"label ab$add%u\n"
	"	add lf@ab$retval lf@a lf@b\n"
	"	jump ab$end%u\n"
	"\n"
	"# konec a ulozeni do retval\n"
	"label ab$end%u\n"
	"move %s lf@ab$retval\n"
	"popframe\n"
	"\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval);
label_cnt++;
node->gvalue = (char *)tmp_retval;
}

void process_leq(ast_node_t *root)
{
	printf("createframe\n"

        "defvar tf@a\n"
        "defvar tf@b\n"
        "move tf@a " );
        write_constant(root->lptr);
        printf("move tf@b ");
        write_constant(root->rptr);

        printf("pushframe\n"
        "defvar lf@ret$val\n"
        "defvar lf@greater$bool\n"
        "type lf@ret$val lf@a\n"
        "jumpifeq a_is_int%u lf@ret$val string@int\n"
        "jumpifeq a_is_float%u lf@ret$val string@float\n"
        "jumpifeq a_is_string%u lf@ret$val string@string\n"
        "label error_exit_int_4%u\n"
        "exit int@4\n"

        "label a_is_string%u\n"
        "type lf@ret$val lf@b\n"
        "jumpifeq a_b_same_type%u lf@ret$val string@string\n"
        "jump error_exit_int_4%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

        printf("label a_is_int%u\n"
        "type lf@ret$val lf@b\n"
        "jumpifeq a_b_same_type%u lf@ret$val string@int\n"
        "jumpifeq b_is_float%u lf@ret$val string@float\n"
        "jump error_exit_int_4\n"
        " \n"
        "label b_is_float%u\n"
        "int2float lf@a lf@a\n"
        "jump a_b_same_type%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

        printf("label a_is_float%u\n"
        "type lf@ret$val lf@b\n"
        "jumpifeq b_is_int%u lf@ret$val string@int\n"
        "jumpifeq a_b_same_type%u lf@ret$val string@float\n"
        "jump error_exit_int_4%u\n"
        " \n"
        "label b_is_int%u\n"
        "int2float lf@b lf@b\n"
        "jump a_b_same_type%u\n"
        " \n"
        "label a_b_same_type%u\n"
        "eq lf@ret$val lf@a lf@b\n"
        "lt lf@greater$bool lf@a lf@b\n"
        "or lf@ret$val lf@ret$val lf@greater$bool\n"
        " \n"
        "move %s lf@ret$val\n"
        "popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
        label_cnt++;
}

void process_geq(ast_node_t *root)
{

	printf("createframe\n"

        "defvar tf@a\n"
        "defvar tf@b\n"
        "move tf@a " );
	write_constant(root->lptr);
	printf("move tf@b ");
	write_constant(root->rptr);

	printf("pushframe\n"
	"defvar lf@ret$val\n"
	"defvar lf@greater$bool\n"
	"type lf@ret$val lf@a\n"
	"jumpifeq a_is_int%u lf@ret$val string@int\n"
	"jumpifeq a_is_float%u lf@ret$val string@float\n"
	"jumpifeq a_is_string%u lf@ret$val string@string\n"
	"label error_exit_int_4%u\n"
	"exit int@4\n"

	"label a_is_string%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@string\n"
	"jump error_exit_int_4%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);
	
	printf("label a_is_int%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@int\n"
	"jumpifeq b_is_float%u lf@ret$val string@float\n"
	"jump error_exit_int_4\n"
	" \n"
	"label b_is_float%u\n"
	"int2float lf@a lf@a\n"
	"jump a_b_same_type%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label a_is_float%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq b_is_int%u lf@ret$val string@int\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@float\n"
	"jump error_exit_int_4%u\n"
	" \n"
	"label b_is_int%u\n"
	"int2float lf@b lf@b\n"
	"jump a_b_same_type%u\n"
	" \n"
	"label a_b_same_type%u\n"
	"eq lf@ret$val lf@a lf@b\n"
	"gt lf@greater$bool lf@a lf@b\n"
	"or lf@ret$val lf@ret$val lf@greater$bool\n"
	" \n"
	"move %s lf@ret$val\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
	label_cnt++;
}

void process_neq(ast_node_t *root)
{

	printf("createframe\n"

	"defvar tf@a\n"
	"defvar tf@b\n"
	
	"move tf@a " );
        write_constant(root->lptr);
	printf("move tf@b ");
        write_constant(root->rptr);
        printf("pushframe\n"

	"defvar lf@ret$val\n"
	"# overeni typu pro A\n"
	"type lf@ret$val lf@a\n"
	"jumpifeq a_is_string%u lf@ret$val string@string\n"
	"jumpifeq a_is_int%u lf@ret$val string@int\n"
	"jumpifeq a_is_float%u lf@ret$val string@float\n"
	"jumpifeq a_is_nil%u lf@ret$val string@nil\n", label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label a_is_string%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@string\n"
	"jump great_true%u\n"
	" \n"
	"label a_b_same_type%u\n"
	"eq lf@ret$val lf@a lf@b\n"
	"not lf@ret$val lf@ret$val\n"
	"jump great_end%u\n"
	" \n"
	"label a_is_int%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@int\n"
	"jumpifeq b_is_float%u lf@ret$val string@float\n"
	"jump great_true%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label b_is_float%u\n"
	"int2float lf@a lf@a\n"
	"jump a_b_same_type%u\n"
	" \n"
	"label a_is_float%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@float\n"
	"jumpifeq convert_b_to_float%u lf@ret$val string@int\n"
	"jump great_true%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label convert_b_to_float%u\n"
	"int2float lf@b lf@b\n"
	"jump a_b_same_type%u\n"
	" \n"
	"label a_is_nil%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_b_same_type%u lf@ret$val string@nil\n"
	" \n"
	"label great_true%u\n"
	"move lf@ret$val bool@true\n"
	" \n"
	"# label ktery nam zarucuje spravny odskok\n"
	"label great_end%u\n"
	" \n"
	"move %s lf@ret$val\n"
	"popframe\n",  label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
	label_cnt++;


}


void process_eq(ast_node_t *root)
{

	printf("createframe\n"
	

	"# a == b typ\n"

	"defvar tf@a\n"
	"defvar tf@b\n"

	"move tf@a "); // a
        write_constant(root->lptr);
        printf("move tf@b "); // b
        write_constant(root->rptr);

        printf("pushframe\n"
	"defvar lf@ret$val\n"
	"# overeni typu pro A\n"
	"type lf@ret$val lf@a\n"
	"jumpifeq a_is_string%u lf@ret$val string@string\n"
	"jumpifeq a_is_int%u lf@ret$val string@int\n"
	"jumpifeq a_is_float%u lf@ret$val string@float\n"
	"jumpifeq a_is_nil%u lf@ret$val string@nil\n"


	"label a_is_string%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq b_is_string%u lf@ret$val string@string\n"
	"jump great_false%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label b_is_string%u\n"
	"eq lf@ret$val lf@a lf@b\n"
	"jumpifeq great_end%u lf@ret$val bool@true\n"
	"jump great_false%u\n"
	" \n"
	"label a_is_int%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq b_is_float%u lf@ret$val string@float\n"
	"jumpifeq b_is_int%u lf@ret$val string@int\n"
	"jump great_false%u\n"
	" \n"
	"label b_is_float%u\n"
	"int2float lf@a lf@a\n"
	"jump a_and_b_are_float%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);
	
	printf("label b_is_int%u\n"
	"eq lf@ret$val lf@a lf@b\n"
	"jump great_end%u\n"
	" \n"
	"label a_is_float%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_and_b_are_float%u lf@ret$val string@float\n"
	"jumpifeq a_is_float_b_is_int%u lf@ret$val string@int\n"
	"jump great_false%u\n"
	" \n"
	"label a_and_b_are_float%u\n"
	"eq lf@ret$val lf@a lf@b\n"
	"jump great_end%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);
	
	printf("label a_is_float_b_is_int%u\n"
	"int2float lf@b lf@b\n"
	"jump a_and_b_are_float%u\n"
	" \n"
	"label a_and_b_are_nil%u\n"
	"eq lf@ret$val lf@a lf@b\n"
	"jump great_end%u\n"
	" \n"
	"label a_is_nil%u\n"
	"type lf@ret$val lf@b\n"
	"jumpifeq a_and_b_are_nil%u lf@ret$val string@nil\n"
	" \n"
	"label great_false%u\n"
	"move lf@ret$val bool@false\n"
	" \n"
	"# label ktery nam zarucuje spravny odskok\n"
	"label great_end%u\n"
	" \n"
	"move %s lf@ret$val\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
	label_cnt++;
}

void process_lt(ast_node_t *root)
{

        printf("createframe\n"
        "defvar tf@a\n"
        "defvar tf@b\n"
        "move tf@a "); // a
        write_constant(root->lptr);
        printf("move tf@b "); // b
        write_constant(root->rptr);

        printf("pushframe\n"
        "defvar lf@a$typ\n"
        "defvar lf@b$typ\n"
        "type lf@a$typ lf@a\n"
        "jumpifeq a_is_string%u lf@a$typ string@string\n"
        "jumpifeq a_is_int%u lf@a$typ string@int\n"
        "jumpifeq a_is_float%u lf@a$typ string@float\n"
        "exit int@4\n"

        "label a_is_string%u\n"
        "type lf@b$typ lf@b\n"
        "jumpifeq b_is_string%u lf@b$typ string@string\n"
        "exit int@4\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

        printf("label b_is_string%u\n"
        "lt lf@a$typ lf@a lf@b\n"
        "jump great_end%u\n"

        "label a_is_int%u\n"
        "type lf@b$typ lf@b\n"
        "jumpifeq b_is_int%u lf@b$typ string@int\n"
        "jumpifeq b_is_float%u lf@b$typ string@float\n"
        "exit int@4\n"

        "# a i b jsou int\n"
        "label b_is_int%u\n"
        "lt lf@a$typ lf@a lf@b\n"
        "jump great_end%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);


        printf("label b_is_float%u\n"
        "int2float lf@a lf@a\n"
        "lt lf@a$typ lf@a lf@b\n"
        "jump great_end%u\n"

        "label a_is_float%u\n"
        "type lf@b$typ lf@b\n"
        "jumpifeq a_is_float_but_b_is_int%u lf@b$typ string@int\n"
        "jumpifeq both_operands_are_float%u lf@b$typ string@float\n"
        "exit int@4\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

        printf("label both_operands_are_float%u\n"
        "lt lf@a$typ lf@a lf@b\n"
        "jump great_end%u\n"

        "# a je float ale b je int, tudiz pretypovat\n"
        "label a_is_float_but_b_is_int%u\n"
        "int2float lf@b lf@b\n"
        "lt lf@a$typ lf@a lf@b\n"

        "# label ktery nam zarucuje spravny odskok\n"
        "label great_end%u\n"
        "move %s lf@a$typ\n"
        "popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
        label_cnt++;
}

void process_gt(ast_node_t *root)
{
	
	printf("createframe\n"
	"defvar tf@a\n"
	"defvar tf@b\n"
	"move tf@a "); // a
	write_constant(root->lptr);
	printf("move tf@b "); // b
	write_constant(root->rptr);
	
	printf("pushframe\n"
	"defvar lf@a$typ\n"
	"defvar lf@b$typ\n"
	"type lf@a$typ lf@a\n"
	"jumpifeq a_is_string%u lf@a$typ string@string\n"
	"jumpifeq a_is_int%u lf@a$typ string@int\n"
	"jumpifeq a_is_float%u lf@a$typ string@float\n"
	"exit int@4\n"

	"label a_is_string%u\n"
	"type lf@b$typ lf@b\n"
	"jumpifeq b_is_string%u lf@b$typ string@string\n"
	"exit int@4\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label b_is_string%u\n"
	"gt lf@a$typ lf@a lf@b\n"
	"jump great_end%u\n"

	"label a_is_int%u\n"
	"type lf@b$typ lf@b\n"
	"jumpifeq b_is_int%u lf@b$typ string@int\n"
	"jumpifeq b_is_float%u lf@b$typ string@float\n"
	"exit int@4\n"

	"# a i b jsou int\n"
	"label b_is_int%u\n"
	"gt lf@a$typ lf@a lf@b\n"
	"jump great_end%u\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);


	printf("label b_is_float%u\n"
	"int2float lf@a lf@a\n"
	"gt lf@a$typ lf@a lf@b\n"
	"jump great_end%u\n"

	"label a_is_float%u\n"
	"type lf@b$typ lf@b\n"
	"jumpifeq a_is_float_but_b_is_int%u lf@b$typ string@int\n"
	"jumpifeq both_operands_are_float%u lf@b$typ string@float\n"
	"exit int@4\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label both_operands_are_float%u\n"
	"gt lf@a$typ lf@a lf@b\n"
	"jump great_end%u\n"

	"# a je float ale b je int, tudiz pretypovat\n"
	"label a_is_float_but_b_is_int%u\n"
	"int2float lf@b lf@b\n"
	"gt lf@a$typ lf@a lf@b\n"

	"# label ktery nam zarucuje spravny odskok\n"
	"label great_end%u\n"
	"move %s lf@a$typ\n"

	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
	label_cnt++;
}

void process_bool(ast_node_t *root)
{
	switch (root->opcode)
	{
		case o_eq:
			process_eq(root);
			break;
		case o_neq:	
			process_neq(root);
			break;
		case o_gt:
			process_gt(root);
			break;
		case o_lt:
			process_lt(root);
			break;
		case o_geq:
			process_geq(root);
			break;
		case o_leq:
			process_leq(root);
			break;
		case o_var:
			process_expr_var(root);
			
			break;
	}
}


// ve vyrazu pouze 1 operand - promenna
void process_expr_var(ast_node_t *root)
{

	printf("createframe\n"
	"defvar tf@a\n"
	"move tf@a ");
	write_constant(root);

	printf("pushframe\n"
	"defvar lf@ret$val\n"

	"type lf@ret$val lf@a\n"
	"jumpifeq nil_false%u lf@ret$val string@nil\n"
	"move lf@ret$val bool@true\n"
	"jump end%u\n"

	"label nil_false%u\n"
	"move lf@ret$val bool@false\n"

	"label end%u\n"

	"move %s lf@ret$val\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, tmp_bool);
	label_cnt++;
}

void process_if_stmt(ast_node_t *node)
{
	unsigned local_label_cnt = label_cnt;
	label_cnt++;
	ast_node_t *expression_root = node->lptr;
	// zpracovani leve casti vyrazu
	process_expression(expression_root->lptr); // v gvalue je nyni return var s numerickym vysledkem	
	// zpracovani prave casti vyrazu
	process_expression(expression_root->rptr); // v gvalue je nyni return var s numerickym vysledkem
	process_bool(expression_root); // zpracovani vysledku vyrazu
	
	//zpracovani deklaraci v obou vetvich
	stmt_declare_vars(node);

	// true vetev
	printf("jumpifeq $if_stmt_false%u %s bool@false\n", local_label_cnt, tmp_bool);
	process(node->rptr->lptr);
	printf("jump $if_end%u\n", local_label_cnt);

	// false vetev
	printf("label $if_stmt_false%u\n", local_label_cnt);
	process(node->rptr->rptr);

	printf("label $if_end%u\n", local_label_cnt);

}


// je potreba predem deklarovat vsechny variables v tele cyklu, jinak by opakovanym prochazenim cyklu dochazelo k redeklaraci variables
void stmt_declare_vars(ast_node_t *node)
{
	if (node == NULL)
		return;

	if (node->opcode == o_var_declare)
	{

		process_var(node, LF);
		printf("defvar %s\n", (char *)node->gvalue);
		printf("move %s nil@nil\n", (char *)node->gvalue);
		node->opcode = o_var;
	}
	stmt_declare_vars(node->lptr);
	stmt_declare_vars(node->rptr);
}

void process_while_stmt(ast_node_t *node)
{
	unsigned local_label_cnt = label_cnt;
	label_cnt++;
	
	//zpracovani deklaraci
	stmt_declare_vars(node->rptr);

	printf("label $while_stmt%u\n", local_label_cnt);

	ast_node_t *expression_root = node->lptr;
        // zpracovani leve casti vyrazu
        process_expression(expression_root->lptr); // v gvalue je nyni return var s numerickym vysledkem
        // zpracovani prave casti vyrazu
        process_expression(expression_root->rptr); // v gvalue je nyni return var s numerickym vysledkem
	process_bool(expression_root); // zpracovani vysledku vyrazu

	// pokud false, skocit na $while_end
	printf("jumpifeq $while_end%u %s bool@false\n", local_label_cnt, tmp_bool);

	process(node->rptr); // telo cyklu

	printf("jump $while_stmt%u\n", local_label_cnt); // skok na zacatek pro nove vyhodnoceni vyrazu
	printf("label $while_end%u\n", local_label_cnt); // navesti pro ukonceni cyklu
}


//funkce length
void process_strlen(ast_node_t *node)
{
	ast_node_t *param = node->lptr->rptr;
	if (param->t_type == t_string)
		process_constant(param);
	else
		process_var(param, LF);	
	char *tmp_retval = unique_return_id();
	printf("createframe\n"
	"defvar tf@a\n"
	"move tf@a "); 
	write_constant(param); //predani parametru
	printf("pushframe\n"


	"defvar lf@$a_type\n"// typ promenne a
	"type lf@$a_type lf@a\n"
	"jumpifeq $is_string%u lf@$a_type string@string\n"
	"exit int@4\n"// parametr neni string
	
	"label $is_string%u\n"
	"strlen %s lf@a\n"// ulozeni delky retezce 'a' do return promenne
	"popframe\n",
	label_cnt, label_cnt, tmp_retval);
	label_cnt++;
	node->gvalue = tmp_retval; // ulozeni vysledku do gvalue uzlu
}

void process_inputs(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	printf("read %s string\n", tmp_retval);
	node->gvalue = tmp_retval;
}

void process_inputi(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	printf("read %s int\n", tmp_retval);
	node->gvalue = tmp_retval;
}

void process_inputf(ast_node_t *node)
{
        char *tmp_retval = unique_return_id();
        printf("read %s float\n", tmp_retval);
        node->gvalue = tmp_retval;
}

void process_ord(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();

	//parametry
	ast_node_t *param = node->lptr->rptr;


	printf("createframe\n"
	"defvar tf@$str\n"//string
	"defvar tf@$idx\n"//index
	"move tf@$str ");
	write_constant(param); // zapis stringu
	printf("move tf@$idx ");
	write_constant(param->rptr); // zapis indexu
	printf("pushframe\n"
	"defvar lf@$str_type\n"
	"defvar lf@$idx_type\n");
	printf("type lf@$str_type lf@$str\n"
	"jumpifeq str$is_string%u lf@$str_type string@string\n"
	"exit int@4\n"

	"# idx muze byt i float, tudiz je potreba jej pretypovat na int\n"
	"label str$is_string%u\n"
	"type lf@$idx_type lf@$idx\n"
	"jumpifeq idx$is_int%u lf@$idx_type string@int\n"
	"jumpifeq idx$convert_to_int%u lf@$idx_type string@float\n"
	"exit int@4\n"
	"\n"

	"label idx$convert_to_int%u\n"
	"float2int lf@$idx lf@$idx\n"
	"\n"

	"# overeni zda je idx ve spravnem rozsahu\n"
	"label idx$is_int%u\n"
	"# test zda je vetsi jak 0\n"
	"defvar lf@$idx$range\n"
	"defvar lf@$str$length\n"
	"\n"

	"# overeni zda je vetsi jak 0\n"
	"strlen lf@$str$length lf@$str\n"
	"lt lf@$idx$range lf@$idx int@0\n"
	"jumpifeq ord$return_nil%u lf@$idx$range bool@true\n"
	"\n"

	"# overeni zda je mensi nez length(str) - 1\n"
	"sub lf@$str$length lf@$str$length int@1\n"
	"gt lf@$idx$range lf@$idx lf@$str$length\n"
	"jumpifneq ord$return_nil%u lf@$idx$range bool@false\n"
	"\n"
	"stri2int %s lf@$str lf@$idx\n"
	"jump ord$end%u\n"
	"\n"
	"# vraceni nil v pripade spatnych indexu\n"
	"label ord$return_nil%u\n"
	"move %s nil@nil\n"
	"\n"
	"label ord$end%u\n"
	"\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval, label_cnt, label_cnt, tmp_retval,  label_cnt);
	label_cnt++;
	node->gvalue = tmp_retval;
}

void process_chr(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	// parametr
	ast_node_t *param = node->lptr->rptr;
	
	printf("createframe\n"
        "defvar tf@a\n"

        "move tf@a ");
	write_constant(param);
	printf("pushframe\n"
	"defvar lf@a$type\n"
        "type lf@a$type lf@a\n"
        "jumpifeq a$is_int%u lf@a$type string@int\n"
        "jumpifeq a$convert_to_int%u lf@a$type string@float\n"
        "label chr$end%u\n"
        "exit int@4\n"

        "label a$convert_to_int%u\n"
        "float2int lf@a lf@a\n"
        "# overeni zda je cislo v rozmezi 0-255\n"
        "label a$is_int%u\n"
        "defvar lf@a$comparison_chr\n"
        "lt lf@a$comparison_chr lf@a int@0\n"
        "jumpifneq chr$end%u lf@a$comparison_chr bool@false\n"

        "#defvar a$is_greater_255\n"
        "gt lf@a$comparison_chr lf@a int@255\n"
        "jumpifneq chr$end%u lf@a$comparison_chr bool@false\n"
        "int2char %s lf@a\n"
        "popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval);
	label_cnt++;
	node->gvalue = tmp_retval;
}

void process_substr(ast_node_t *node)
{
	char *tmp_retval = unique_return_id();
	ast_node_t *param = node->lptr->rptr;
	
	printf("createframe\n"

	"defvar tf@str\n"
	"defvar tf@idx$start\n"
	"defvar tf@idx$end\n");

	printf("move tf@str "); // string
	write_constant(param);
	printf("move tf@idx$start "); // start idx
	write_constant(param->rptr);
	printf("move tf@idx$end "); // pocet znaku ke zpracovani
	write_constant(param->rptr->rptr);
	
	printf("pushframe\n"
	"defvar lf@substr$retval\n"
	"defvar lf@str$type\n"
	"defvar lf@idx$start$type\n"
	"defvar lf@idx$end$type\n"
	"\n"
	"# overeni zda se jedna o string\n"
	"type lf@str$type lf@str\n"
	"jumpifeq substr$is$string%u lf@str$type string@string\n"
	"exit int@4\n"
	"\n"
	"# overeni zda idx start je int/float\n"
	"label substr$is$string%u\n"
	"type lf@idx$start$type lf@idx$start\n"
	"jumpifeq idx$start$is$int%u lf@idx$start$type string@int\n"
	"jumpifeq idx$start$is$float%u lf@idx$start$type string@float\n"
	"exit int@4\n", label_cnt, label_cnt, label_cnt, label_cnt);

	printf("# overeni zda idx end je int/float\n"
	"label idx$start$is$int%u\n"
	"type lf@idx$end$type lf@idx$end\n"
	"jumpifeq idx$end$is$int%u lf@idx$end$type string@int\n"
	"jumpifeq idx$end$is$float%u lf@idx$end$type string@float\n"
	"exit int@4\n"
	"\n"
	"# prevod idx start na int\n"
	"label idx$start$is$float%u\n"
	"float2int lf@idx$start lf@idx$start\n"
	"jump idx$start$is$int%u\n"
	"\n"
	"# prevod idx end na int\n"
	"label idx$end$is$float%u\n"
	"float2int lf@idx$end lf@idx$end\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt);

	printf("label idx$end$is$int%u\n"
	"# overeni zda je index vetsi jak 0\n"
	"defvar lf@idx$range\n"
	"lt lf@idx$range lf@idx$start int@0\n"
	"jumpifeq substr$return$nil%u lf@idx$range bool@true\n"
	"\n"
	"# overeni zda index nepresahuje length(str)\n"
	"#defvar lf@idx$in$length\n"
	"strlen lf@idx$range lf@str\n"
	"gt lf@idx$range lf@idx$start lf@idx$range # potencionalni chyba\n"
	"jumpifeq substr$return$nil%u lf@idx$range bool@true\n", label_cnt, label_cnt, label_cnt);

	printf("# overeni zda n < 0\n"
	"lt lf@idx$range lf@idx$end int@0\n"
	"jumpifeq substr$return$nil%u lf@idx$range bool@true\n"
	"# udelani prazdneho stringu pro konkatenaci znaku\n"
	"move lf@substr$retval string@\n"
	"strlen lf@idx$range lf@str\n"
	"# strlen - i\n"
	"sub lf@idx$range lf@idx$range lf@idx$start\n"
	"# overeni zda n je korektni nebo ne\n"
	"defvar lf@idx$or$n\n"
	"gt lf@idx$or$n lf@idx$range lf@idx$end\n"
	"jumpifeq n$is$correct%u lf@idx$or$n bool@true\n"
	"move lf@idx$end lf@idx$range\n", label_cnt, label_cnt);
	
	printf("label n$is$correct%u\n"
	"defvar lf@tmp$char\n"
	"defvar lf@loop$end\n"
	"move lf@idx$range lf@idx$end\n"
	"\n"
	"# samotny substr algoritmus, kde bereme pozadovany znak\n"
	"# spojujeme a ptame se zda mame uz hodnotu n v loop$range\n"
	"label loop$for$substr%u\n"
	"eq lf@loop$end lf@idx$range int@0\n"
	"jumpifeq substr$return%u lf@loop$end bool@true\n"
	"getchar lf@tmp$char lf@str lf@idx$start\n"
	"concat lf@substr$retval lf@substr$retval lf@tmp$char\n"
	"add lf@idx$start lf@idx$start int@1\n"
	"sub lf@idx$range lf@idx$range int@1\n"
	"jump loop$for$substr%u\n"
	"\n"
	"label substr$return$nil%u\n"
	"move lf@substr$retval nil@nil\n"
	"\n"
	"label substr$return%u\n"
	"move %s lf@substr$retval\n"
	"\n"
	"popframe\n", label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, label_cnt, tmp_retval);
	label_cnt++;
	node->gvalue = tmp_retval;
}

void process_params_define(ast_node_t *node)
{
	if (node == NULL)
		return;
	process_params_define(node->rptr);
	process_var(node, TF);
	printf("defvar %s\n", (char *)node->gvalue);
	printf("POPS %s\n", (char *)node->gvalue);
}

void process_fnc_define(ast_node_t *node)
{
	printf("label $%s\n", (char *)node->value);
       
        ast_node_t *param = node->lptr->rptr; // prvni parametr
	process_params_define(param);
	printf("pushframe\n");
	process(node->rptr); // telo funkce
	printf("popframe\n"
	"return\n");

}

void process_fnc_call(ast_node_t *node)
{
	printf("createframe\n");
	ast_node_t *param = node->lptr->rptr;
	while (param != NULL)
	{
		if (param->t_type == t_id)
		{
			process_var(param, LF);
			printf("PUSHS %s\n", (char *)param->gvalue);
		}
		else
		{
			printf("PUSHS ");
			write_constant(param);
		}
		param = param->rptr;
	}
	printf("call $%s\n", (char *)node->value);
	node->gvalue = (char *)global_retval;
}



//TODO doresit retvalues
void process(ast_node_t *node)
{
	if (node == NULL)	
		return;
//	printf("processing opcode: %s\n", opcode_str[node->opcode]);
	switch (node->opcode)
	{
		case o_var_assign:
			process(node->lptr);
			process(node->rptr);	
			printf("move %s ", (char *)node->lptr->gvalue);
			write_constant(node->rptr); // konstanta nebo id
			printf("move %s %s\n", global_retval, (char *)node->lptr->gvalue); // retval
			break;
		case o_var_declare:
			process_var(node, LF);
			printf("defvar %s\n", (char *)node->gvalue);
			printf("move %s nil@nil\n", (char *)node->gvalue); // implicitni init na nil
			printf("move %s %s\n", global_retval, (char *)node->gvalue); // retval
			break;
		case o_constant:
			process_constant(node);	
			printf("move %s ", global_retval);
			write_constant(node);
			break;
		case o_program:
			printf(".IFJcode18\n");
			define_globals(node); // definice vsech potrebnych globalnich promennych
			printf("jump $main\n\n");	
			process(node->lptr); // definice funkci
			printf("\nlabel $main\n");
			printf("createframe\npushframe\n");

			process(node->rptr); // main
			printf("popframe\n");
			printf("exit int@0\n");
			break;
		case o_if_cond:
			process_if_stmt(node);			
			break;
		case o_while:
			process_while_stmt(node);
			break;
		case o_statement_list:
		case o_statement:

			process(node->lptr);			
			process(node->rptr);
			break;	
		case o_var:
			process_var(node, LF);
			printf("move %s %s\n", global_retval, (char *)node->gvalue); // retval
			break;
		case o_function_call:
			process_fnc_call(node);
			break;
		case o_function_define:
			process_fnc_define(node);
			break;
		case o_strlen:
			process_strlen(node);
			break;
		case o_sum:	
		case o_sub:
		case o_mul:
		case o_div:
			process_expression(node);
			printf("move %s %s\n", global_retval, (char *)node->gvalue); // retval
			break;
		case o_print:
			process_print(node);
			break;
		case o_inputs:
			process_inputs(node);
			break;
		case o_inputi:
			process_inputi(node);
			break;
		case o_inputf:
			process_inputf(node);
			break;
		case o_ord:
			process_ord(node);
			break;
		case o_chr:
			process_chr(node);
			break;
		case o_substr:
			process_substr(node);
			break;
		default:
			printf("Undefined opcode found: %s\n", opcode_str[node->opcode]);
			break;
	}


}



