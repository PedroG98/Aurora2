#include "MagneticDipole.h"


MagneticDipole::MagneticDipole(const double mass, const vec& moment, const vec& pos0, const vec& vel0, const std::string& name) :
    Particle(mass, pos0, vel0), moment(moment)
{}

MagneticDipole::MagneticDipole(const MagneticDipole& other) :
    Particle(other.mass, other.pos, other.vel), moment(other.moment)
{

}


// Generate dipole B Field
vec MagneticDipole::BField(const vec& r, const double t) const
{
    vec R = r - pos;
    double u = R.norm();
    vec Rhat = R / u;
    return (Rhat * 3.0 * (Rhat*moment) - moment) / (u*u*u);
}

// No reading (yet)
void MagneticDipole::setFields(const System* system) {}
// No movement (yet)
void MagneticDipole::move(const double dt) {}

// Calls base, then: moment
void MagneticDipole::adimensionalize(const DimensionSet& base)
{
    Particle::adimensionalize(base);
    moment /= base.magneticMoment;
}


vec MagneticDipole::getMoment() const
{
    return moment;
}
