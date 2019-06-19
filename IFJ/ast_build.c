/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: ast_build.c
 *
 */
#include "semantic.h"
#include "symtable.h"
#include "error.h"
#include "list.h"
#include "syntax.h"
#include  "scanner.h"
#include "my_malloc.h"
#include "ast_build.h"
#include "expression.h"
#include "generate.h"

list_t * all_tokens;
symtable_node_t *global_tabulka;

ast_node_t *function_defs;

symtable_node_t *global_full_tabulka;

//#define DEBUG

#ifdef DEBUG
#define D 
#else
#define D for(;0;)
#endif

static int i = 0;

ast_node_t *ast_inside_commands_end(symtable_node_t **local_tabulka, unsigned int zanoreni);

ast_node_t *ast_inputf(symtable_node_t **tabulka) 
{
	token_t t;
	do {
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		}

	} while (t.type != tkn_new_line);
	ast_node_t *inputf = ast_node_create(o_inputf, t_null, NULL, NULL, NULL);
	return inputf;
	//all ok... ukazatel ted ukazuje na dalsi prikaz za inputf( az za EOL)

}
ast_node_t *ast_inputi(symtable_node_t **tabulka)
{
	token_t t;
	do {
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		}

	} while (t.type != tkn_new_line);
	ast_node_t *inputi = ast_node_create(o_inputi, t_null, NULL, NULL, NULL);
	return inputi;
	//all ok... ukazatel ted ukazuje na dalsi prikaz za inputi( az za EOL)

}
ast_node_t *ast_inputs(symtable_node_t **tabulka)
{
	token_t t;
	do {
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		}

	} while (t.type != tkn_new_line);
	ast_node_t *inputs = ast_node_create(o_inputs, t_null, NULL, NULL, NULL);
	return inputs;
	//all ok... ukazatel ted ukazuje na dalsi prikaz za inputs( az za EOL)

}
ast_node_t *ast_print(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	ast_node_t *paramlist = statement_list_init();	
	ast_node_t *param;
	*idc = 42;//totally useless cislo
	do {
		param = NULL;
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
		case tkn_comma:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			param = function_param_constant(t);
			break;
		case tkn_identifier:
			//zde se zjisti typ dane promenne a zavola se WRITE s tim typem co to vratilo
			param = function_param_identifier(t.value);
			break;
		}
		if (param != NULL)
		{
			statement_list_insert(param, paramlist);
		}
	} while (t.type != tkn_new_line);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za print( az za EOL)
	// vytvoreni print funkce
	ast_node_t *print = print_builtin(paramlist);
	return print;
}
ast_node_t *ast_length(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	ast_node_t *paramlist = statement_list_init();
        ast_node_t *param;
	int prm_cnt = 0;
	do {
		param = NULL;
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
		case tkn_comma:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		case tkn_string:
			param = function_param_constant(t);
			prm_cnt++;
			break;
		case tkn_identifier:
			prm_cnt++;
			param = function_param_identifier(t.value);
			//zde se zjisti jestli je string a zavola se strlen
			break;
		}
		if (param != NULL)
		{
			statement_list_insert(param, paramlist);
		}
	} while (t.type != tkn_new_line);
	//ted zkontrolujeme ze fakt prisel jen jeden argument
	ast_node_t *length = strlen_builtin(paramlist);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za length( az za EOL)
	return length;
}
ast_node_t *ast_substr(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	ast_node_t *paramlist = statement_list_init();
	ast_node_t *param;
	*idc = 42;//totally useless cislo
	int prm_cnt = 0;
	do {
		param = NULL;
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
		case tkn_comma:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		case tkn_integer:
		case tkn_double:
			param = function_param_constant(t);
			prm_cnt++;
			break;
		case tkn_string:
			param = function_param_constant(t);
			prm_cnt++;
			break;
		case tkn_identifier:
			param = function_param_identifier(t.value);
			prm_cnt++;
			break;
		}
		if (param != NULL)
		{
			statement_list_insert(param, paramlist);
		}
	} while (t.type != tkn_new_line);
	ast_node_t *substr = substr_builtin(paramlist);
	return substr;
	//all ok... ukazatel ted ukazuje na dalsi prikaz za substr( az za EOL)

}
ast_node_t *ast_ord(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	ast_node_t *paramlist = statement_list_init();
	ast_node_t *param;
	int prm_cnt = 0;
	do {
		param = NULL;
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
		case tkn_comma:
			continue;//zavorky ignorujeme a carky taky...
		case tkn_new_line:
			//volani je uspesne
			break;
		case tkn_integer:
		case tkn_double:
			param = function_param_constant(t);
			prm_cnt++;
			break;
		case tkn_string:
			param = function_param_constant(t);
			prm_cnt++;
			break;
		case tkn_identifier:
		
			param = function_param_identifier(t.value);
			prm_cnt++;
			break;
		}
		if (param != NULL)
		{
			statement_list_insert(param, paramlist);
		}
	} while (t.type != tkn_new_line);
	ast_node_t *ord = ord_builtin(paramlist);
	return ord;
	//all ok... ukazatel ted ukazuje na dalsi prikaz za ord( az za EOL)
}
ast_node_t *ast_chr(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	ast_node_t *paramlist = statement_list_init();
        ast_node_t *param;
	int prm_cnt = 0;
	do {
		param = NULL;
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//zavorky ignorujeme
		case tkn_new_line:
			//volani je uspesne
			break;
		case tkn_integer:
		case tkn_double:
			prm_cnt++;
			param = function_param_constant(t);
			break;
		case tkn_identifier:
			param = function_param_identifier(t.value);
			prm_cnt++;
			break;
		}
		if (param != NULL)
                {
                        statement_list_insert(param, paramlist);
                }
	} while (t.type != tkn_new_line);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za chr( az za EOL)
	ast_node_t *chr = chr_builtin(paramlist);
	return chr;
}




ast_node_t *ast_call_func(token_t came, symtable_node_t **tabulka, unsigned int zanoreni)
{
	//came = nazev fce
	symtable_value_t param_cnt = my_malloc(sizeof(symtable_value_t)); // bez mallocu mi to davalo segfault
	symtable_value_t idc = my_malloc(sizeof(int));//funkce vraci value ale je mi k nicemu pri overzovani variables
	*idc = 42;//totally useless cislo
	int this_param_cnt = 0;
	token_t t;
	ast_node_t *param_list = statement_list_init();
	ast_node_t *param;

	do {
		param = NULL;
		t = list_get_active(all_tokens);//parametry, carky, zavorky
		list_active_next(all_tokens);

		switch (t.type)
		{
		case tkn_comma:
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//ignorujem, syntaxe uz kontrolvala ze to tam je dobre
		case tkn_new_line:
			break;//end of call func
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			this_param_cnt++;
			//predani primo cisla, or neco takoveho
			param = function_param_constant(t);
			break;
			
		case tkn_identifier:
			//prisel parametr funkce
		
			this_param_cnt++;
			param = function_param_identifier(t.value);
				//v came je nazev, tady prichazi postupne dalsi parametry
			break;
		}
		if (param != NULL)
			statement_list_insert(param, param_list);
	} while (t.type != tkn_new_line);
	//konec volani fuknce
	ast_node_t *fnc_call = function_call(came.value, param_list);
	return fnc_call;
}


ast_node_t *ast_assign(token_t came, symtable_node_t **tabulka,unsigned int zanoreni)
{
	token_t t, u;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo



	ast_node_t *id;

	if(!symtable_search(*tabulka, came.value, idc)) //id neni v tabulce -- jeste nebylo deklarovano
	{
		id = id_declare(came); // deklarujeme id
		symtable_insert(tabulka, came.value, idc); // vlozime do tabulky symbolu abychom ho pozdeji nemohli re-deklarovat

	}
	else
	{
		id = id_reference(came);

	}	

	


	t = list_get_active(all_tokens);//prvni prvek za =
	list_active_next(all_tokens);
	ast_node_t *rvalue;
	
	switch (t.type)
	{
	case tkn_left_bracket:
		list_active_previous(all_tokens);//nastavime na prvni zavorku
		rvalue = expression(all_tokens, *tabulka);
		break;
	case tkn_integer: //a = 5 
	case tkn_double: 
	case tkn_string:
	case tkn_key_nil:
		u = t; // ulozeni hodnoty tokenu pred posunutim
		t = list_get_active(all_tokens);//prvek za itemem
		//neni posun
		if (t.type == tkn_new_line)
		{	//obycejne prirazeni normalni hodoty
			rvalue = constant_declare(u);
			list_active_next(all_tokens);//posunuti za EOL a return
		}
		else if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
		{
			list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
			rvalue = expression(all_tokens, *tabulka);
		}
		break;
	case tkn_identifier: //a = id \n

		if (symtable_search(global_full_tabulka, t.value, idc))
		{
		
			//pokud toto ID jiz bylo pouzito jako nazev funkce, tak se prepneme do call_func (kde znovu otestujeme podle zanoreni jestli je to jiz definovana funkce
			rvalue = ast_call_func(t,tabulka,zanoreni);
			break;
		}
		u = t; // ulozeni hodnoty tokenu pred posunutim
		t = list_get_active(all_tokens);//prvek za ID
		if (t.type == tkn_new_line)
		{	//obycejne prirazeni ID do ID
			rvalue = id_reference(u);	
			list_active_next(all_tokens);//posunuti za EOL a return
		}
		else if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
		{
			list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
			rvalue = expression(all_tokens, *tabulka);
			D printf("prisel operand v casti prirazeni");
		}
		break;
	case tkn_key_inputf:
		rvalue = ast_inputf(tabulka);
		//came -> inputf
		break;
	case tkn_key_inputi:
		rvalue = ast_inputi(tabulka);
		break;
	case tkn_key_inputs:
		rvalue = ast_inputs(tabulka);
		break;
	case tkn_key_print:
		rvalue = ast_print(tabulka);
		//mov came, nil
		break;
	case tkn_key_length:
		rvalue = ast_length(tabulka);
		break;
	case tkn_key_substr:
		rvalue = ast_substr(tabulka);
		break;
	case tkn_key_ord:
		rvalue = ast_ord(tabulka);
		break;
	case tkn_key_chr:
		rvalue = ast_chr(tabulka);
		break;
	}
	ast_node_t *assign = id_assign(id, rvalue);
	return assign;
}

ast_node_t *ast_id_came(symtable_node_t **tabulka, token_t came, unsigned int zanoreni)
{
	token_t t;
	t = list_get_active(all_tokens);
	list_active_next(all_tokens);
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	D i++;
	D printf("prvek %d je neco po ID\n", i);
	ast_node_t *node;
	switch (t.type)
	{
	case tkn_left_bracket:
		D printf("prvek %d je (\n", i);
		node = ast_call_func(came, tabulka, zanoreni);//zde je aktivni prvni parametr a predame came jako parametr jako nazev funkce
		break;
	case tkn_identifier:
	case tkn_integer:
	case tkn_key_nil:
	case tkn_string:
	case tkn_double:
		list_active_previous(all_tokens);//ñastaveni aktivniho na prvni parametr
		node = ast_call_func(came, tabulka, zanoreni);//came je nazev funkce
		break;
	case tkn_equal:
		D printf("prvek %d je =\n", i);
		node = ast_assign(came, tabulka,zanoreni);//came je nazev promenne do ktere prirazujeme
		break;
	case tkn_plus:
	case tkn_minus:
	case tkn_star:
	case tkn_slash:
		D printf("prvek %d je opearotor\n", i);
		list_active_previous(all_tokens);//nastaveni na znamenko
		list_active_previous(all_tokens);//nastaveni na prvni prvek expression
		node = expression(all_tokens, *tabulka);
		break;
	case tkn_new_line:
		if (symtable_search(*tabulka, came.value, idc))	// prohledame lokalni tabulku
		{
			node = id_reference(came);	// id nalezeno
		}
		else
		{//zde se podivame do tabulky funkci
	
			switch (zanoreni)
			{
			case NOT_INSIDE_DEF://pokud nejsme uvnitr funkce divame se do tabulky kterou vytvarime az ted
				if (symtable_search(global_tabulka, came.value, idc))
				{
					ast_node_t *paramlist = statement_list_init();
					//je to funkce... vyresit call a v idc je pocet parametru
					node = function_call(came.value, paramlist);
				}

				break;
			case INSIDE_DEF://pokud sme uvnitr funkce tak se divame do plne tabulky, jestli nahodou uz ta funkce neexistuje az pod ni
				if (symtable_search(global_full_tabulka, came.value, idc))
				{	//je to funkce... vyresit call a v idc je pocet parametru
					ast_node_t *paramlist = statement_list_init();
					node = function_call(came.value, NULL);
				}
				break;
			}

		}
		break;
	case tkn_key_inputf:
		node = ast_inputf(tabulka);
		break;
	case tkn_key_inputi:
		node = ast_inputi(tabulka);
		break;
	case tkn_key_inputs:
		node = ast_inputs(tabulka);
		break;
	case tkn_key_print:
		node = ast_print(tabulka);
		break;
	case tkn_key_length:
		node = ast_length(tabulka);
		break;
	case tkn_key_substr:
		node = ast_substr(tabulka);
		break;
	case tkn_key_ord:
		node = ast_ord(tabulka);
		break;
	case tkn_key_chr:
		node = ast_chr(tabulka);
		break;
	}
	return node;	
}

ast_node_t *ast_if_came(symtable_node_t **tabulka,unsigned int zanoreni)
{
	//ukazuje na zacatek bool expr...
	ast_node_t *expression_node = expression(all_tokens, *tabulka); // zkontrolovat jestli jde o bool vyraz
	
	list_active_next(all_tokens);
	list_active_next(all_tokens);
	//ukazuje na prvni prikaz v ifu
	ast_node_t *statement_list_true = ast_inside_commands_end(tabulka, zanoreni);

	//prislo ELSE a EOL a ted zase ukazuje na dalsi prvni prikaz v else casti
	ast_node_t *statement_list_false = ast_inside_commands_end(tabulka, zanoreni);
	//ukazujeme ZA eol za endem
	ast_node_t *node = if_statement(expression_node, statement_list_true, statement_list_false);
	return node;
}


ast_node_t *ast_while_came(symtable_node_t **tabulka, unsigned int zanoreni)
{
	//ukazuje na zacatek bool expr...
	ast_node_t *expression_node = expression(all_tokens, *tabulka);

	list_active_next(all_tokens);
	list_active_next(all_tokens);
	//ukazuje na prvni prikaz ve whilu
	ast_node_t *statement_list = ast_inside_commands_end(tabulka,zanoreni);
	//ukazujeme ZA eol za endem
	ast_node_t *node = while_statement(expression_node, statement_list);
	return node;
}


void ast_def_func(symtable_node_t **main_tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	t = list_get_active(all_tokens);//name of func
	list_active_next(all_tokens);
	D i++;
	D printf("prvek %d je nazev funkce\n", i);
	ast_node_t *param_list = statement_list_init();
	ast_node_t *param;
	
	symtable_node_t *local_tabulka_true;
	local_tabulka_true= symtable_init();//init tabulky

	symtable_node_t **local_tabulka = my_malloc(sizeof(symtable_node_t*));//alokace pointeru v pameti, protoze potzrebujeme ukazetel na ukazatel...
	*local_tabulka = local_tabulka_true;


	symtable_value_t param_cnt = my_malloc(sizeof(int));
	*param_cnt = 0;
	symtable_insert(&global_tabulka, t.value, param_cnt);

	char *fnc_name = t.value;

	do {
		param = NULL;
		t = list_get_active(all_tokens);//param or ( or ) or , ; EOL means end of cycle
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_comma:
		case tkn_left_bracket:
		case tkn_right_bracket:
			continue;
		case tkn_identifier:
			symtable_insert(local_tabulka, t.value, idc);//pridani nazvu promennych do lokalni tabulky co zname uvnitr teto funkce
			param = function_param_identifier(t.value);
			D printf("zvetsim pocet parametru\n");
			(*param_cnt)++;//zvetseni poctu pozadovanych parametru
			D printf("zvetsil sem pocet parametru na: %d\n",*param_cnt);
			break;
		case tkn_new_line:
			break;
		}
		if (param != NULL)
		{
			statement_list_insert(param, param_list);	
		}
	} while (t.type != tkn_new_line);
	D printf("konecny pocet paramtru: %d\n", *param_cnt);

	//ted ukazatel ukazuje za EOL, neboli prvni prikaz funkce nebo end
	ast_node_t *statement_list = ast_inside_commands_end(local_tabulka,INSIDE_DEF); // tohle mi musi vratit statement list funkce
	//tady by to melo ukazovat za EOL za endem

	ast_node_t *fnc_define = function_define(fnc_name, param_list, statement_list);
	ast_node_t *statement = statement_make(fnc_define);
	statement_list_insert(statement, function_defs);
}


ast_node_t *ast_inside_commands_end(symtable_node_t **local_tabulka, unsigned int zanoreni)
{
	token_t t;
	ast_node_t *statement_list = statement_list_init();
	ast_node_t *statement;
	do
	{
		statement = NULL;
		t = list_get_active(all_tokens);
		list_active_next(all_tokens);

		switch (t.type)
		{
		case tkn_identifier:
			statement = statement_make(ast_id_came(local_tabulka, t,zanoreni));
			break;
		case tkn_left_bracket:
			list_active_previous(all_tokens);//ukazujeme na prvni zavorku
			statement = statement_make(expression(all_tokens, *local_tabulka));
			break;
		case tkn_new_line:
			continue;
		case tkn_key_if:
			statement = statement_make(ast_if_came(local_tabulka, zanoreni)); // if... za ifem prijde expression nejaka a dela se dokut neprijde tkn "then" pak se o jedno posune (EOL) a pak zacina vnitrni commandy na to bych udelal celou new funkci
									//ta pomocna funkce dostane cim se ukoncuje (else,end) a uvnitr proste bude delat co tento switch s whilem ale bez moznosti slova def (to prijit stejne nemuze to kontrolovala syntax)

			break;
		case tkn_key_while:
			statement = statement_make(ast_while_came(local_tabulka, zanoreni));//to stejny co if... ale je tam expression to se ukonci slovem do pak se preskoci eol a pak se skonci endem myslim... (stejna funkce na inside_commands jako u if)
			break;

		case tkn_key_inputf:
			statement = statement_make(ast_inputf(local_tabulka));
			break;
		case tkn_key_inputi:
			statement = statement_make(ast_inputi(local_tabulka));
			break;
		case tkn_key_inputs:
			statement = statement_make(ast_inputs(local_tabulka));
			break;
		case tkn_key_print:
			statement = statement_make(ast_print(local_tabulka));
			break;
		case tkn_key_length:
			statement = statement_make(ast_length(local_tabulka));
			break;
		case tkn_key_substr:
			statement = statement_make(ast_substr(local_tabulka));
			break;
		case tkn_key_ord:
			statement = statement_make(ast_ord(local_tabulka));
			break;
		case tkn_key_chr:
			statement = statement_make(ast_chr(local_tabulka));
			break;
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			t = list_get_active(all_tokens);
			if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
			{
				list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
				statement = statement_make(expression(all_tokens, *local_tabulka));
			}
			break;

		case tkn_key_end:
		case tkn_key_else:
			//end...
			break;
		}
		if (statement != NULL)
			statement_list_insert(statement, statement_list);
	} while ((t.type) != tkn_key_end && (t.type) != tkn_key_else);
	//ukazuje na EOL
	list_active_next(all_tokens);//posunuti za EOL
	return statement_list;
}



// hlavni funkce pro generovani stromu
ast_node_t *ast_build(symtable_node_t *all_def_funkce, list_t * all_tokens_came)
{
	all_tokens = all_tokens_came;
	list_activate_first(all_tokens);
	symtable_node_t *global_full_tabulka = all_def_funkce;

	function_defs = statement_list_init();

	
	symtable_node_t *main_tabulka_true = symtable_init();

	symtable_node_t **main_tabulka = my_malloc(sizeof(symtable_node_t*));
	*main_tabulka = main_tabulka_true;


	global_tabulka=symtable_init();
	token_t t;
	
	ast_node_t *main_statement_list = statement_list_init();
	ast_node_t *statement = NULL;
	D printf("zacatek programu\n");




	D printf("po inicializaci tabulek\n");
	//do main tabulky se postupne pridavaji nazvy funkci co se nadefinovaly a nazvy promennych co jsou a =, a pred pouztim neceho z hlavniho tela se da na ni search
	// pokud je ale semantika uvnitr napriklad ast_def_func, tak si musí udìlat novou lokalni tabulku... do ni si nahaze parametry (zna je) a pak kdyz ma volat funkci tak se diva na globalni tabulku
	//ta globalni tabulka je vystupem syntakticke analyzy neboli jsou tam jmena vsech funkci co byly nadefinovany uzivatelem
	
	

	do
	{
		statement = NULL;
		t = list_get_active(all_tokens);
		list_active_next(all_tokens);
		
		D i++;
		D printf("prvek: %d a je to token: %d\n",i,t.type);
		switch (t.type)
		{
		case tkn_key_def:
			D printf("prvek %d je def\n", i);
			ast_def_func(main_tabulka); // definici funkce nechci pridat do main stmt listu
			break;
		case tkn_identifier:

//			D printf("sem v mainu: a podivam se jestli existuje v main tabulce %s\n",t.value);
//			D if (symtable_search(*main_tabulka, t.value, idc))
//				D printf("nasel sem tam: %s\n",t.value);
//			D else
//				D printf("nenasel sem tam: %s\n", t.value);
			statement = statement_make(ast_id_came(main_tabulka,t,NOT_INSIDE_DEF));

								//jen tak pro info tim "zahodit" myslim jen posunout o jedno ukazatel
								//pokud prislo ID tak se zkontroluje co je dalsi (je na to funkce get_after_active), momentale ale aktivnim je uz stejne to za tim (na zacatkuhned po ziskani hodnoty to posunu)
								//pokud prislo = tak se prepne do expression modu
								//pokud prisel EOL tak se podivat do main tabulky jestli je to funkce nebo jestli je to promenna(pokud ji nezna tak semantic error)
								//pokud prislo dalsi id tak to bylo volani funkce a to dalsi id je prvni parametr (nacitam dalsi parametry dokud ne EOL)
									//ve tvaru jeden zahodim to je carka dalsi je parametr (ten uplne prvni se kontroluje na zacatku
								//pokud prisla '(' tak nacitam parametry dokud neprijde ')' a pak jeden token zahodim ten je EOL (to uz kontrolovala syntaxe)
									//ve tvaru param zahodit param zahodit atd... (zahodit jsou carky)(carky uz kontrolovala syntaxe jsou to vzdy carky)
								//pokud prisel operator tak je to expression bez prirazeni a uz to prvni "t" co se nacetlo pred timto je jeden operand
									//je potreba zkontrolovat jestli je v main_tabulce uz prvni operand (vzdy to bude promenna scitani s funkci nemuzou testovat protoze to je rozsireni)
									//a pak je potreba operand zahodit operand zahodit (pokazdy kontrolovat jestli je operand v main tabulce
									//mozna kontrolovat if tkn_string tak se prepnout do modu prisel string a pokud prijme tkn_integer nebo tkn_float tak hned semanticka chyba
			break;

		case tkn_left_bracket:
			list_active_previous(all_tokens);//ukazujeme na prvni zavorku
			// vyraz bez prirazeni do promenne ignorujeme
			break;

		case tkn_new_line:
			D printf("prvek %d je EOL\n", i);
			continue;
		case tkn_key_if:
			D printf("prvek %d je IF\n", i);
			statement = statement_make(ast_if_came(main_tabulka,NOT_INSIDE_DEF)); // if... za ifem prijde expression nejaka a dela se dokut neprijde tkn "then" pak se o jedno posune (EOL) a pak zacina vnitrni commandy na to bych udelal celou new funkci
									//ta pomocna funkce dostane cim se ukoncuje (else,end) a uvnitr proste bude delat co tento switch s whilem ale bez moznosti slova def (to prijit stejne nemuze to kontrolovala syntax)
			break;
		case tkn_key_while:
			D printf("prvek %d je WHILE\n", i);
			statement = statement_make(ast_while_came(main_tabulka, NOT_INSIDE_DEF));//to stejny co if... ale je tam expression to se ukonci slovem do pak se preskoci eol a pak se skonci endem myslim... (stejna funkce na inside_commands jako u if)
			break;
		case tkn_key_inputf:
			statement = statement_make(ast_inputf(main_tabulka));
			break;//sem v mainu => ignorovat
		case tkn_key_inputi:
			statement = statement_make(ast_inputi(main_tabulka));
			break;//sem v mainu => ignorovat
		case tkn_key_inputs:
			//inputs
			statement = statement_make(ast_inputs(main_tabulka));
			//ani negenerovat asm asi 
			break;//sem v mainu => ignorovat
		case tkn_key_print:
			statement = statement_make(ast_print(main_tabulka));
			break;
		case tkn_key_length:
			statement = statement_make(ast_length(main_tabulka));
			break;
		case tkn_key_substr:
			statement = statement_make(ast_substr(main_tabulka));
			break;
		case tkn_key_ord:
			statement = statement_make(ast_ord(main_tabulka));
			break;
		case tkn_key_chr:
			statement = statement_make(ast_chr(main_tabulka));
			break;

		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			t = list_get_active(all_tokens);
			if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
			{
				list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
				statement = statement_make(expression(all_tokens, *main_tabulka));
			}
			else
			{
				list_active_next(all_tokens);//tady prisel EOL aspon by podle semantiky mel
			}	
			break;

		case tkn_end_of_file:
			//end...
			break;
		}
		if (statement != NULL)
			statement_list_insert(statement, main_statement_list);
	} while ((t.type) != tkn_end_of_file);
	
	// konec programu
	ast_node_t *program = program_node(function_defs, main_statement_list);

	generate(program);
}










