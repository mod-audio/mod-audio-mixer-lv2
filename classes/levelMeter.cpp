#include "levelMeter.hpp"

LevelMeter::LevelMeter()
{

}

LevelMeter::~LevelMeter()
{

}

void LevelMeter::setSampleRate(float sampleRate)
{
    this->sampleRate = sampleRate;
}

float LevelMeter::process(float input)
{
    return level + fabs(input);
}
