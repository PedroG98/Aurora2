/* Interface for objects with dynamic behavior inside a system
 * Particles, external fields, measuring devices, ...
 */

#ifndef IDYNAMICOBJECT_H
#define IDYNAMICOBJECT_H

#include "DimensionSet.h"
#include "Vec.h"
#include "System.h"


class IDynamicObject
{
    public:
        // Read field values as needed
        virtual void setFields(const System* system) = 0;
        // Movement
        virtual void move(const double dt) = 0;
        // Adimensionalization
        virtual void adimensionalize(const DimensionSet& base) = 0;

        virtual vec EField(const vec& r, const double t) const = 0;
        virtual vec BField(const vec& r, const double t) const = 0;

};

#endif // IDYNAMICOBJECT_H
