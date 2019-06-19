/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: error.h
 *
 */
#ifndef _ERROR_H_
#define _ERROR_H_

#define SYNTAX_ERR 2

#define SEMANTIC_VAR_DEF 3//redefinitiom
#define SEMANTIC_TYP_EXPR 4//str+int
#define SEMANTIC_PARAM_CNT 5//spatny pocet param
#define SEMANTIC_OTHER 6//ostatni semanticke chyby

#define RUN_ERR_DIVISION_ZERO 9

#define INTERN_ERROR 99

/**
* na stderr vypise syntax chybu ve formatu Syntax Error: param \n a ukonci program s navratovou hodnotou 2(SYNTAX_ERR)
* @param string popisujici chybu
*/
void syntax_error(const char *str);


/**
* na stderr vypise semantic chybu ve formatu Semantic Error: param \n a ukonci program s prislusnou navratovou hodnotou
* @param str popisujici chybu
* @param exit_code exit code podle zadani
*/
void semantic_error(const char *str, unsigned int exit_code);

#endif 
