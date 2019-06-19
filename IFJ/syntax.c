/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: syntax.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"
#include "syntax.h"
#include "symtable.h"
#include "list.h"
#include "my_malloc.h"

#include "semantic.h"


symtable_node_t *global_tabulka;
list_t *global_list;


void assign()//,unsigned int zanoren imozna toto pridat
{
	token_t t, u;
	t = get_next_token();
	list_insert_last(global_list, t);
	//prirazeni identifikatoru, ktery je bud volani funkce nebo vyraz
	if (t.type == tkn_identifier)
	{
		u = get_next_token();
		list_insert_last(global_list, u);
		if (u.type == tkn_new_line)//bud 1 promenna nebo funkce bez parametru
		{
			return;
		}
		else if (u.type == tkn_left_bracket)
			call_func(RBRACKET);
		else if (u.type == tkn_identifier || u.type == tkn_integer || u.type == tkn_key_nil || u.type == tkn_string || u.type == tkn_double)
			call_func(NOBRACKET);
		else if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash)
			expr_aritmetic(NOBRACKET);
		else
			syntax_error("wrong assingment");
	}
	//prirazeni volani built in funkce
	else if (t.type == tkn_key_inputf || t.type == tkn_key_inputi || t.type == tkn_key_inputs || t.type == tkn_key_print || t.type == tkn_key_length || t.type == tkn_key_substr || t.type == tkn_key_ord || t.type == tkn_key_chr)
	{
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_new_line)
			return;
		else if (t.type == tkn_left_bracket)
			call_func(RBRACKET);
		else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
			call_func(NOBRACKET);
		else
			syntax_error("Wrong call of built in function");
	}
	//prirazeni primo hodnoty
	else if (t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
	{
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
			expr_aritmetic(NOBRACKET);
		else if (t.type == tkn_new_line)
			return;
		else
			syntax_error("expected EOL or aritmetic expression");
	}
	//zacatek vyrazu hned zavorkou
	else if (t.type == tkn_left_bracket)
		epxr_left_bracket_checkEOL();
	else
		syntax_error("wrong assingment");

}

void expr_bool_left_bracket_check_ending(unsigned int if_or_while)
{
	token_t u;
	expr_bool(RBRACKET, if_or_while);
	u = get_next_token();
	list_insert_last(global_list, u);
	if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash || u.type == tkn_equal_2 || u.type == tkn_not_equal || u.type == tkn_less || u.type == tkn_less_or_equal || u.type == tkn_more || u.type == tkn_more_or_equal)
		expr_bool(NOBRACKET, if_or_while);
	else
	{
		switch (if_or_while)
		{
		case END_DO://while
			if (u.type != tkn_key_do)
				syntax_error("wrong bool expression, expected keyword 'do' or another operator");
			break;
		case END_THEN:
			if (u.type != tkn_key_then)
				syntax_error("wrong bool expression, expected keyword 'then' or another operator");
			break;
		}
		u = get_next_token();
		list_insert_last(global_list, u);
		if (u.type == tkn_new_line)
			return;
		else
			syntax_error("wrong bool expression, expected EOL after keyword then/do");
	}

}

void epxr_left_bracket_checkEOL()
{
	token_t u;
	expr_aritmetic(RBRACKET);
	u = get_next_token();
	list_insert_last(global_list, u);
	if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash)
		expr_aritmetic(NOBRACKET);
	else if (u.type == tkn_new_line)
		return;
	else
		syntax_error("wrong expression, expected operator or EOL");

}

void expr_bool(unsigned int type, unsigned int if_or_while)
{//same as expr but there can be logical operators
	token_t t, u;
	switch (type)
	{
	case NOBRACKET:
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_left_bracket)
			expr_bool_left_bracket_check_ending(if_or_while);
		else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
		{
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash || u.type == tkn_equal_2 || u.type == tkn_not_equal || u.type == tkn_less || u.type == tkn_less_or_equal || u.type == tkn_more || u.type == tkn_more_or_equal)
				expr_bool(NOBRACKET, if_or_while);
			else
			{
				switch (if_or_while)
				{
				case END_DO://while
					if (u.type != tkn_key_do)
						syntax_error("wrong bool expression, expected keyword 'do' or another operator");
					break;
				case END_THEN:
					if (u.type != tkn_key_then)
						syntax_error("wrong bool expression, expected keyword 'then'");
					break;
				}
				u = get_next_token();
				list_insert_last(global_list, u);
				if (u.type == tkn_new_line)
					return;
				else
					syntax_error("wrong bool expression, expected EOL after keyword then/do");
			}
		}
		else
			syntax_error("wrong bool expression, expected '(' or id/item");
		break;
	case RBRACKET:
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_left_bracket)
		{
			expr_bool(RBRACKET, if_or_while);
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type == tkn_right_bracket)
				return;
			else if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash || u.type == tkn_equal_2 || u.type == tkn_not_equal || u.type == tkn_less || u.type == tkn_less_or_equal || u.type == tkn_more || u.type == tkn_more_or_equal)
				expr_bool(RBRACKET, if_or_while);
			else
				syntax_error("wrong expression, expected ')'");
		}
		else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
		{
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash || u.type == tkn_equal_2 || u.type == tkn_not_equal || u.type == tkn_less || u.type == tkn_less_or_equal || u.type == tkn_more || u.type == tkn_more_or_equal)
				expr_bool(RBRACKET, if_or_while);
			else if (u.type == tkn_right_bracket)
				return;
			else
				syntax_error("wrong bool expression, expected ')' or operator");
		}
		else
			syntax_error("wrong bool expression, expected '(' or id/item");
		break;
	}
}




void expr_aritmetic(unsigned int type)
{
	token_t t, u;
	switch (type)
	{
	case NOBRACKET:
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_left_bracket)
			epxr_left_bracket_checkEOL();
		else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
		{
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash)
				expr_aritmetic(NOBRACKET);
			else if (u.type == tkn_new_line)
				return;
			else
				syntax_error("wrong expression, expected EOL or another operator (end of expression)");

		}
		else
			syntax_error("wrong expression, expected '(' or id/item");
		break;
	case RBRACKET:
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_left_bracket)
		{
			expr_aritmetic(RBRACKET);
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type == tkn_right_bracket)
				return;
			else if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash)
				expr_aritmetic(RBRACKET);
			else
				syntax_error("wrong expression, expected ')'");
		}
		else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
		{
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type == tkn_minus || u.type == tkn_plus || u.type == tkn_star || u.type == tkn_slash)
				expr_aritmetic(RBRACKET);
			else if (u.type == tkn_right_bracket)
				return;
			else
				syntax_error("wrong expression, expected ')' or operator");
		}
		else
			syntax_error("wrong expression, expected '(' or id/item");
		break;
	}
}


void while_came()
{
	token_t t;
	expr_bool(NOBRACKET, END_DO);//checkuje i EOL

	inside_commands(END_END);
	//EOL
	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type == tkn_end_of_file)
		syntax_error("unexpected eof");
	else if (t.type != tkn_new_line)
		syntax_error("expected EOL");
}


void if_came()
{
	token_t t;
	expr_bool(NOBRACKET, END_THEN);//checkuje i EOL

	inside_commands(END_ELSE);
	//EOL
	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type == tkn_end_of_file)
		syntax_error("unexpected eof");
	else if (t.type != tkn_new_line)
		syntax_error("expected EOL after else");
	inside_commands(END_END);
	//EOL
	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type == tkn_end_of_file)
		syntax_error("unexpected eof");
	else if (t.type != tkn_new_line)
		syntax_error("expected EOL after end (from if)");
}



void params_func(symtable_value_t param_cnt)
{//asi by melo byt rekurzivni ale slo to nahradit pomoci while, tak pro� ne 
	token_t t, u;

	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type == tkn_end_of_file)//mluvi samo za sebe... EOF se cpe tam kde byt nema
		syntax_error("unexpected eof");
	else if (t.type == tkn_right_bracket)//konec neboli prisla prava zavorka
	{
		u = get_next_token();
		list_insert_last(global_list, u);
		if (u.type != tkn_new_line)//za pravou zavorkou musi byt EOL
			syntax_error("expected EOL after ')'");
		return;
	}
	else if (t.type != tkn_identifier)//prvni parametr
		syntax_error("expected 1st parametr or ')'");

	(*param_cnt)++;//prvni parametr
	//dalsi parametry
	while (1)
	{
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_end_of_file)//mluvi samo za sebe... EOF se cpe tam kde byt nema
			syntax_error("unexpected eof");
		else if (t.type == tkn_right_bracket)//konec neboli prisla prava zavorka
		{
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type != tkn_new_line)//za pravou zavorkou musi byt EOL
				syntax_error("expected EOL after ')'");
			break;
		}

		u = get_next_token();
		list_insert_last(global_list, u);
		if (t.type == tkn_comma && u.type == tkn_identifier)//carka a pak to co muze byt parametrem fce
		{
			(*param_cnt)++;//dalsi parametry
			continue;
		}
		else
			syntax_error("expected comma and parametr");
	}
}


void params(unsigned int type)
{//asi by melo byt rekurzivni ale slo to nahradit pomoci while, tak pro� ne 
	token_t t, u;
	switch (type)
	{
	case RBRACKET:
		t = get_next_token();
		list_insert_last(global_list, t);
		if (t.type == tkn_end_of_file)//mluvi samo za sebe... EOF se cpe tam kde byt nema
			syntax_error("unexpected eof");
		else if (t.type == tkn_right_bracket)//konec neboli prisla prava zavorka
		{
			u = get_next_token();
			list_insert_last(global_list, u);
			if (u.type != tkn_new_line)//za pravou zavorkou musi byt EOL
				syntax_error("expected EOL after ')'");
			return;
		}
		else if (t.type != tkn_identifier && t.type != tkn_integer && t.type != tkn_key_nil && t.type != tkn_string && t.type != tkn_double)//prvni parametr
			syntax_error("expected 1st parametr or ')'");
		//dalsi parametry
		while (1)
		{
			t = get_next_token();
			list_insert_last(global_list, t);
			if (t.type == tkn_end_of_file)//mluvi samo za sebe... EOF se cpe tam kde byt nema
				syntax_error("unexpected eof");
			else if (t.type == tkn_right_bracket)//konec neboli prisla prava zavorka
			{
				u = get_next_token();
				list_insert_last(global_list, u);
				if (u.type != tkn_new_line)//za pravou zavorkou musi byt EOL
					syntax_error("expected EOL after ')'");
				break;
			}

			u = get_next_token();
			list_insert_last(global_list, u);
			if (t.type == tkn_comma && (u.type == tkn_identifier || u.type == tkn_integer || u.type == tkn_key_nil || u.type == tkn_string || u.type == tkn_double))//carka a pak to co muze byt parametrem fce
				continue;
			else
				syntax_error("expected comma and parametr");
		}
		break;
	case NOBRACKET:
		//prvni parametr uz vzala volajici funkce, takze zde osetrujeme pouze dalsi parametry
		while (1)
		{
			t = get_next_token();
			list_insert_last(global_list, t);
			if (t.type == tkn_end_of_file)
				syntax_error("unexpected eof");
			else if (t.type == tkn_new_line)
				break;

			u = get_next_token();
			list_insert_last(global_list, u);
			if (t.type == tkn_comma && (u.type == tkn_identifier || u.type == tkn_integer || u.type == tkn_key_nil || u.type == tkn_string || u.type == tkn_double))
				continue;
			else
				syntax_error("expected command and parametr or EOL");

		}
		break;
	}
}


void inside_commands(unsigned int type)
{
	token_t t;
	switch (type)
	{
	case END_END:
		do
		{
			t = get_next_token();
			list_insert_last(global_list, t);
			switch (t.type)
			{
			case tkn_identifier:
				id_came(t);
				break;
			case tkn_new_line:
				continue;
			case tkn_key_if:
				if_came();
				break;
			case tkn_key_while:
				while_came();
				break;
			case tkn_key_inputf:
			case tkn_key_inputi:
			case tkn_key_inputs:
			case tkn_key_print:
			case tkn_key_length:
			case tkn_key_substr:
			case tkn_key_ord:
			case tkn_key_chr:
				t = get_next_token();
				list_insert_last(global_list, t);
				if (t.type == tkn_end_of_file)
					syntax_error("unexpected EOF");
				else if (t.type == tkn_left_bracket)
					call_func(RBRACKET);
				else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
					call_func(NOBRACKET);
				else if (t.type == tkn_new_line)
					break;
				else
					syntax_error("wrong parametrs of builtin function");
				break;
			case tkn_double:
			case tkn_integer:
			case tkn_string:
			case tkn_key_nil:
				t = get_next_token();
				list_insert_last(global_list, t);
				if (t.type == tkn_end_of_file)
					syntax_error("unexpected EOF");
				else if (t.type == tkn_new_line)
					continue;
				else if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
					expr_aritmetic(NOBRACKET);
				else
					syntax_error("expected EOL or expression after item (inside commands)");
				break;
			case tkn_key_end:
				break;
			default:
				syntax_error("expected keyword 'end'");
				break;
			}

		} while (t.type != tkn_key_end);
		break;
	case END_ELSE:
		do
		{
			t = get_next_token();
			list_insert_last(global_list, t);
			switch (t.type)
			{
			case tkn_identifier:
				id_came(t);
				break;
			case tkn_new_line:
				continue;
			case tkn_key_if:
				if_came();
				break;
			case tkn_key_while:
				while_came();
				break;
			case tkn_key_inputf:
			case tkn_key_inputi:
			case tkn_key_inputs:
			case tkn_key_print:
			case tkn_key_length:
			case tkn_key_substr:
			case tkn_key_ord:
			case tkn_key_chr:
				t = get_next_token();
				list_insert_last(global_list, t);
				if (t.type == tkn_end_of_file)
					syntax_error("unexpected EOF");
				else if (t.type == tkn_left_bracket)
					call_func(RBRACKET);
				else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
					call_func(NOBRACKET);
				else if (t.type == tkn_new_line)
					break;
				else
					syntax_error("wrong parametrs of builtin function");
				break;
			case tkn_double:
			case tkn_integer:
			case tkn_string:
			case tkn_key_nil:
				t = get_next_token();
				list_insert_last(global_list, t);
				if (t.type == tkn_end_of_file)
					syntax_error("unexpected EOF");
				else if (t.type == tkn_new_line)
					continue;
				else if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
					expr_aritmetic(NOBRACKET);
				else
					syntax_error("expected EOL or expression after item (inside if)");
				break;
			case tkn_key_else:
				break;
			default:
				syntax_error("expected keyword 'else'");
				break;
			}

		} while (t.type != tkn_key_else);
		break;
	}
}

void def_func()
{
	token_t t;
	symtable_value_t nechcito = my_malloc(sizeof(int));
	*nechcito = 42;//random useless cislo co mi je k nicemu
	symtable_value_t param_cnt = my_malloc(sizeof(int));
	*param_cnt = 0;
	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type == tkn_end_of_file)
		syntax_error("unexpected eof");
	else if (t.type != tkn_identifier)
		syntax_error("expected name of func");
	if (symtable_search(global_tabulka, t.value, nechcito) == true)
		semantic_error("redefiniton of function", SEMANTIC_VAR_DEF);
	//else
	symtable_insert(&global_tabulka, t.value, param_cnt);

	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type != tkn_left_bracket)
		syntax_error("expected '('");
	//pokud 't.type' byla '('


	params_func(param_cnt);

	inside_commands(END_END);
	//EOL
	t = get_next_token();
	list_insert_last(global_list, t);
	if (t.type == tkn_end_of_file)
		syntax_error("unexpected eof");
	else if (t.type != tkn_new_line)
		syntax_error("expected EOL");

}


void call_func(unsigned int type)
{//nakonec sem tuto funkci delal asi zbytecne... mely se tu kontrolovat i dalsi casti ale byly presunuty jinam
	switch (type)
	{
	case RBRACKET:
		params(RBRACKET);
		break;
	case NOBRACKET:
		params(NOBRACKET);
		break;
	}
}


void id_came(token_t came)
{
	token_t t;
	t = get_next_token();

	list_insert_last(global_list, t);
	char *nazev;

	switch (t.type)
	{
	case tkn_left_bracket:
		call_func(RBRACKET);
		break;
	case tkn_identifier:
	case tkn_integer:
	case tkn_key_nil:
	case tkn_string:
	case tkn_double:
		call_func(NOBRACKET);
		break;
	case tkn_equal:

		nazev = my_malloc(sizeof(char)*(strlen(came.value) + 1));
		strcpy(nazev, came.value);
		if (nazev[strlen(came.value) - 1] == '?' || nazev[strlen(came.value) - 1] == '!')
			syntax_error("Cannot assign to identifier that can be used only for functions");
		assign();
		break;
	case tkn_plus:
	case tkn_minus:
	case tkn_star:
	case tkn_slash:
		expr_aritmetic(NOBRACKET);
		break;
	case tkn_new_line:
		//prislo jen tak ID
		break;
	default:
		syntax_error("id came, but next token looks wrong");
	}

}


void parse()
{
	token_t t;
	global_tabulka = symtable_init();//tabulka kde budou nazvy vsech fci
	global_list = init_list();

	do
	{
		t = get_next_token();
		list_insert_last(global_list, t);
		switch (t.type)
		{
		case tkn_key_def:
			def_func();
			break;
		case tkn_identifier:
			id_came(t);
			break;
		case tkn_left_bracket:
			epxr_left_bracket_checkEOL();
			break;
		case tkn_new_line:
			continue;
		case tkn_key_if:
			if_came();
			break;
		case tkn_key_while:
			while_came();
			break;
		case tkn_key_inputf:
		case tkn_key_inputi:
		case tkn_key_inputs:
		case tkn_key_print:
		case tkn_key_length:
		case tkn_key_substr:
		case tkn_key_ord:
		case tkn_key_chr:
			t = get_next_token();
			list_insert_last(global_list, t);
			if (t.type == tkn_end_of_file)
				syntax_error("unexpected EOF");
			else if (t.type == tkn_left_bracket)
				call_func(RBRACKET);
			else if (t.type == tkn_identifier || t.type == tkn_integer || t.type == tkn_key_nil || t.type == tkn_string || t.type == tkn_double)
				call_func(NOBRACKET);
			else if (t.type == tkn_new_line)
				break;
			else
				syntax_error("wrong parametr of built in function");
			break;
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			t = get_next_token();
			list_insert_last(global_list, t);
			if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
				expr_aritmetic(NOBRACKET);
			else if (t.type == tkn_new_line)
				continue;
			else
				syntax_error("unexpected token after item");
			break;
		case tkn_end_of_file:
			break;
		default:
			syntax_error("unexpected token");
			break;
		}

	} while ((t.type) != tkn_end_of_file);

	//syntaxe probehla v poradku ted je na rade semantika
	//v prubehu syntaxe sem poukladal do global table nazvy vsech definovanych funkci (ty se budou moct dat volat z jin�ch funkc�)

	semantic_check(global_tabulka, global_list);
}
