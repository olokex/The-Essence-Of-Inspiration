/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: expression.h
 *
 */

#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include "ast.h"
#include "scanner.h"
#include "token.h"
#include "stack.h"
#include "my_malloc.h"
#include "ast.h"
#include "list.h"
#include "symtable.h"
#include "syntax.h"
#include "semantic.h"

/**
* pri zavolani bere stream input tokenu dokud nenarazi na EOL/EOF
* prevede tokeny z infixove na postfixovou notaci a nasledne vytvori
* abstraktni datovy strom
* @return vrcholovy uzel AST
*/
ast_node_t *expression(list_t *tkn_source, symtable_node_t *local_table);

// for debugging
void generate_code(ast_node_t *root);

#endif
