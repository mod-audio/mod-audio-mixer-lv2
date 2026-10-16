#include "channelStrip.hpp"

ChannelStrip::ChannelStrip()
{
    gain = 0.0;
    altGain = 0.0;
    smoothGain = 0.0;
    volumeLevel = 0.0;
    panningLevel = 0.0;
    mute = false;
    altChannel = false;

    onepole1.setFc(10.0/48000.0);
    onepole2.setFc(10.0/48000.0);
}

ChannelStrip::~ChannelStrip()
{
}

void ChannelStrip::setVolume(float level)
{
    volumeLevel = level;
    volumeSlider.setCoef(volumeLevel);
}

void ChannelStrip::setMute(bool mute)
{
    this->mute = mute;
}

void ChannelStrip::setAlt(bool altChannel)
{
    this->altChannel = altChannel;
}

void ChannelStrip::setPanning(float panningLevel)
{
    this->panningLevel = panningLevel;
    panner.setAngle(onepole2.process(panningLevel));
}

float ChannelStrip::getVolume()
{
    return volumeLevel;
}

float ChannelStrip::getPanning()
{
    return panningLevel;
}

bool ChannelStrip::getMute()
{
    return mute;
}

bool ChannelStrip::getAltChannel()
{
    return altChannel;
}

float ChannelStrip::getSample(int channel)
{
    return samples[channel];
}

void ChannelStrip::process(float input)
{

    float sample = input;

    float gain = (mute || altChannel) ? 0.0 :volumeSlider.getCoef();

    if (altChannel && !mute) {
        altGain = (altGain < volumeSlider.getCoef()) ? altGain + 0.001 : volumeSlider.getCoef();
    } else {
        altGain = (altGain > 0.0) ? altGain - 0.001 : 0.0;
    }

    panner.setAngle(onepole2.process(panningLevel));
    smoothGain = onepole1.process(gain);

    panner.process(sample);

    samples[0] = panner.getSample(0) * smoothGain;
    samples[1] = panner.getSample(1) * smoothGain;
    samples[2] = panner.getSample(0) * altGain;
    samples[3] = panner.getSample(1) * altGain;
}

