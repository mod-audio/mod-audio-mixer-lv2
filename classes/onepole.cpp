//written by: Nigel Redmon
//source from: www.earlevel.com/main/2012/12/15/a-one-pole-filter/
//
//  OnePole.h
//

#include <stdio.h>
#include "onepole.hpp"

OnePole::OnePole()
{
    a0 = 1.0; b1 = 0.0; z1 = 0.0;
}

OnePole::OnePole(double Fc)
{
    z1 = 0.0; setFc(Fc);
}

OnePole::~OnePole()
{
}

void OnePole::setZ1(double z1)
{
    this->z1 = z1;
}

void OnePole::setFc(double Fc) {
    b1 = exp(-2.0 * M_PI * Fc);
    a0 = 1.0 - b1;
}


float OnePole::process(float in) {
    return z1 = in * a0 + z1 * b1;
}
