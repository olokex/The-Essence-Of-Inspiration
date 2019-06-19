/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: token.h
 *
 */


#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include "utils.h"


	
//vsechny mozne token typy
#define ENUM_TOKEN_TYPE(a)\
	a(tkn_double)\
	a(tkn_identifier)\
	a(tkn_integer)\
	a(tkn_minus)\
	a(tkn_plus)\
	a(tkn_comma)\
	a(tkn_right_bracket)\
	a(tkn_left_bracket)\
	a(tkn_star)\
	a(tkn_slash)\
	a(tkn_more)\
	a(tkn_more_or_equal)\
	a(tkn_less)\
	a(tkn_less_or_equal)\
	a(tkn_equal)\
	a(tkn_not_equal)\
	a(tkn_string)\
	a(tkn_new_line)\
	a(tkn_end_of_file)\
	a(tkn_equal_2)\
	a(tkn_key_def)\
	a(tkn_key_do)\
	a(tkn_key_else)\
	a(tkn_key_end)\
	a(tkn_key_if)\
	a(tkn_key_not)\
	a(tkn_key_nil)\
	a(tkn_key_then)\
	a(tkn_key_while)\
	a(tkn_key_inputs)\
	a(tkn_key_inputi)\
	a(tkn_key_inputf)\
	a(tkn_key_print)\
	a(tkn_key_length)\
	a(tkn_key_substr)\
	a(tkn_key_ord)\
	a(tkn_key_chr)\
	a(tkn_null)\
	a(tkn_node)

typedef enum tokentype {
    ENUM_TOKEN_TYPE(MAKE_ENUM)
} tokentype_t;
 
extern const char *tokentype_str[];
 

//struktura tokenu obsahuje 
//typ tokenu ve tvaru tkn_type
//hodnotu tokenu
typedef struct token {
    tokentype_t type;
    void *value;
} token_t;
 
#endif
