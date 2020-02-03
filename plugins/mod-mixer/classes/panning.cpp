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
}

void Panning::process(float sample)
{
    samples[0] = sqrt(2)/2.0 * (cos(angle) - sin(angle)) * sample;
    samples[1] = sqrt(2)/2.0 * (cos(angle) + sin(angle)) * sample;
}

float Panning::getSample(int channel)
{
    return samples[channel];
}
