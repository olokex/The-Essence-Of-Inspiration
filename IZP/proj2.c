
/*
 * Soubor: proj2.c
 * Datum: 11/11/2017
 * Autor: Adam SedlĂĄÄek, xsedla1e
 * Projekt: projekt Ä.2 do IZP
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define IT 10 // dostatecny pocet iteraci pro zlomky
              // dostatecny pocet by mel byt i 9

double taylor_tan(double x, unsigned int n) {
  long int exp[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582,
                         443861162, 18888466084, 113927491862, 58870668456604};

  long int jmenovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875,
                        10854718875, 1856156927625, 194896477400625,
                        49308808782358125, 3698160658676859375};
  double doc = x;
  double tt = x;
  int l = (int)n;
  for (int i = 1; i < l; i++) {
    doc = doc * x * x;
    tt = tt + (doc * exp[i]) / jmenovatel[i];
  }
  return tt;
}

double cfrac_tan(double x, unsigned int n) {
  int k = n * 2 - 1;
  double vysled = 0;
  for (int i = n; i > 1; i--) {
    vysled = (x*x)/(k-vysled);
    k -= 2;
  }
  return x/(1-vysled);
}

bool jf(char *arg, double *d) { // Je Float
  char *endptr;
  *d = strtod(arg, &endptr);
  return !(*endptr == '\0');
}

bool jc(char *arg, int *num) { // Je Cislo
  char *endptr;
  *num = (int)strtol(arg, &endptr, 10);
  return !(*endptr == '\0');
}

void chyba() {
  fprintf(stderr, "Zkontroluj znova zadane hodnoty\n");
}

bool over_uhly(double a, double b) {
 return a > 0 && a <= 1.4 && b > 0 && b <= 1.4;
}

bool over_uhel(double a) {
  return a > 0 && a <= 1.4;
}

bool over_vysku(double v) {
  return !(v > 0 && v <= 100);
}

int main(int argc, char *argv[]) {
  double vyska = 1.5; // v metrech
  if (argc == 2 && strcmp("--help", argv[1]) == 0) {
    printf("1. pouziti: %s --tan A N M\n", argv[0]);
    printf("2. pouziti: %s [-c X] -m A [B]\n", argv[0]);
  } else if (argc == 5 && strcmp("--tan", argv[1]) == 0) {
      double u;
      int n, m;
      if (jf(argv[2], &u) || jc(argv[3], &n) || jc(argv[4], &m)) {
        chyba();
        return 1;
      }
      if (!((n > 0) && (m >= n) && (m < 14))) {
        fprintf(stderr, "Neplatny rozsah\n");
        return 1;
      }
      if (isnan(u) || fabs(isinf(u))) {
        fprintf(stderr, "Zadavas neplatne hodnoty\n");
        return 1;
      }
      double M = tan(u);
      double T, TE, C, CE;
      for (;n <= m; n++) {
        T = taylor_tan(u, n);
        TE = fabs(T-M);
        C = cfrac_tan(u, n);
        CE = fabs(M-C);
        printf("%d %e %e %e %e %e\n", n, M, T, TE, C, CE);
      }
  } else if (argc == 4 &&strcmp("-m", argv[1]) == 0) {
      double alfa, beta;
      if (jf(argv[2], &alfa) || jf(argv[3], &beta)) {
        chyba();
        return 1;
      }
      if (over_uhly(alfa, beta)) {
        double vzdalenost = vyska / cfrac_tan(alfa, IT);
        double vyska_objektu = vzdalenost * cfrac_tan(beta, IT) + vyska;
        printf("%.10e\n", vzdalenost);
        printf("%.10e\n", vyska_objektu);
      } else {
        chyba();
        return 1;
      }
  } else if (argc == 6 && strcmp("-c", argv[1]) == 0 && strcmp("-m", argv[3]) == 0) {
      double vou, alfa, beta; // vou - Vyska Od Uzivatele
      if (jf(argv[2], &vou) || jf(argv[4], &alfa) || jf(argv[5], &beta)) {
        chyba();
        return 1;
      }
      if (over_vysku(vou)) {
        fprintf(stderr, "Zadej vysku v rozsahu 0 < X <= 100 metru\n");
        return 1;
      }
      if (over_uhly(alfa, beta)) {
        double vzdalenost = vou / cfrac_tan(alfa, IT);
        double vyska_objektu = vzdalenost * cfrac_tan(beta, IT) + vou;
        printf("%.10e\n", vzdalenost);
        printf("%.10e\n", vyska_objektu);
      } else {
        chyba();
        return 1;
      }
  } else if (argc == 3 && strcmp("-m", argv[1]) == 0) {
      double alfa;
      if (jf(argv[2], &alfa)) {
        chyba();
        return 1;
      }
      if (over_uhel(alfa)) {
        printf("%.10e\n", vyska / cfrac_tan(alfa, IT));
      } else {
        chyba();
        return 1;
      }
  } else if (argc == 5 && strcmp("-c", argv[1]) == 0 && strcmp("-m", argv[3]) == 0){
    double alfa, vou;
    if (jf(argv[2], &vou) || jf(argv[4], &alfa)) {
      chyba();
      return 1;
    }
    if (!over_uhel(alfa)) {
      chyba();
      return 1;
    }
    if (over_vysku(vou)) {
      fprintf(stderr, "Zadej vysku v rozsahu 0 < X <= 100 metru\n");
      return 1;
    }
    double vzdalenost = vou / cfrac_tan(alfa, IT);
    printf("%.10e\n", vzdalenost);
  } else {
    fprintf(stderr, "Zadej: %s --help\n", argv[0]);
    return 1;
  }
  return 0;
}