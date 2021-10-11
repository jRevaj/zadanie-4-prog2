#include "rng.h"

static long long unsigned int SEED = 0x1; // seed generatora

void srnd(int seed) {
    SEED = seed;
}

int rnd(int from, int to ) {
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}
