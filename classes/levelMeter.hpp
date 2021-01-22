#ifndef _H_LEVEL_METER_
#define _H_LEVEL_METER_

#include <math.h>

class LevelMeter {
public:
    LevelMeter();
    ~LevelMeter();
    void setSampleRate(float sampleRate);
    float process(float input);

private:
    float omega;
    float sampleRate;
    float level;
};

#endif //_H_LEVEL_METER_
