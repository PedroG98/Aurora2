/*
 * Base class for all objects, includes transform and mass
 * Derived objects add charge, moment, ...
 * All particles define evolution/rescaling in some way
 * and store information about the external E/B fields
 */


#ifndef PARTICLE_H
#define PARTICLE_H

#include "../Core/Vec.h"
#include "../Core/DimensionSet.h"
#include "../Core/IDynamicObject.h"

#include <string>
#include <fstream>


// Forward declaration for field settings
class System;

class Particle : public IDynamicObject
{
    protected:
        // Kinematics
        double mass;
        vec pos;
        vec vel;

    public:
        Particle(const double mass, const vec& pos0, const vec& vel0);

        // Pass to read field values as needed
        virtual void setFields(const System* system) = 0;

        // Movement algorithm
        virtual void move(const double dt) = 0;
        // Adimensionalization
        virtual void adimensionalize(const DimensionSet& base);

        // Generate fields at r (Empty by default)
        virtual vec EField(const vec& r, const double t = 0) const;
        virtual vec BField(const vec& r, const double t = 0) const;

        // Getters
        vec getPos() const;
        vec getVel() const;
        double getMass() const;

        // Analysis
        double K() const;
};




#endif // PARTICLE_H
