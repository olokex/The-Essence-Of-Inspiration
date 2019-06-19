/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: error.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void syntax_error(const char *str)
{
	fprintf(stderr, "Syntax Error: %s \n",str);
	exit(SYNTAX_ERR);
}

void semantic_error(const char *str, unsigned int exit_code)
{
	fprintf(stderr, "Semantic Error: %s \n", str);
	exit(exit_code);
}
