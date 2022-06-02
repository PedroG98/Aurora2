TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Core/DimensionSet.cpp \
        Particles/Particle.cpp \
        Core/System.cpp \
        Core/Vec.cpp \
        DriftMeasureDevice.cpp \
        Particles/MagneticDipole.cpp \
        Particles/PointCharge.cpp \
        Systems/ChargeDipoleSystem.cpp \
        Systems/SineWaveSystem.cpp \
        main.cpp

HEADERS += \
    Core/CircularBuffer.h \
    Core/DimensionSet.h \
    Core/IDynamicObject.h \
    Particles/Particle.h \
    Core/System.h \
    Core/Vec.h \
    DriftMeasureDevice.h \
    Particles/MagneticDipole.h \
    Particles/PointCharge.h \
    Systems/ChargeDipoleSystem.h \
    Systems/SineWaveSystem.h

DISTFILES += \
    TODO.txt
