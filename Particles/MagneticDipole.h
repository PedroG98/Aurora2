/*
 * Magnetic dipole object
 * Derives from particle
 * Movement implementation in the future
 */

#ifndef MAGNETICDIPOLE_H
#define MAGNETICDIPOLE_H

#include "../Core/Vec.h"
#include "Particle.h"


class MagneticDipole : public Particle
{
    protected:
        vec moment;

    public:
        MagneticDipole(const double mass, const vec& moment, const vec& pos0, const vec& vel0, const std::string& name = "");
        MagneticDipole(const MagneticDipole& other);

        // Generate dipole B Field
        virtual vec BField(const vec& r, const double t) const override;

        // Nothing (yet)
        virtual void setFields(const System* system) override;

        // No movement (yet)
        virtual void move(const double dt) override;
        // Adimensionalization
        virtual void adimensionalize(const DimensionSet& base) override;

        vec getMoment() const;
};

#endif // MAGNETICDIPOLE_H
