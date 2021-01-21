//written by: Nigel Redmon
//source from: www.earlevel.com/main/2012/12/15/a-one-pole-filter/
//
//  OnePole.h
//

#ifndef OnePole_h
#define OnePole_h

#include <math.h>

class OnePole {
public:
    OnePole();
    OnePole(double Fc);
    ~OnePole();
    void setZ1(double z1);
    void setFc(double Fc);
    float process(float in);

protected:
    double a0, b1, z1;
};

#endif
