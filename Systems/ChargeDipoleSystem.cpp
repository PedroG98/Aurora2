#include "ChargeDipoleSystem.h"
#include <cmath>


ChargeDipoleSystem::ChargeDipoleSystem(const PointCharge& q, const MagneticDipole& m, const std::string& output_dir) :
    System(output_dir), charge(q), dipole(m), driftDevice(charge, 2)
{
    objects.push_back(&charge);
    objects.push_back(&dipole);
    objects.push_back(&driftDevice);

    if (charge.getVel().z == 0)
        throw std::invalid_argument("The charge's initial velocity cannot be 0");

    maxHeight = 0.0;

    // Open files
    generalData.open(outputDir + "/General.dat");
    trajectory.open(outputDir + "/trajectory.dat");
    driftData.open(outputDir + "/Drift.dat");

    trajectory << "x\ty\tz" << std::endl;
    driftData << "Expected drift\tMeasured drift\tEpsilon" << std::endl;
}


// Destructor closes files
ChargeDipoleSystem::~ChargeDipoleSystem()
{
    generalData.close();
    trajectory.close();
    driftData.close();
}



// Generates units and passes table to particles. Generated right before running
void ChargeDipoleSystem::adimensionalize()
{
    units.mass = 1;

    vec B0 = B(charge.getPos(), &charge);
    units.B = B0.norm();
    units.time = 2 * M_PI * M_C * charge.getMass() / (charge.getCharge() * units.B);      // Initial gyroperiod
    units.length = units.time / (2*M_PI) * charge.getVel().project(B0).norm();      // Initial gyroradius

    // Derived units. Check notes
    units.speed = units.length / units.time;
    units.E = units.B * units.speed / M_C;
    units.charge = units.length * units.length * units.E;
    units.magneticMoment = units.B * std::pow(units.length, 3);

    // Pass this struct to all particles
    for (IDynamicObject* p : objects)
        p->adimensionalize(units);
}


// Factor of gyroperiod
double ChargeDipoleSystem::getLocalTimeStep(const double factor_of_period) const
{
        return factor_of_period * 2 * M_PI / std::abs(driftDevice.gyroFrequency());
}


void ChargeDipoleSystem::setFields()
{
    charge.setFields(this);
    dipole.setFields(this);
    driftDevice.setFields(this);
}


void ChargeDipoleSystem::move(const double dtau)
{
    charge.move(dtau);
    dipole.move(dtau);
    driftDevice.move(dtau);
}


// Total time in seconds. step_precision is fraction of gyroperiod wanted as time step
void ChargeDipoleSystem::run(const double total_time, const double step_precision)
{
    adimensionalize();
    // Set total time
    double total_tau = total_time / units.time;

    // Transient phase ---
    // 1 full 2nd invariant
    // Dynamical reduction of time step until minimum achieved at maximal B
    // maxHeight calculated from z position
    unsigned int horizon_passes = 0;
    double z = charge.getPos().z;
    dt = getLocalTimeStep(step_precision);

    while (horizon_passes < 2)
    {
        setFields();
        dt = std::min(dt, getLocalTimeStep(step_precision));
        move(dt);
        time += dt;

        if (z == 0 || charge.getPos().z * z < 0)
            horizon_passes++;

        z = charge.getPos().z;
        maxHeight = std::max(maxHeight, std::abs(z));

        //writeData(1);

        if (time >= total_tau)
        {
            std::cout << "Time ran out before simulation finished calibrating! No data output" << std::endl;
            return;
        }
    }
    writeData(0);

    // Simulation phase
    // dt is determined, as is max height
    // To show: Trajectory + Drift + Angles between intersections with xy plane
    bool integrating = false;
    while (time < total_tau)
    {
        setFields();
        move(dt);
        time += dt;
        //writeData(1);

        // Drift integration while on 1/4 of maxheight
        if (!integrating && std::abs(charge.getPos().z) < maxHeight / 5)
        {
            integrating = true;
            driftDevice.startIntegrating();
        }
        // Stop signal
        if (integrating && std::abs(charge.getPos().z) >= maxHeight / 5 && driftDevice.hasLooped())
        {
            integrating = false;
            driftDevice.stopIntegrating();
            if (driftDevice.isValid())
                writeData(2);
        }

        // Drift integration one loop after

    }
}



// Writing to output files. Cringe
void ChargeDipoleSystem::writeData(const unsigned int which)
{
    switch (which)
    {
        // General
        case 0:
        {
            generalData << "Parameters:" << std::endl;
            generalData << "Magnetic moment: " << dipole.getMoment() << std::endl;
            generalData << "Particle mass: " << charge.getMass() << "\t Particle charge: " << charge.getCharge() << std::endl;
            generalData << "Wave parameters: " << std::endl;
            generalData << "Initial conditions: " << std::endl;
            generalData << "Units:" << std::endl;
            generalData << "Mass: " << units.mass << std::endl;
            generalData << "Length: " << units.length << std::endl;
            generalData << "Time: " << units.time << std::endl;
            generalData << "E: " << units.E << std::endl;
            generalData << "B: " << units.B << std::endl;
            generalData << "Speed: " << units.speed << std::endl;
            generalData << "Charge: " << units.charge << std::endl;
            generalData << "Magnetic moment: " << units.magneticMoment << std::endl;
            // Put transient stuff too
        }
        break;
        // Trajectory
        case 1:
            trajectory << charge.getPos() << std::endl;
        break;
        // Drift
        case 2:
        {
            driftData << driftDevice.getExpectedDrift() << '\t' << driftDevice.getMeasuredDrift() << '\t' << driftDevice.getMeanEpsilon() << std::endl;
        }
        break;
    }
}


// Calculate fields
vec ChargeDipoleSystem::E(const vec& r, const Particle* exclude) const
{
    vec e;
    if (exclude != &charge)
        e += charge.EField(r, time);
    if (exclude != &dipole)
        e += dipole.EField(r, time);
    return e;
}

vec ChargeDipoleSystem::B(const vec& r, const Particle* exclude) const
{
    vec b;
    if (exclude != &charge)
        b += charge.BField(r, time);
    if (exclude != &dipole)
        b += dipole.BField(r, time);
    return b;
}
