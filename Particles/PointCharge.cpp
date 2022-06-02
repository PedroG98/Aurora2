#include "PointCharge.h"

#include "../Core/System.h"     // Forward declared
#include <cmath>


// PointCharge ------------------------------------------------------------------------------------------------------
// Initialized as garbage, then set once
double PointCharge::sBorisFactor = 0;

PointCharge::PointCharge(const double mass, const double charge, const vec& pos0, const vec& vel0) :
    Particle(mass, pos0, vel0), charge(charge)
{}

PointCharge::PointCharge(const PointCharge& other) :
    Particle(other.mass, other.pos, other.vel), charge(other.charge), E(other.E), B(other.B)
{}


void PointCharge::setFields(const System* system)
{
    E = system->E(pos, this);
    B = system->B(pos, this);
}


// Adimensionalizes charge and sets factor
void PointCharge::adimensionalize(const DimensionSet& base)
{
    Particle::adimensionalize(base);
    if (sBorisFactor == 0)
        sBorisFactor = base.charge * base.B * base.time / (base.mass * M_C);

    charge /= base.charge;
}


// Boris algorithm
void PointCharge::move(const double dt)
{
    // Dimensional + Utility constant
    double K = sBorisFactor * charge * dt / (2 * mass);

    // Step 1 (v minus)
    vec dv = K * E;
    vec vm = vel + dv;

    // Step 2 (v plus)
    vec c = K * B;
    double h = 2.0 / (1 + c.norm2());
    vec vp = vm + cross((vm + cross(vm, c)), c) * h;

    // Step 3 (v_k+1)
    vel = vp + dv;

    // Position
    pos += dt * vel;
}



// Generate point E field (B later)
vec PointCharge::EField(const vec& r, const double t) const
{
    vec R = r - pos;
    return charge * R.normalized() / R.norm2();
}

double PointCharge::getCharge() const
{
    return charge;
}
