/* Experimento 1
 * Dipolo fijo en centro con eje alineado en z
 * Campo externo dado por una onda sinusoidal
 */

#ifndef SINEWAVESYSTEM_H
#define SINEWAVESYSTEM_H

#include "ChargeDipoleSystem.h"
#include "../Core/IDynamicObject.h"
#include <fstream>


class SineWave : public IDynamicObject
{
    private:
        vec k;
        vec E0;
        double omega;

    public:
        SineWave(const vec& k, const vec& E0);
        SineWave(const SineWave& other);

        virtual vec EField(const vec& r, const double t) const;
        virtual vec BField(const vec& r, const double t) const;

        // Nothing
        virtual void setFields(const System* system) {}
        // Nothing
        virtual void move(const double dt) {}
        // Adimensionalization
        virtual void adimensionalize(const DimensionSet& base);

        // Getters
        double getOmega() const;
        vec getAmplitude() const;
};


class SineWaveSystem : public ChargeDipoleSystem
{
    private:
        // Wave field
        SineWave wave;

        // Get from current gyroperiod and wave
        virtual double getLocalTimeStep(const double factor_of_period) const override;

        // Auxilliary methods

        // Write wave info
        virtual void writeData(const unsigned int which) override;

    public:
        // Charge and wave parameters. Improve on polarization and stuff maybe
        SineWaveSystem(const PointCharge& charge, const double magneticMoment, const SineWave& wave, const std::string& output_dir);

        // Adds sinewave field
        virtual vec E(const vec& r, const Particle* exclude=nullptr) const override;
        virtual vec B(const vec& r, const Particle* exclude=nullptr) const override;
};

#endif // SINEWAVESYSTEM_H
