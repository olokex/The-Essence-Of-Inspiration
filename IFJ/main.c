/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: main.c
 *
 */
#include "generate.h"
#include "scanner.h"
#include "syntax.h"

int main(void)
{
    init_scanner();
    set_input_file(stdin);
    parse();
    return 0;
}