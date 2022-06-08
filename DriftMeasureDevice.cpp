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
    if (!integrating)
        return;

    vec pos = pCharge->getPos();
    E = system->E(pos, pCharge);
    B = system->B(pos, pCharge);
    gradB = system->gradB(pos, pCharge);
    L = system->L(pos);
}


// Scale factors
void DriftMeasureDevice::adimensionalize(const DimensionSet& base)
{
    driftFactor = base.mass * M_C / (base.charge * base.B * base.time);
    frequencyFactor = base.time * base.charge * base.B / (base.mass * M_C);
}

// Does nothing
vec DriftMeasureDevice::EField(const vec& r, const double t) const { return vec(); }
vec DriftMeasureDevice::BField(const vec& r, const double t) const { return vec(); }


// Helper methods -------------------------------------------------------
double DriftMeasureDevice::Kparallel() const
{
    return pCharge->getMass() * pCharge->getVel().project(B).norm2() / 2.0;
}

double DriftMeasureDevice::gyroRadius() const
{
    vec v = pCharge->getVel();
    return (v - v.project(B)).norm() / std::abs(gyroFrequency());
}

vec DriftMeasureDevice::expectedDriftVel() const
{
    //revisar
    vec Bdrift = driftFactor * (pCharge->K() + Kparallel()) / (pCharge->getCharge() * std::pow(B.norm(), 3)) * cross(B, gradB);
    vec polDrift = cross(E, B) / (B.norm() * B.norm());
    return  Bdrift + polDrift;
}



// Drift calculation
void DriftMeasureDevice::move(const double dt)
{
    // If not integrating, quits
    if (!integrating)
    {
        posTrail.push(pCharge->getPos());
        velTrail.push(pCharge->getVel());
        return;
    }

    vec pos = pCharge->getPos();
    vec vel = pCharge->getVel();

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

    // Valid if horizon crossed
    if (pos.z * posTrail.front().z <= 0.0)
        valid = true;

    // Calculate drifts:
    totalTime += dt;
    totalExpectedDrift += expectedDriftVel() * dt;
    totalDrift += vel * dt;
    totalEpsilon += epsilon();

    posTrail.push(pos);
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

double DriftMeasureDevice::getMeanEpsilon() const
{
    return totalEpsilon / totalTime;
}


bool DriftMeasureDevice::hasLooped() const
{
    return looped;
}

bool DriftMeasureDevice::isValid() const
{
    return valid;
}

// r / L
double DriftMeasureDevice::epsilon() const
{
    return gyroRadius() / L;
}


// Signals --------------------------------------------
void DriftMeasureDevice::startIntegrating()
{
    integrating = true;
    valid = false;

    // Reset
    totalTime = 0.0;
    totalEpsilon = 0.0;
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
