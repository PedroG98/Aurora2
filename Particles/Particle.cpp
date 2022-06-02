#include "Particle.h"
#include "../Core/System.h"

#include <iomanip>



Particle::Particle(const double mass, const vec& pos0, const vec& vel0) :
    mass(mass), pos(pos0), vel(vel0)
{}


// Adimensionalization of mass, position and speed
void Particle::adimensionalize(const DimensionSet& base)
{
    mass /= base.mass;
    pos /= base.length;
    vel /= base.speed;
}

// Generate fields at r (Empty)
vec Particle::EField(const vec& r, const double t) const
{
    return vec(0.0, 0.0, 0.0);
}
vec Particle::BField(const vec& r, const double t) const
{
    return vec(0.0, 0.0, 0.0);
}


// Getters
vec Particle::getPos() const
{
    return pos;
}
vec Particle::getVel() const
{
    return vel;
}
double Particle::getMass() const
{
    return mass;
}

// Analysis
double Particle::K() const
{
    return mass * vel.norm2() / 2.0;
}


