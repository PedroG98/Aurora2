#include "DriftMeasureDevice.h"

#include <cmath>


DriftMeasureDevice::DriftMeasureDevice(const PointCharge& charge, const unsigned int trail_size) :
    pCharge(&charge), integrating(false), loops(0), relativeAngle(0.0), looped(false),
    posTrail(trail_size), velTrail(trail_size)
{
    driftFactor = 1.0;
    frequencyFactor = 1.0;
}


// Reads fields
void DriftMeasureDevice::setFields(const System* system)
{
    vec pos = pCharge->getPos();
    E = system->E(pos, pCharge);
    B = system->B(pos, pCharge);
    gradB = system->gradB(pos, pCharge);
}


// Scale factors
void DriftMeasureDevice::adimensionalize(const DimensionSet& base)
{
    driftFactor = base.mass * M_C / (base.charge * base.B * base.time);
    frequencyFactor = base.charge * base.B / (base.mass * M_C);
}

// Does nothing
vec DriftMeasureDevice::EField(const vec& r, const double t) const { return vec(); }
vec DriftMeasureDevice::BField(const vec& r, const double t) const { return vec(); }


// Helper methods
double DriftMeasureDevice::Kparallel() const
{
    return pCharge->getMass() * pCharge->getVel().project(B).norm2() / 2.0;
}
vec DriftMeasureDevice::expectedDriftVel() const
{
    //revisar
    vec b = B.normalized();
    return driftFactor * (pCharge->K() + Kparallel()) / (pCharge->getCharge() * std::pow(B.norm(), 3)) * cross(b, gradB);
}

// Drift calculation
void DriftMeasureDevice::move(const double dt)
{
    vec vel = pCharge->getVel();
    // Calcular extensión máxima en z (transiente) previo
    // Después integrar entre fracción de extensión (~5 oscilaciones)

    // If integrating, drifts
    if (integrating)
    {
        // Get angle change from change in v (tangent to path)
        vec tprev = velTrail.front().normalized();
        vec t = vel.normalized();
        // a dot b = |a||b|cos theta
        relativeAngle += std::acos(tprev * t);

        // Loop
        looped = relativeAngle > 2 * M_PI;
        if (looped)
        {
            relativeAngle -= 2*M_PI;
            loops++;
        }

        // Calculate drifts:
        totalExpectedDrift += expectedDriftVel() * dt;
        totalDrift += vel * dt;
    }

    // Extend trails
    posTrail.push(pCharge->getPos());
    velTrail.push(vel);
}



// API -----------------------------------
// Negative sign for direction
double DriftMeasureDevice::gyroFrequency() const
{
    return -frequencyFactor * pCharge->getCharge() * B.norm() / pCharge->getMass();
}



double DriftMeasureDevice::getMeasuredDrift() const
{
    // Project onto theta and remove last element
    vec pos = pCharge->getPos();
    vec theta = vec(-pos.y, pos.x, 0.0);
    return totalDrift.project(theta).norm();
}

double DriftMeasureDevice::getExpectedDrift() const
{
    return totalExpectedDrift.norm();
}

bool DriftMeasureDevice::hasLooped() const
{
    return looped;
}


// Signals
void DriftMeasureDevice::startIntegrating()
{
    integrating = true;

    // Reset
    totalDrift = vec();
    totalExpectedDrift = vec();
    relativeAngle = 0.0;
    loops = 0;
    looped = false;
}

void DriftMeasureDevice::stopIntegrating()
{
    integrating = false;
}
