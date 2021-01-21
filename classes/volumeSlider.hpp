#ifndef _H_VOLUME_
#define _H_VOLUME_

#include <math.h>

class VolumeSlider {
public:
    VolumeSlider();
    ~VolumeSlider();

    void setCoef(float level);
    float getCoef();

private:
    float coef;
};

#endif // _H_VOLUME_
