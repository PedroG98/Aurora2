/*
 *
 *
 */

#ifndef FINITEDIFFERENCE_H
#define FINITEDIFFERENCE_H

#include "Vec.h"

namespace FiniteDifference
{
    vec grad(double (*scalar)(const vec&), const vec& r);
    //vec grad(double (*scalar)(double,vec), const double t, const vec& r);
}




#endif // FINITEDIFFERENCE_H
