/* Contains particles and manages interactions, scaling
 * and controls program flow
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include "Vec.h"
#include "DimensionSet.h"
#include <vector>
#include <string>


class IDynamicObject;

class System
{
    protected:
        // Inner lab time
        double time;
        // Convenient but not super efficient array. Modification depends on child implementation
        std::vector<IDynamicObject*> objects;
        // Units
        DimensionSet units;

        // Helper methods
        virtual void adimensionalize() = 0;
        virtual double getLocalTimeStep(const double factor_of_period) const = 0;

        // Updates all particles (dimensionless time)
        virtual void setFields();
        virtual void move(const double dtau);

        // Writing data. Directory with possible multiple outputs
        const std::string outputDir;

    public:
        System(const std::string& output_dir);
        virtual ~System();

        // API virtual
        // Run (time in seconds)
        virtual void run(const double total_time, const double step_precision) = 0;
        // Fields
        virtual vec E(const vec& r, const IDynamicObject* exclude=nullptr) const;
        virtual vec B(const vec& r, const IDynamicObject* exclude=nullptr) const;

        // Utility
        vec gradB(const vec& r, const IDynamicObject* exclude=nullptr) const;
        double L(const vec& r) const;
};


#endif // SYSTEM_H
