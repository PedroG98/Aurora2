#include <iostream>
#include <filesystem>
#include "Systems/SineWaveSystem.h"


// Input en cgs puro
int main()
{
    for (double vperp = 0.00002; vperp <= 0.0002; vperp += 0.00002)
    {
        PointCharge q(1e-7, 1, vec(4, 0.0, 0.0), vec(0.0, vperp, 0.002));
        SineWave w(vec(0.0, 1e-10, 0.0), vec(0, 0.0, 0.0));

        std::string dir = "../data/Drift/" + std::to_string(vperp);
        std::filesystem::create_directory(dir);
        SineWaveSystem system(q, 1.6e5, w, dir);
        system.run(500000, 0.1);
    }

    return 0;

}
