/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: my_malloc.h
 *
 */
#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

/**
* alokuje pamet a vrati ukazatel na zacatek pole
* po skonceni programu se pamet sama uvolni
* @param size velikost pole pro alokaci
* @return ukazatel na zacatek pole
*/
void *my_malloc(unsigned size);

#endif
