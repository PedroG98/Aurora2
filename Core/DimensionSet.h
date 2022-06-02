/* Unit list
 */

#ifndef DIMENSIONSET_H
#define DIMENSIONSET_H

// c in cgs
constexpr double M_C = 2.9979e10;


struct DimensionSet {
    double time;
    double length;
    double speed;
    double mass;
    double E;
    double B;
    double charge;
    double magneticMoment;

    // Default starts with 1 everywhere
    DimensionSet();
};


#endif // DIMENSIONSET_H
