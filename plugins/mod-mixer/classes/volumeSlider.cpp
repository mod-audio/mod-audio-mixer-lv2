#include "volumeSlider.hpp"

VolumeSlider::VolumeSlider()
{
    coef = 0.0;
}

VolumeSlider::~VolumeSlider()
{
}

void VolumeSlider::setCoef(float level)
{
    if (level > 0.0)
        coef = pow(10.0f, (1 - level) * -45 / 20.0f);
    else
        coef = 0.0;
}

float VolumeSlider::getCoef()
{
    return coef;
}

