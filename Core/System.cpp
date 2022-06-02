#include "System.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <limits>

#include "../Particles/PointCharge.h"


// Empty constructor
System::System(const std::string& output_dir) :
    time(0.0), units(), outputDir(output_dir)
{}


System::~System() {}


// Calculate fields (later filter through debye length)
vec System::E(const vec& r, const IDynamicObject* exclude) const
{
    vec e;
    for (const IDynamicObject* p : objects)
    {
        // No self interaction
        if (p != exclude)
            e += p->EField(r, time);
    }
    return e;
}

vec System::B(const vec& r, const IDynamicObject* exclude) const
{
    vec b;
    for (const IDynamicObject* p : objects)
    {
        // No self interaction
        if (p != exclude)
            b += p->BField(r, time);
    }
    return b;
}

vec System::gradB(const vec& r, const IDynamicObject* exclude) const
{
    const double e = r.norm() * std::cbrt(std::numeric_limits<double>::epsilon());
    double Bx, By, Bz;
    Bx = (B(r + vec(e,0,0), exclude).norm() - B(r - vec(e,0,0), exclude).norm()) / (2*e);
    By = (B(r + vec(0,e,0), exclude).norm() - B(r - vec(0,e,0), exclude).norm()) / (2*e);
    Bz = (B(r + vec(0,0,e), exclude).norm() - B(r - vec(0,0,e), exclude).norm()) / (2*e);
    return vec(Bx, By, Bz);
}


// Updates all particles (dimensionless time)
void System::setFields()
{
    for (auto& p : objects)
        p->setFields(this);    
}


void System::move(const double dtau)
{
    for (auto& p : objects)
        p->move(dtau);
}
