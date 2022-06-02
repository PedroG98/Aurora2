/* System with one charge and one dipole
 * Optimized method calls
 * Whether particles move, external fields are added and other things depends on implementation
 * Charge drift analysis through device
 */

#ifndef CHARGEDIPOLESYSTEM_H
#define CHARGEDIPOLESYSTEM_H

#include "../Core/System.h"
#include "../Particles/PointCharge.h"
#include "../Particles/MagneticDipole.h"
#include "../DriftMeasureDevice.h"



class ChargeDipoleSystem : public System
{
    protected:
        PointCharge charge;
        DriftMeasureDevice driftDevice;
        MagneticDipole dipole;

        virtual void adimensionalize();
        virtual double getLocalTimeStep(const double factor_of_period) const;

        // Dynamical parameters
        double dt;
        double maxHeight;

        // Updates all particles (dimensionless time)
        virtual void setFields();
        virtual void move(const double dtau);

        // Write relevant parameters + charge position + Drift comparison in different files
        std::ofstream generalData, trajectory, driftData;
        virtual void writeData(const unsigned int which);

    public:
        ChargeDipoleSystem(const PointCharge& q, const MagneticDipole& m, const std::string& output_dir);
        virtual ~ChargeDipoleSystem();

        // Defines actual running. Transient particle until all properties have been recovered
        virtual void run(const double total_time, const double step_precision);
        // Fields
        virtual vec E(const vec& r, const Particle* exclude=nullptr) const;
        virtual vec B(const vec& r, const Particle* exclude=nullptr) const;
};

#endif // CHARGEDIPOLESYSTEM_H
