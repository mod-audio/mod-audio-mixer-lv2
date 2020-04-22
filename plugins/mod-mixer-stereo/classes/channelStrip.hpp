#ifndef _H_CHANNEL_
#define _H_CHANNEL_

#include "onepole.hpp"
#include "panning.hpp"
#include "volumeSlider.hpp"

class ChannelStrip {
public:
    ChannelStrip(int reduction);
    ~ChannelStrip();
    void setVolume(float level);
    void setMute(float level);
    void setPanning(float level);
    void updateParameters();
    float getSample(int channel);
    void process(float input, int channel);
private:
    float samples[4] = {0.0, 0.0, 0.0, 0.0};
    float sample_channel;
    float gain;
    float alt_gain;
    float smooth_gain = 0.0;
    float volume_level;
    float mute_level;
    float prev_mute_level = 0.0;
    float panning_level;
    int sampleRateReductionFactor = 8;

    VolumeSlider volumeSlider;
    Panning panner;
    OnePole onepole1;
    OnePole onepole2;
};

#endif // _H_CHANNEL
