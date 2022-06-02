#include "SineWaveSystem.h"
#include <cmath>
#include <algorithm>


// Sine wave field ----------------------------------------------------------------------------------------------
SineWave::SineWave(const vec& k, const vec& E0) :
    k(k), E0(E0), omega(M_C * k.norm())
{}

SineWave::SineWave(const SineWave& other) :
    k(other.k), E0(other.E0), omega(other.omega)
{}


vec SineWave::EField(const vec& r, const double t) const
{
    return E0 * std::cos(r * k - omega * t);
}

vec SineWave::BField(const vec& r, const double t) const
{
    return cross(k, E0) * std::cos(r * k - omega * t);
}

double SineWave::getOmega() const
{
    return omega;
}

vec SineWave::getAmplitude() const
{
    return E0;
}

void SineWave::adimensionalize(const DimensionSet& base)
{
    omega *= base.time;
    k *= base.length;
    E0 /= base.E;
}


// System --------------------------------------------------------------------------------------------------------
SineWaveSystem::SineWaveSystem(const PointCharge& charge, const double magneticMoment, const SineWave& wave, const std::string& output_dir) :
    ChargeDipoleSystem(charge, MagneticDipole(0.0, vec(0.0, 0.0, magneticMoment), vec(), vec()), output_dir),
    wave(wave)
{
    objects.push_back(&this->wave);
}



vec SineWaveSystem::E(const vec& r, const Particle* exclude) const
{
    return ChargeDipoleSystem::E(r, exclude) + wave.EField(r, time);
}
vec SineWaveSystem::B(const vec& r, const Particle* exclude) const
{
    return ChargeDipoleSystem::B(r, exclude) + wave.BField(r, time);
}


double SineWaveSystem::getLocalTimeStep(const double factor_of_period) const
{
    // Minimum between wave- and gyro-period * factor
    return factor_of_period * 2 * M_PI / std::max(std::abs(driftDevice.gyroFrequency()), wave.getOmega() * units.time);
}



// Write field data
void SineWaveSystem::writeData(const unsigned int which)
{
    ChargeDipoleSystem::writeData(which);

    if (which == 0)
    {
        generalData << "\tOmega = " << wave.getOmega();
        generalData << "\tE0 = " << wave.getAmplitude();
    }
}
