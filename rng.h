#ifndef Z4_RNG_H
#define Z4_RNG_H

// Vsetky generovane cisla su mensie ako R_MAX.
#define R_MAX 2147483647

// Funkcia na inicializaciu generatora pseudo-nahodnych cisiel.
void srnd(int seed);

// Funkcia na generovanie pseudo-nahodneho cisla v intervale <from,to>.
int rnd(int from, int to);

#endif //Z4_RNG_H
