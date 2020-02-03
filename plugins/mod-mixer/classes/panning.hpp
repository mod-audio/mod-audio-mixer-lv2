#ifndef _H_PANNING_
#define _H_PANNING_

#include <math.h>

class Panning {
public:
    Panning();
    ~Panning();
    void setAngle(float angle);
    void process(float sample);
    float getSample(int channel);

private:
    float samples[2];
    float angle;
};

#endif //_H_PANNING
