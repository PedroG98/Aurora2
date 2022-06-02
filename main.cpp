#include <iostream>
#include "Systems/SineWaveSystem.h"


// Input en cgs puro
int main()
{
    PointCharge q(1e-7, 1, vec(4, 0.0, 0.0), vec(0.0, 0.00005, 0.002));
    SineWave w(vec(0.0, 1e-10, 0.0), vec(0, 0.0, 0.0));

    //
    SineWaveSystem system(q, 1.6e5, w, "../data/External/SmallOmegaSmallAmplitude");
    system.run(500000, 0.1);

    return 0;

}
