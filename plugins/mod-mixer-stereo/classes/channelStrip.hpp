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
    void setMute(float level);
    void setPanning(float level);
    float getSample(int channel);
    void process(float input);
private:
    float samples[4] = {0.0, 0.0, 0.0, 0.0};
    float sample_channel;
    float gain;
    float alt_gain;
    float smooth_gain;
    float volume_level;
    float mute_level;
    float panning_level;

    VolumeSlider volumeSlider;
    Panning panner;
    OnePole onepole1;
    OnePole onepole2;
};

#endif // _H_CHANNEL
