#include "channelStrip.hpp"

ChannelStrip::ChannelStrip(int reduction)
{
    sample_channel = 0.0;
    gain = 0.0;
    alt_gain = 0.0;
    smooth_gain = 0.0;
    volume_level = 0.0;
    mute_level = 0.0;
    panning_level = 0.0;

    onepole1.setFc(10.0/(48000.0/(float)reduction));
    onepole2.setFc(10.0/(48000.0/(float)reduction));
}

ChannelStrip::~ChannelStrip()
{
}

void ChannelStrip::setVolume(float level)
{
    volume_level = level;
    volumeSlider.setCoef(volume_level);
}

void ChannelStrip::setMute(float level)
{
    mute_level = level;
}

void ChannelStrip::setPanning(float level)
{
    panning_level = level;
    panner.setAngle(onepole2.process(panning_level));
}

void ChannelStrip::updateParameters()
{
    float gain = volumeSlider.getCoef();

    if (mute_level == 1.0) {
        gain= 0.0;
        alt_gain = (alt_gain < volumeSlider.getCoef()) ? alt_gain + 0.001 : volumeSlider.getCoef();
    } else {
        alt_gain = (alt_gain > 0.0) ? alt_gain - 0.001 : 0.0;
    }

    smooth_gain = onepole1.process(gain);

    panner.setAngle(onepole2.process(panning_level));
}

float ChannelStrip::getSample(int channel)
{
    return samples[channel];
}


void ChannelStrip::process(float input, int channel)
{
    float sample = input;

    panner.process(sample);

    samples[channel] = panner.getSample(channel) * smooth_gain;
    samples[2 + channel] = panner.getSample(channel) * alt_gain;
}

