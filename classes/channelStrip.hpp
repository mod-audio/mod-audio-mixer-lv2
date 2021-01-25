#ifndef _H_CHANNEL_
#define _H_CHANNEL_

#include "onepole.hpp"
#include "panning.hpp"
#include "volumeSlider.hpp"

class ChannelStrip {
public:
    ChannelStrip();
    ~ChannelStrip();
    void setVolume(float level);
    void setMute(bool level);
    void setAlt(bool level);
    void setPanning(float level);
    bool getMute();
    bool getAltChannel();
    float getSample(int channel);
    void process(float input);
private:
    float samples[4] = {0.0, 0.0, 0.0, 0.0};
    float gain;
    float altGain;
    float smoothGain = 0.0;
    float volumeLevel;
    float panningLevel;
    bool mute;
    bool altChannel;

    VolumeSlider volumeSlider;
    Panning panner;
    OnePole onepole1;
    OnePole onepole2;
};

#endif // _H_CHANNEL
