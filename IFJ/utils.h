/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: utils.h
 *
 */
#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


//case pro vsechna pismena, jen pro mala pismena, jen pro cisla, pro hexa znaky
#define CASE_SMALL_ALPHA 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z'
#define CASE_ALL_ALPHA  'A': case 'a': case 'B': case 'b': case 'C': case 'c': case 'D': case 'd': case 'E': case 'e': case 'F': case 'f': case 'G': case 'g': case 'H': case 'h': case 'I': case 'i': case 'J': case 'j': case 'K': case 'k': case 'L': case 'l': case 'M': case 'm': case 'N': case 'n': case 'O': case 'o': case 'P': case 'p': case 'Q': case 'q': case 'R': case 'r': case 'S': case 's': case 'T': case 't': case 'U': case 'u': case 'V': case 'v': case 'W': case 'w': case 'X': case 'x': case 'Y': case 'y': case 'Z': case 'z'
#define CASE_ALL_DIGIT  '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9'
#define CASE_HEXA '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'a': case 'b': case 'c': case 'd': case 'e': case'f'
 
#define MAKE_ENUM(e)  e,
#define MAKE_STRING(e)  #e,

static const size_t allocate = 64;

//strunktura utils_string
struct utils_string {
    // Retezec stringu
    char *string;
    // Alokovana pamet pro string
    size_t allocated_mem;
    // Aktualni delka stringu
    size_t length;
};

struct utils_string;
typedef struct utils_string utils_string_t;

/**
 * @brief Inicializace stringu
 * @return Ukazatel na alokovany utils_string
 */
utils_string_t *utils_string_init();
/**
 * @brief Likvidace stringu
 * @param us Ukazatel na utils_string
 */
void utils_string_destroy(utils_string_t *us);

/**
 * @brief Uklidi string
 * @param us Ukazatel na utils_string
 */
void utils_string_clear(utils_string_t *us);

/**
 * @brief Alokace nove pameti pro string
 * @param us Ukazatel na utils_string
 */
void utils_string_allocate_mem(utils_string_t *us);

/**
 * @brief Prida jeden znak do stringu v utils_string
 * @param us Ukazatel na utils_string
 * @param c Znak ktery se ma pridat
 */
void utils_string_add_char(utils_string_t *us, char c);

/**
 * @brief Vytvori novy retezec s obsahem utils_string a navrati jej
 * @param us Ukazatel na utils_string
 * @return Pozadovany novy retezec
 */
char *utils_string_copy(utils_string_t *us);

#endif