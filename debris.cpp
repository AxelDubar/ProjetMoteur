#include "debris.h"

Debris::Debris()
{
    velocity=QVector2D(5*(rand()-RAND_MAX/2)/RAND_MAX,5*(rand()-RAND_MAX/2)/RAND_MAX);
    nbBounce=3;
}
