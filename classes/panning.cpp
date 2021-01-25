#include "panning.hpp"

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
    left = calcSin((angle * (M_PI/180.0)) + (M_PI/2.0));
    right = calcSin(angle* (M_PI/180.0));
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
