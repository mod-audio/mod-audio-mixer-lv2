#include "panning.hpp"

#define PI 3.14159265
#define PI_OVER_TWO 1.570796325
#define PI_OVER_HUNDREDEIGHTY 0.017453292500000002

Panning::Panning()
{
    samples[0] = 0.0;
    samples[1] = 0.0;
}

Panning::~Panning()
{
}

void Panning::setAngle(float angle)
{
    this->angle = angle;
    calcPanning(angle);
}

float Panning::calcSin(float angle)
{
    return angle - pow(angle, 3)/6 + pow(angle, 5)/120 - pow(angle, 7)/5040 + pow(angle, 9)/362880;
}

void Panning::calcPanning(float angle)
{
    left = calcSin(angle*PI_OVER_HUNDREDEIGHTY + PI_OVER_TWO);
    right = calcSin(angle*PI_OVER_HUNDREDEIGHTY);
}

void Panning::process(float sample)
{
    samples[0] = left * sample;
    samples[1] = right * sample;
}

float Panning::getSample(int channel)
{
    return samples[channel];
}
