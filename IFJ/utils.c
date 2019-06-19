/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: utils.c
 *
 */


#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "my_malloc.h"


utils_string_t *utils_string_init() {
    utils_string_t *us;
 
    us = calloc(1, sizeof(*us));
    us->string = malloc(allocate);
    if (us->string == NULL) {
        fprintf(stderr, "Error during memory ALLOCATION in utils.");
    }
 
    *us->string = '\0';
    us->allocated_mem = 64;
    us->length = 0;
 
    return us;
}

void utils_string_destroy(utils_string_t *us) {
    if (us == NULL)
        return;
 
    free(us->string);
    free(us);
}

void utils_string_clear(utils_string_t *us) {
    if (us == NULL)
        return;
 
    us->string[0] = '\0';
    us->length = 0;
}

void utils_string_allocate_mem(utils_string_t *us) {
    if (us == NULL)
        return;
 
    us->allocated_mem += 64;
    us->string = realloc(us->string, us->allocated_mem);
    if (us->string == NULL) {
        fprintf(stderr, "Error occured in utils_string_allocate_mem");
    }
}

void utils_string_add_char(utils_string_t *us, const char c) {
    if (us == NULL)
        return;
 
    if (us->length+1 >= us->allocated_mem)
        utils_string_allocate_mem(us);
 
    us->string[us->length++] = c;
    us->string[us->length] = '\0';
}

char *utils_string_copy(utils_string_t *us) {
    if (us == NULL)
        return NULL;
 
    char *output_string = (char *) my_malloc(sizeof(char)*(us->length+1));
    if (output_string == NULL) {
        fprintf(stderr, "Error during memory REALLOCATION in utils ");
        return NULL;
    }
    memcpy(output_string, us->string, us->length+1);
 
    return output_string;
}
