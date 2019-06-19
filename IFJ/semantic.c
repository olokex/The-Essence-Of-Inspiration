/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: semantic.c
 *
 */
#include "semantic.h"
#include "symtable.h"
#include "error.h"
#include "list.h"
#include "syntax.h"
#include  "scanner.h"
#include "my_malloc.h"
#include "ast.h"
#include "ast_build.h"

list_t * all_tokens;
symtable_node_t *global_tabulka;

symtable_node_t *global_full_tabulka;

extern ast_node_t *ast_build(symtable_node_t *all_def_funkce, list_t * all_tokens_came);

void sem_inputf(symtable_node_t **tabulka) 
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
		default:
			semantic_error("inputf takes no argument",SEMANTIC_PARAM_CNT);
		}

	} while (t.type != tkn_new_line);

	//all ok... ukazatel ted ukazuje na dalsi prikaz za inputf( az za EOL)

}
void sem_inputi(symtable_node_t **tabulka)
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
		default:
			semantic_error("inputi takes no argument", SEMANTIC_PARAM_CNT);
		}

	} while (t.type != tkn_new_line);

	//all ok... ukazatel ted ukazuje na dalsi prikaz za inputi( az za EOL)

}
void sem_inputs(symtable_node_t **tabulka)
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
		default:
			semantic_error("inputs takes no argument", SEMANTIC_PARAM_CNT);
			break;
		}

	} while (t.type != tkn_new_line);

	//all ok... ukazatel ted ukazuje na dalsi prikaz za inputs( az za EOL)

}
void sem_print(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	do {
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
			//v kazdym toto case se jen zavola ASM funkce WRITE s danym typem ez
			break;
		case tkn_identifier:
			if (symtable_search(*tabulka, t.value, idc) == false)
				semantic_error("unknown id at arument of print", SEMANTIC_VAR_DEF);
			break;
		}
	} while (t.type != tkn_new_line);

	//all ok... ukazatel ted ukazuje na dalsi prikaz za print( az za EOL)

}
void sem_length(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	int prm_cnt = 0;
	do {
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
			//jen zavolat
			prm_cnt++;
			break;
		case tkn_identifier:
			if (symtable_search(*tabulka, t.value, idc) == false)
				semantic_error("unknown id at arument of length", SEMANTIC_VAR_DEF);
			prm_cnt++;
			//zde se zjisti jestli je string a zavola se strlen
			break;
		default:
			semantic_error("unknown argument of length", SEMANTIC_TYP_EXPR);
			break;
		}
	} while (t.type != tkn_new_line);
	//ted zkontrolujeme ze fakt prisel jen jeden argument
	if(prm_cnt!=1)
		semantic_error("wrong number of arguments of length", SEMANTIC_PARAM_CNT);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za length( az za EOL)

}
void sem_substr(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	int prm_cnt = 0;
	do {
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
			if (prm_cnt == 0)
				semantic_error("1st argument of substr cannot be number", SEMANTIC_TYP_EXPR);
			prm_cnt++;
			break;
		case tkn_string:
			if (prm_cnt != 0)
				semantic_error("only 1st argument of substr can be string", SEMANTIC_TYP_EXPR);
			prm_cnt++;
			break;
		case tkn_identifier:
			if (symtable_search(*tabulka, t.value, idc) == false)
				semantic_error("unknown id at arument of substr", SEMANTIC_VAR_DEF);
			prm_cnt++;
			break;
		default:
			semantic_error("unknown argument of substr", SEMANTIC_TYP_EXPR);
			break;
		}
	} while (t.type != tkn_new_line);
	//ted zkontrolujeme ze fakt prisly jen 3 argumenty
	if (prm_cnt != 3)
		semantic_error("wrong number of arguments of substr", SEMANTIC_PARAM_CNT);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za substr( az za EOL)

}
void sem_ord(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	int prm_cnt = 0;
	do {
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
			if (prm_cnt == 0)
				semantic_error("1st argument of ord cannot be number", SEMANTIC_TYP_EXPR);
			prm_cnt++;
			break;
		case tkn_string:
			if (prm_cnt != 0)
				semantic_error("only 1st argument of ord can be string", SEMANTIC_TYP_EXPR);
			prm_cnt++;
			break;
		case tkn_identifier:
			if (symtable_search(*tabulka, t.value, idc) == false)
				semantic_error("unknown id at arument of ord", SEMANTIC_VAR_DEF);
			prm_cnt++;
			break;
		default:
			semantic_error("unknown argument of ord", SEMANTIC_TYP_EXPR);
			break;
		}
	} while (t.type != tkn_new_line);
	//ted zkontrolujeme ze fakt prisly jen 2 argumenty
	if (prm_cnt != 2)
		semantic_error("wrong number of arguments of ord", SEMANTIC_PARAM_CNT);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za ord( az za EOL)

}
void sem_chr(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	int prm_cnt = 0;
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
		case tkn_integer:
		case tkn_double:
			prm_cnt++;
			break;
		case tkn_identifier:
			if (symtable_search(*tabulka, t.value, idc) == false)
				semantic_error("unknown id at arument of chr", SEMANTIC_VAR_DEF);
			prm_cnt++;
			break;
		default:
			semantic_error("unknown argument of chr", SEMANTIC_TYP_EXPR);
			break;
		}
	} while (t.type != tkn_new_line);
	//ted zkontrolujeme ze fakt prisel jen 1 argument
	if (prm_cnt != 1)
		semantic_error("wrong number of arguments of chr", SEMANTIC_PARAM_CNT);
	//all ok... ukazatel ted ukazuje na dalsi prikaz za chr( az za EOL)

}




void semantic_call_func(token_t came, symtable_node_t **tabulka, unsigned int zanoreni)
{
	//came = nazev fce
	symtable_value_t param_cnt = my_malloc(sizeof(int));;
	symtable_value_t idc = my_malloc(sizeof(int));//funkce vraci value ale je mi k nicemu pri overzovani variables
	*idc = 42;//totally useless cislo
	int this_param_cnt = 0;
	token_t t;
	


	switch (zanoreni)
	{
	case NOT_INSIDE_DEF:
		if (symtable_search(global_tabulka, came.value, param_cnt)==false)
			semantic_error("undefined reference to call function from main", SEMANTIC_VAR_DEF);
		break;
	case INSIDE_DEF:
		if (symtable_search(global_full_tabulka, came.value, param_cnt)==false)
			semantic_error("undefined reference to call func from func", SEMANTIC_VAR_DEF);
		break;
	}
	//je to funkce... vyresit call a v param_cnt je pocet parametru

	do {
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
			break;
			
		case tkn_identifier:
			//prisel parametr funkce
			this_param_cnt++;
			if (symtable_search(*tabulka,t.value,idc) == false)//pokud se snazime predat promennou co nezname tak error
				semantic_error("undefined reference in parametrs function", SEMANTIC_VAR_DEF);
			break;
		}

	} while (t.type != tkn_new_line);
	//konec volani fuknce
	if (this_param_cnt != (*param_cnt))//pokud se neshoduje pocet parametru s definovavym poctem tak error
		semantic_error("Wrong number of params", SEMANTIC_PARAM_CNT);

	
}


void semantic_assign(token_t came, symtable_node_t **tabulka,unsigned int zanoreni)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	if (symtable_search(global_tabulka, came.value, idc))
		semantic_error("redefinition of func", SEMANTIC_VAR_DEF);//pokud sme nasli jiz toto jmeno v tabulce tak je to error


	if (symtable_search(*tabulka, came.value, idc) == false)
	{
		symtable_insert(tabulka, came.value, idc);//pridani prvku do lokalni tabulky
	}
	
	t = list_get_active(all_tokens);//prvni prvek za =
	list_active_next(all_tokens);

	switch (t.type)
	{
	case tkn_left_bracket:
		list_active_previous(all_tokens);//nastavime na prvni zavorku
		semantic_expr_aritmetic(tabulka, ASSIGN);//ukazuje uz na prvni prvek vyrazu
		break;
	case tkn_integer: //a = 5 
	case tkn_double: 
	case tkn_string:
	case tkn_key_nil:
		t = list_get_active(all_tokens);//prvek za itemem
		//neni posun
		if (t.type == tkn_new_line)
		{
			list_active_next(all_tokens);//posunuti za EOL a return
			return;
		}
		else if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
		{
			list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
			semantic_expr_aritmetic(tabulka, ASSIGN);
		}
		break;
	case tkn_identifier:

		if (symtable_search(global_full_tabulka, t.value, idc))
		{
			switch (zanoreni)
			{
			case NOT_INSIDE_DEF:
				if (symtable_search(global_tabulka, t.value, idc))
				{
					semantic_call_func(t, tabulka, zanoreni);
					return;
				}
				//else pokud nebyl nalezen v prozatim vytvorene tabulce(v plne byl ale to je jedno) tak zatim predpokladame ze je to lokalni promenna
				break;
			case INSIDE_DEF:
				semantic_call_func(t, tabulka, zanoreni);
				return;
			}
			
			//pokud toto ID jiz bylo pouzito jako nazev funkce, tak se prepneme do call_func (kde znovu otestujeme podle zanoreni jestli je to jiz definovana funkce

		}
		else if (symtable_search(*tabulka, t.value, idc) == false)
			semantic_error("undefined reference to ID in assingment", SEMANTIC_VAR_DEF);
		t = list_get_active(all_tokens);//prvek za ID
		if (t.type == tkn_new_line)
		{	
			list_active_next(all_tokens);//posunuti za EOL a return
			return;
		}
		else if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
		{
			list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
			semantic_expr_aritmetic(tabulka, ASSIGN);
		}
		break;
	case tkn_key_inputf:
		sem_inputf(tabulka);
		break;
	case tkn_key_inputi:
		sem_inputi(tabulka);
		break;
	case tkn_key_inputs:
		sem_inputs(tabulka);
		break;
	case tkn_key_print:
		sem_print(tabulka);
		break;
	case tkn_key_length:
		sem_length(tabulka);
		break;
	case tkn_key_substr:
		sem_substr(tabulka);
		break;
	case tkn_key_ord:
		sem_ord(tabulka);
		break;
	case tkn_key_chr:
		sem_chr(tabulka);
		break;
	}

}

void semantic_id_came(symtable_node_t **tabulka, token_t came, unsigned int zanoreni)
{
	token_t t;
	t = list_get_active(all_tokens);
	list_active_next(all_tokens);
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	switch (t.type)
	{
	case tkn_left_bracket:
		semantic_call_func(came, tabulka, zanoreni);//zde je aktivni prvni parametr a predame came jako parametr jako nazev funkce
		break;
	case tkn_identifier:
	case tkn_integer:
	case tkn_key_nil:
	case tkn_string:
	case tkn_double:
		list_active_previous(all_tokens);//ñastaveni aktivniho na prvni parametr
		semantic_call_func(came, tabulka, zanoreni);//came je nazev funkce
		break;
	case tkn_equal:
		semantic_assign(came, tabulka,zanoreni);//came je nazev promenne do ktere prirazujeme
		break;
	case tkn_plus:
	case tkn_minus:
	case tkn_star:
	case tkn_slash:
		list_active_previous(all_tokens);//nastaveni na znamenko
		list_active_previous(all_tokens);//nastaveni na prvni prvek expression
		semantic_expr_aritmetic(tabulka,CAN_BE_RETURN);
		break;
	case tkn_new_line:
		if (symtable_search(*tabulka, came.value, idc))//pokud neni v tabulce lokalnich symbolu tak se podivame do tabulky funkci
		{
			;//je v tabulce lokalnich symbolu a nic se nedeje, muze byt navratova hodnota
		}
		else
		{//zde se podivame do tabulky funkci
			switch (zanoreni)
			{
			case NOT_INSIDE_DEF://pokud nejsme uvnitr funkce divame se do tabulky kterou vytvarime az ted
				if (symtable_search(global_tabulka, came.value, idc))
					if ((*idc) != 0)//pokud neni pozadovany pocet parametru 0 tak error
						semantic_error("expected params of function", SEMANTIC_PARAM_CNT);
					else;
				else
					semantic_error("undefined reference", SEMANTIC_VAR_DEF);
				break;
			case INSIDE_DEF://pokud sme uvnitr funkce tak se divame do plne tabulky, jestli nahodou uz ta funkce neexistuje az pod ni
				if (symtable_search(global_full_tabulka, came.value, idc))
					if ((*idc) != 0)//pokud neni pozadovany pocet parametru 0 tak error
						semantic_error("expected params of function", SEMANTIC_PARAM_CNT);
					else
						;
				else
					semantic_error("undefined reference",SEMANTIC_VAR_DEF);
				break;
			}

		}
		break;
	case tkn_key_inputf:
		sem_inputf(tabulka);
		break;
	case tkn_key_inputi:
		sem_inputi(tabulka);
		break;
	case tkn_key_inputs:
		sem_inputs(tabulka);
		break;
	case tkn_key_print:
		sem_print(tabulka);
		break;
	case tkn_key_length:
		sem_length(tabulka);
		break;
	case tkn_key_substr:
		sem_substr(tabulka);
		break;
	case tkn_key_ord:
		sem_ord(tabulka);
		break;
	case tkn_key_chr:
		sem_chr(tabulka);
		break;
	default:
		syntax_error("id came, but next token looks wrong_uvnitr_semantiky_toto_se_stat_nemuze...");
	}

	
}

void semantic_if_came(symtable_node_t **tabulka,unsigned int zanoreni)
{
	//ukazuje na zacatek bool expr...
	semantic_expr_bool(tabulka);
	
	//ukazuje na prvni prikaz v ifu
	semantic_inside_commands_end(tabulka, zanoreni);
	//prislo ELSE a EOL a ted zase ukazuje na dalsi prvni prikaz v else casti
	semantic_inside_commands_end(tabulka, zanoreni);
	//ukazujeme ZA eol za endem

}


void semantic_while_came(symtable_node_t **tabulka, unsigned int zanoreni)
{
	//ukazuje na zacatek bool expr...
	semantic_expr_bool(tabulka);
	
	//ukazuje na prvni prikaz ve whilu
	semantic_inside_commands_end(tabulka,zanoreni);
	//ukazujeme ZA eol za endem
}


void semantic_expr_bool(symtable_node_t **tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	do {
		t = list_get_active(all_tokens);//id, operatory, itemy, zavorky vzdycky ukazuje na prvni cast vyrazu
		list_active_next(all_tokens);
		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//ignorujem, syntaxe uz kontrolvala ze to tam je dobre
		case tkn_star:
		case tkn_plus:
		case tkn_slash:
		case tkn_minus:
		case tkn_equal_2:
		case tkn_not_equal:
		case tkn_less:
		case tkn_less_or_equal:
		case tkn_more:
		case tkn_more_or_equal:
			continue;
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			break;

		case tkn_identifier:
			//prisel parametr BOOL vyrazu
			if (symtable_search(*tabulka, t.value, idc) == false)//pokud se snazime delat vyraz s promennou co nezname tak error
				semantic_error("undefined reference in bool expr", SEMANTIC_VAR_DEF);
			break;

		case tkn_key_do:
		case tkn_key_then:
			break;//end of bool expression
		}

	} while (t.type != tkn_key_then && t.type != tkn_key_do);
	//konec vyrazu
	//ted ukazuje na EOL za do/then
	list_active_next(all_tokens);//posunuti na prvni prikaz ifu/whilu
}



void semantic_expr_aritmetic(symtable_node_t **tabulka, unsigned int type)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	int str_came = 0;
	int number_came = 0;
	do {
		t = list_get_active(all_tokens);//id, operatory, itemy, zavorky
		list_active_next(all_tokens);

		switch (t.type)
		{
		case tkn_right_bracket:
		case tkn_left_bracket:
			continue;//ignorujem, syntaxe uz kontrolvala ze to tam je dobre
		case tkn_star:
		case tkn_plus:
		case tkn_slash:
		case tkn_minus:
			continue;
		case tkn_integer:
		case tkn_double:
			number_came++;
			break;
		case tkn_string:
			str_came++;
			break;
		case tkn_key_nil:
			semantic_error("Nil came in expression",SEMANTIC_TYP_EXPR);
			break;

		case tkn_identifier:
			//prisel parametr matematickeho vyrazu
			if (symtable_search(*tabulka, t.value, idc) == false)//pokud se snazime delat vyraz s promennou co nezname tak error
				semantic_error("undefined reference in aritm. expr", SEMANTIC_VAR_DEF);
			break;

		case tkn_new_line:
			break;//end of aritemtic expression
		}

	} while (t.type != tkn_new_line);
	//konec vyrazu
	if (str_came > 0 && number_came > 0)
		semantic_error("string operator cislo...", SEMANTIC_TYP_EXPR);//pokud prislo zaraz vic nez 0 cisel a vice nez 0 stringu ve vyrazu...
}


void semantic_def_func(symtable_node_t **main_tabulka)
{
	token_t t;
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo
	t = list_get_active(all_tokens);//name of func
	list_active_next(all_tokens);

	if (symtable_search(*main_tabulka, t.value, idc))//pokud najdeme jmeno teto funkce jako id co sme jiz pouzili v mainu je to redefinice tudiz chyba
		semantic_error("redefinition of variable to function",SEMANTIC_VAR_DEF);
	
	symtable_node_t *local_tabulka_true;
	local_tabulka_true= symtable_init();//init tabulky

	symtable_node_t **local_tabulka = my_malloc(sizeof(symtable_node_t*));//alokace pointeru v pameti, protoze potzrebujeme ukazetel na ukazatel...
	*local_tabulka = local_tabulka_true;


	symtable_value_t param_cnt = my_malloc(sizeof(int));
	*param_cnt = 0;
	symtable_insert(&global_tabulka, t.value, param_cnt);

	do {
		t = list_get_active(all_tokens);//param or ( or ) or , ; EOL means end of cycle
		list_active_next(all_tokens);
		
		switch (t.type)
		{
		case tkn_comma:
		case tkn_left_bracket:
		case tkn_right_bracket:
			continue;
		case tkn_identifier:
			if (symtable_search(global_tabulka, t.value, idc))
				semantic_error("Parametr of func is name of another func", SEMANTIC_VAR_DEF);
			
			if (symtable_search(*local_tabulka, t.value, idc))
				semantic_error("Parametr of func already used", SEMANTIC_VAR_DEF);
			symtable_insert(local_tabulka, t.value, idc);//pridani nazvu promennych do lokalni tabulky co zname uvnitr teto funkce
			(*param_cnt)++;//zvetseni poctu pozadovanych parametru
			break;
		case tkn_new_line:
			break;
		}

	} while (t.type != tkn_new_line);

	//ted ukazatel ukazuje za EOL, neboli prvni prikaz funkce nebo end
	semantic_inside_commands_end(local_tabulka,INSIDE_DEF);
	//tady by to melo ukazovat za EOL za endem

}


void semantic_inside_commands_end(symtable_node_t **local_tabulka, unsigned int zanoreni)
{
	token_t t;

	do
	{
		t = list_get_active(all_tokens);
		list_active_next(all_tokens);

		switch (t.type)
		{
		case tkn_identifier:
			semantic_id_came(local_tabulka, t,zanoreni);
			break;
		case tkn_left_bracket:
			list_active_previous(all_tokens);//ukazujeme na prvni zavorku
			semantic_expr_aritmetic(local_tabulka,CAN_BE_RETURN);//vyrazy... idk yet
			break;
		case tkn_new_line:
			continue;
		case tkn_key_if:
			semantic_if_came(local_tabulka, zanoreni); // if... za ifem prijde expression nejaka a dela se dokut neprijde tkn "then" pak se o jedno posune (EOL) a pak zacina vnitrni commandy
			break;
		case tkn_key_while:
			semantic_while_came(local_tabulka, zanoreni);//to stejny co if
			break;

		case tkn_key_inputf:
			sem_inputf(local_tabulka);
			break;
		case tkn_key_inputi:
			sem_inputi(local_tabulka);
			break;
		case tkn_key_inputs:
			sem_inputs(local_tabulka);
			break;
		case tkn_key_print:
			sem_print(local_tabulka);
			break;
		case tkn_key_length:
			sem_length(local_tabulka);
			break;
		case tkn_key_substr:
			sem_substr(local_tabulka);
			break;
		case tkn_key_ord:
			sem_ord(local_tabulka);
			break;
		case tkn_key_chr:
			sem_chr(local_tabulka);
			break;
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			t = list_get_active(all_tokens);
			if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
			{
				list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
				semantic_expr_aritmetic(local_tabulka, ASSIGN);
			}
			else//EOL

			break;

		case tkn_key_end:
		case tkn_key_else:
			//end...
			break;
		default:
			//semka ten program fakt nemuze dojit... pokud dojde tak je tim padem spatne asi nejaka funkce ktera se odsud volala... 
			syntax_error("toto by se teoreticky stat nemelo... protoze vse prislo po syntakticke kontrole");
			break;
		}

	} while ((t.type) != tkn_key_end && (t.type) != tkn_key_else);
	//ukazuje na EOL
	list_active_next(all_tokens);//posunuti za EOL

}

void semantic_check(symtable_node_t *all_def_funkce, list_t * all_tokens_came)
{
	all_tokens = all_tokens_came;
	list_activate_first(all_tokens);

	token_t t;
	
	
	symtable_value_t idc = my_malloc(sizeof(int));
	*idc = 42;//totally useless cislo

	symtable_node_t *main_tabulka_true;
	main_tabulka_true = symtable_init();

	symtable_node_t **main_tabulka = my_malloc(sizeof(symtable_node_t*));
	*main_tabulka = main_tabulka_true;


	global_tabulka=symtable_init();
	global_full_tabulka = all_def_funkce;

	//do main tabulky se postupne pridavaji nazvy funkci co se nadefinovaly a nazvy promennych co jsou a =, a pred pouztim neceho z hlavniho tela se da na ni search
	// pokud je ale semantika uvnitr napriklad semantic_def_func, tak si musí udìlat novou lokalni tabulku... do ni si nahaze parametry (zna je) a pak kdyz ma volat funkci tak se diva na globalni tabulku
	//ta globalni tabulka je vystupem syntakticke analyzy neboli jsou tam jmena vsech funkci co byly nadefinovany uzivatelem

	do
	{
		t = list_get_active(all_tokens);
		list_active_next(all_tokens);
		
		switch (t.type)
		{
		case tkn_key_def:
			semantic_def_func(main_tabulka);
			break;
		case tkn_identifier:
			semantic_id_came(main_tabulka,t,NOT_INSIDE_DEF);
			break;
		case tkn_left_bracket:
			list_active_previous(all_tokens);//ukazujeme na prvni zavorku
			semantic_expr_aritmetic(main_tabulka,CAN_BE_RETURN);//vyrazy
			break;
		case tkn_new_line:
			continue;
		case tkn_key_if:
			semantic_if_came(main_tabulka,NOT_INSIDE_DEF); 
			break;
		case tkn_key_while:
			semantic_while_came(main_tabulka, NOT_INSIDE_DEF);
			break;
		case tkn_key_inputf:
			sem_inputf(main_tabulka);
			break;//sem v mainu => ignorovat
		case tkn_key_inputi:
			sem_inputi(main_tabulka);
			break;//sem v mainu => ignorovat
		case tkn_key_inputs:
			sem_inputs(main_tabulka);
			break;//sem v mainu => ignorovat
		case tkn_key_print:
			sem_print(main_tabulka);
			break;
		case tkn_key_length:
			sem_length(main_tabulka);
			break;
		case tkn_key_substr:
			sem_substr(main_tabulka);
			break;
		case tkn_key_ord:
			sem_ord(main_tabulka);
			break;
		case tkn_key_chr:
			sem_chr(main_tabulka);
			break;
		case tkn_integer:
		case tkn_double:
		case tkn_string:
		case tkn_key_nil:
			t = list_get_active(all_tokens);
			if (t.type == tkn_minus || t.type == tkn_plus || t.type == tkn_star || t.type == tkn_slash)
			{
				list_active_previous(all_tokens);//ukazuje na prvni prvek vyrazu
				semantic_expr_aritmetic(main_tabulka, ASSIGN);
			}
			else
				list_active_next(all_tokens);//tady prisel EOL aspon by podle syntaxe mel
			break;
		case tkn_end_of_file:
			//end...
			break;
		default:
			//semka ten program fakt nemuze dojit... pokud dojde tak je tim padem spatne asi nejaka funkce ktera se odsud volala... 
			syntax_error("toto by se teoreticky stat nemelo... protoze vse prislo po syntakticke kontrole");
			break;
		}


	} while ((t.type) != tkn_end_of_file);

	ast_build(all_def_funkce ,all_tokens);
}



