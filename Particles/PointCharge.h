/* PointCharge
 * DriftParticle handles numerical comparison with guiding center motion
 */

#ifndef POINTCHARGE_H
#define POINTCHARGE_H

#include "Particle.h"



class PointCharge : public Particle
{
    protected:
        double charge;

        // Stores E,B fields at position
        vec E;
        vec B;

        // Boris constant
        static double sBorisFactor;

    public:
        PointCharge(const double mass, const double charge, const vec& pos0, const vec& vel0);
        PointCharge(const PointCharge& other);

        // Reads pure fields
        virtual void setFields(const System* system);

        // Boris algorithm
        virtual void move(const double dt);
        // Adimensionalizes charge
        virtual void adimensionalize(const DimensionSet& base);

        // Generate point E field (B later)
        virtual vec EField(const vec& r, const double t = 0) const;

        // Getter
        double getCharge() const;
};


#endif // POINTCHARGE_H
