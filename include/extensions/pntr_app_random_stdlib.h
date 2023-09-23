/**
 * @file Uses stdlib.h's rand() for pntr_app_random()
 *
 * @see pntr_app_random()
 * @see pntr_app_random_seed()
 */

#include <stdlib.h>
#include <time.h>

PNTR_APP_API int pntr_app_random(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

PNTR_APP_API void pntr_app_random_seed(unsigned int seed) {
    if (seed != 0) {
        srand(seed);
    }
    else {
        srand((unsigned int)time(NULL));
    }
}
