#include "random.h"

#include <stdlib.h>
#include <time.h>

namespace Random
{
void init()
{
    srand(time(NULL));
    random_engine.seed(std::random_device()());
    distribution
}
int integer_between(int min, int max)
{
    return rand() % ((max - min) + 1) + min;
}
double double_between(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}
} // namespace Random