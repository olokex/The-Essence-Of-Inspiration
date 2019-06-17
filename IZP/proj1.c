/*
 *  Soubor: proj1.c
 *  Datum: 28/10/2017
 *  Autor: Adam Sedláček, xsedla1e
 *  Projekt: projekt č.1 do IZP
 */

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINE_LEN 100
#define ASCII_SIZE 128
/*
 * Funkce nacte radek ze vstupu, ukonceny znakem \n,
 * do promenne line
 *
 * V pripade preteceni funkce docte radek a v promenne
 * overflow vraci hodnotu true
 *
 * Vraci pocet nactenych znaku
 */
int nacti_radek_velkym(char *line, bool *overflow) {
  int i = 0;
  int c;

  while ((c=getchar()) != '\n' && c != EOF) {
    if (i < MAX_LINE_LEN) {
      line[i] = toupper(c);
      i++;
    }
  }

  if (i == MAX_LINE_LEN) {
    *overflow = true;
    i--;
  } else {
    *overflow = false;
  }

  line[i] = '\0';

  return i;
}

/*
 * Funkce najde prvni lisici se znak mezi retezci
 * str_a a str_b a vrati jeho index
 *
 * v pripade shody vraci -1
 */
int najdi_odlisny_znak(char *str_a, char *str_b) {
  int i = 0;

  while (toupper(str_a[i]) == toupper(str_b[i]) && str_a[i] != '\0') {
    i++;
  }

  if (str_a[i] == '\0' && str_b[i] == '\0')
    return -1;

  return i;
}

/*
 * Funkce vypise vsechny znaky ktere jsou povoleny
 *
 */
void vypis_enable_charu (bool prepinac[ASCII_SIZE]) {
  printf("Enable: ");
  for (int i = 0; i < ASCII_SIZE; i++) {
    if (prepinac[i]) {
      putchar(i);
    }
  }
  printf("\n");
}

int main (int argc, char *argv[]){
  char radek[MAX_LINE_LEN];
  bool overflow = false;
  bool vypsano = false;
  char *vstup;
  bool abeceda[ASCII_SIZE] = { false };

  int shoda = 0;
  char mesto[MAX_LINE_LEN];

  if (argc == 1) {
    vstup = "";
  } else if (argc == 2) {
    vstup = argv[1];
  } else {
    fprintf(stderr, "Pouziti: %s [ADRESA]\n", argv[0]);
    return 1;
  }

  while (nacti_radek_velkym(radek, &overflow)) {
    int rozdil = najdi_odlisny_znak(vstup, radek);

    if (rozdil == -1) {
      shoda++;
      strcpy(mesto, radek);
    } else {
      if (vstup[rozdil] == '\0') {
        abeceda[(int)radek[rozdil]] = true;
        shoda++;
        strcpy(mesto, radek);
      }
    }

    if (overflow == true && vypsano == false) {
      fprintf(stderr, "Doslo k prekroceni maximalni delky radku, "
                      "nektere vysledky mohou byt oseknuty\n");
      vypsano = true;
    }
  }

  if (shoda == 1) {
    printf("Found: %s\n", mesto);
  } else if (shoda > 1) {
    vypis_enable_charu(abeceda);
  } else {
    printf("Not found\n");
  }

  return vypsano ? 1 : 0;
}