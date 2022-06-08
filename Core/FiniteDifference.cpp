#include "FiniteDifference.h"

#include <cmath>

using namespace FiniteDifference;


vec grad(double (*F)(const vec&), const vec& r)
{
    const double e = r.norm() * std::cbrt(std::numeric_limits<double>::epsilon());
    double Fx, Fy, Fz;
    Fx = (F(r + vec(e,0,0)) - F(r - vec(e,0,0))) / (2*e);
    Fy = (F(r + vec(0,e,0)) - F(r - vec(0,e,0))) / (2*e);
    Fz = (F(r + vec(0,0,e)) - F(r - vec(0,0,e))) / (2*e);
    return vec(Fx, Fy, Fz);
}

