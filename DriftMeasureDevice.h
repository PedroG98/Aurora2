#ifndef DRIFTMEASUREDEVICE_H
#define DRIFTMEASUREDEVICE_H

#include "Particles/PointCharge.h"

#include "Core/Vec.h"
#include "Core/CircularBuffer.h"
#include "Core/IDynamicObject.h"



// Include drift calculation methods here, remove DriftParticle
class DriftMeasureDevice : public IDynamicObject
{
    private:
        // Read only reference to point charge
        const PointCharge* pCharge;

        // Stored fields
        vec E, B;
        vec gradB;
        double L;

        // Scale factors
        double driftFactor;
        double frequencyFactor;

        // Drift control
        double totalTime;
        vec totalDrift;
        vec totalExpectedDrift;
        double totalEpsilon;

        bool valid;
        bool integrating;
        unsigned int loops;     // Claramente no generales al rotar la particula, en torno a z, pero casi constantes dentro de la 2da invariante
        double relativeAngle;
        bool looped;

        // Trails
        CircularBuffer<vec> posTrail;
        CircularBuffer<vec> velTrail;

        // Helper methods
        double Kparallel() const;
        double gyroRadius() const;
        vec expectedDriftVel() const;

    public:
        DriftMeasureDevice(const PointCharge& charge, const unsigned int trail_size);

        // Reads B and gradient
        virtual void setFields(const System* system);
        // Calculates drifts
        virtual void move(const double dt);
        // Scale factors
        virtual void adimensionalize(const DimensionSet& base);

        // Does nothing
        virtual vec EField(const vec& r, const double t = 0) const;
        virtual vec BField(const vec& r, const double t = 0) const;

        // API
        double gyroFrequency() const;
        double getMeasuredDrift() const;
        double getExpectedDrift() const;
        double getMeanEpsilon() const;

        bool hasLooped() const;
        bool isValid() const;
        // Get error
        double epsilon() const;

        // Signals
        void startIntegrating();
        void stopIntegrating();
};

#endif // DRIFTMEASUREDEVICE_H
