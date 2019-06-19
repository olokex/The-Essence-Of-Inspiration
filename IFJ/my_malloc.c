/**
 * Implementace překladače imperativního jazyka IFJ18
 *
 * Tým:
 *    Sedláček Adam        xsedla1e
 *    Kurka David          xkurka04
 *    Havlíček Lukáš       xhavli46
 *    Katrušák Jaroslav    xkatru00
 *
 * Soubor: my_malloc.c
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "my_malloc.h"

#define BASE_ARR_LENGTH 64 // zakladni delka pole

typedef struct {
        size_t size; // aktualni pocet ukazatelu v poli
        size_t capacity; // maximalni kapacita pole
        size_t **arr_ptr; // ukazatel na pole
} garbage_collector_t;

garbage_collector_t *trash; // ukazatel na garbage collector

int collector_initialized = 0; // prepinac ktery urcuje zda uz byl garbage collector v programu inicializovan

/**
* inicializuje strukturu garbage collector
* nastavi velikost pole ukazatelu na hodnotu BASE_ARR_LENGTH
*/
void garbage_collector_init();

/**
* uvolni vsechny alokovane pole v garbage collectoru + samotny garbage collector
*/
void my_free();

/**
* zvetsi velikost pole o BASE_ARR_LENGTH
* @return ukazatel na nove alokovane pole
*/
size_t **resize_malloc();

/**
* vypis chyboveho hlaseni pri neuspesne alokaci pameti
*/
void error_handle();

void garbage_collector_init()
{
	trash = malloc(sizeof(garbage_collector_t));
	if (!trash)
		error_handle();
	trash->arr_ptr = malloc(sizeof(size_t *) * BASE_ARR_LENGTH);
	if (!trash->arr_ptr)
	{
		free(trash);
		error_handle();
	}
	trash->size = 0;
	trash->capacity = BASE_ARR_LENGTH;
	atexit(my_free);
}

void my_free() {
	for (int i = 0; i < trash->size; i++)
	{
		if (trash->arr_ptr[i])
		{
			free(trash->arr_ptr[i]);
		}
		else
		{
			printf("double free prevented\n");
		}
	}
	free(trash->arr_ptr);
	free(trash);
}

size_t **resize_malloc()
{
	trash->capacity += BASE_ARR_LENGTH;
	size_t **resize = realloc(trash->arr_ptr, sizeof(size_t *) * trash->capacity);
	if (resize == NULL)
		error_handle();
	return resize;
}

void *my_malloc(unsigned size) {
	if (!collector_initialized)
	{
		garbage_collector_init();
		collector_initialized = 1;
	}
	void *tmp = (void *)malloc(size);
	if (tmp == NULL) error_handle();

	if (trash->size == trash->capacity) {	
		trash->arr_ptr = resize_malloc();
	}
	trash->arr_ptr[trash->size++] = tmp;
	return tmp;
}
void error_handle() {
        fprintf(stderr, "Out of memory for malloc. Free'd all the trash\n"); 
        exit(1); 
}
