#ifndef _H_LEVEL_METER_
#define _H_LEVEL_METER_

#include <math.h>
#include <stdint.h>

class LevelMeter {
public:
    LevelMeter();
    ~LevelMeter();
    void setSampleRate(float sampleRate);
    void pre(uint32_t n_samples);
    void post();
    float process(float *input, uint32_t n_samples);

private:
    float sampleRate;
    float level;
    float rate;
    float meterLevel;
    float dbLvl;
    float levelOut;
    float spp;
    float falloff;
};

#endif //_H_LEVEL_METER_
