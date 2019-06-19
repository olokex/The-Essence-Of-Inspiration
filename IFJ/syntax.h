/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: syntax.h
 *
 */
#ifndef _SYNTAX_H_
#define _SYNTAX_H_

#include "symtable.h"

#define RBRACKET 12 //some cislo
#define NOBRACKET 13 //some cislo
#define END_THEN 14 //some cislo
#define END_DO 15 //some cislo
#define END_ELSE 16 //some cislo
#define END_END 17 //some cislo



/*
* @brief syntakticka kontrola prirazeni do promenne
*/
void assign();	
/*
* @brief rekurzivni syntakticka kontrola bool vyrazu
* @param typ jestli na konci ocekavame pravou zavorku ci nikoliv
* @param pokud na konci bude end nebo then (zalezi kde se bool vyraz vyskytnul)
*/
void expr_bool(unsigned int type, unsigned int if_or_while);
/*
* @brief rekurzivni syntakticka kontrola aritmetickeho vyrazu
* @param typ jestli na konci ocekavame pravou zavorku ci nikoliv
*/
void expr_aritmetic(unsigned int type);
/*
* @brief syntakticka kontrola while casti
*/
void while_came();
/*
* @brief syntakticka kontrola if casti
*/
void if_came();
/*
* @brief syntakticka kontrola parametru fce pri volani
* @param jestli se na konci ocekava ')' nebo ne
*/
void params(unsigned int type);
/*
* @brief syntakticka kontrola parametru fce pri definici
* @param ukazatel na pocet parametru, toto cislo se nachazi v symtable
*/
void params_func(symtable_value_t param_cnt);
/*
* @brief syntakticka kontrola vnitrnich prikazu (stejne jako klasicke prikazy ale nemuze se zde objevit slovo def (definice fce)
* @param jestli na konci ocekavame end nebo slovo else
*/
void inside_commands(unsigned int type);
/*
* @brief syntakticka kontrola definice fce, vola inside_commands a params_func
*/
void def_func();
/*
* @brief syntakticka kontrola volani fce
* @param jestli na konci ocekavame ')' nebo ne
*/
void call_func(unsigned int type);
/*
* @brief syntakticka kontrola po te co prislo id
*/
void id_came();
/*
* @brief zacatek celkove syntaxe, vola funkci semantic_check ze semantic.c, a postupne ostatni fce ze syntaxe podle toho co prislo na vstup
*/
void parse();
/*
* @brief po te co prisla leva zavorka, vola fukci aritmetic_expr a kontroluje zda je na konci EOL
*/
void epxr_left_bracket_checkEOL();
/*
* @brief po te co prisla leva zavorka, vola fukci bool_expr a kontroluje zda je na konci then/do
*/
void expr_bool_left_bracket_check_ending(unsigned int if_or_while);


#endif // !_SYNTAX_H_



