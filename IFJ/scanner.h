/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: scanner.h
 *
 */


#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

extern token_t token;


/**
 * @brief Zavola funkci utils_string_destroy pro uklid
 * @return -
 */
void cleanup_behind_scanner();


/**
 * @brief Inicializace scanneru - nastaveni nulovych hodnot cisel znaku a radku, nastaveni vstupu, inicializace utils_string
 * @return -
 */
void init_scanner();


/**
 * @brief Umoznuje externi nastaveni vstupu pro scanner
 * @param Ukazatel na vstupni soubor
 * @return -
 */
void set_input_file(FILE *fh);


/**
 * @brief Funkce pro nacitani znaku ze stdin ktera prida EOL pred EOF, pokud tam meni
 * @return Nacteny znak
 */
int get_char_from_stdin();


/**
 * @brief Nacita radky dokud nenarazi na EOF
 * @return -
 */
void load_next_line();


/**
 * @brief Nacita znaky z line_bufferu dokud nenarazi na '\0'
 * @return Nacteny znak
 */
int get_next_char();


/**
 * @brief Pokud neni aktualne zpracovavany znak roven '\0' nebo EOF, tak se vrati o jeden znak zpet 
 * @param Znak ktery se ma vratit
 * @return -
 */
void unget_next_char(int c);


/**
 * @brief Vyprodukuje vystup scanneru - token
 * @return Jeden token (jeho hodnotu a tkn_type)
 */
token_t get_next_token();


/**
 * @brief Prevede jeden HEXA znak na hodnotu 0-15
 * @param Dekadicka hodnota ASCII znaku ktery se ma prevest
 * @return hondota 0-15
 */
int hexa_to_decimal(int letter);


/**
 * @brief Prevede vsechny HEXA znaky v poli na dekadicke cislo odpovidajici zadanemu znaku z ASCII tabulky
 * @return hondota 0-127
 */
int hexa_array_value();

#endif