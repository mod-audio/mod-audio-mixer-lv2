#include "mod-mixer-stereo.hpp"

START_NAMESPACE_DISTRHO


// -----------------------------------------------------------------------

Mixer::Mixer()
    : Plugin(paramCount, 0, 0) // 1 program, 0 states
{
    pluginEnabled = true;
    truePanning = true;
    sampleRate = (float)getSampleRate();

    postFader1Level = 0.0;
    postFader2Level = 0.0;
    postFader3Level = 0.0;
    postFader4Level = 0.0;
    masterMonitorLevel = 0.0;
    altMonitorLevel = 0.0;

    volumeCoef = 0.0;
    altCoef = 0.0;

    masterVolume = 0.5;
    masterSlider.setCoef(masterVolume);

    altVolume = 0.5;
    altSlider.setCoef(altVolume);

    onepole1.setFc(10.0/48000.0);
    onepole2.setFc(10.0/48000.0);

    mixerChannel = new ChannelStrip*[NUM_CHANNEL_STRIPS];
    levelMeter   = new LevelMeter*[(NUM_CHANNEL_STRIPS/2) + 2];

    int channelSide = 0;

    for (unsigned i = 0; i < NUM_CHANNEL_STRIPS; i++) {
        mixerChannel[i] = new ChannelStrip();
        mixerChannel[i]->setVolume(0.5);
        mixerChannel[i]->setPanning((float)channelSide * 90.0);
        mixerChannel[i]->setMute(false);
        mixerChannel[i]->setAlt(false);
        channelSide ^= 1;
    }

    for (unsigned i = 0; i < NUM_CHANNEL_STRIPS/2; i++) {
        muteParam[i] = false;
        soloParam[i] = false;
    }

    for (unsigned i = 0; i < (NUM_CHANNEL_STRIPS / 2) + 2; i++) {
        levelMeter[i] = new LevelMeter();
    }

    sampleL = 0.0;
    sampleR = 0.0;
    sampleAltL = 0.0;
    sampleAltR = 0.0;

    reset();
}

Mixer::~Mixer()
{
    for (unsigned i = 0; i < NUM_CHANNEL_STRIPS; i++) {
        delete mixerChannel[i];
    }
    delete[] mixerChannel;
    mixerChannel = nullptr;

    for (unsigned i = 0; i < (NUM_CHANNEL_STRIPS / 2) + 2; i++) {
        delete levelMeter[i];
    }
    delete[] levelMeter;
    levelMeter = nullptr;
}

// -----------------------------------------------------------------------
// Init

void Mixer::initParameter(uint32_t index, Parameter& parameter)
{
    switch (index)
    {
        case paramVolume1:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Volume1";
            parameter.symbol     = "Volume1";
            parameter.unit       = "";
            parameter.ranges.def = 5.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPanning1:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Panning1";
            parameter.symbol     = "Panning1";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = -1.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramSolo1:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Solo1";
            parameter.symbol     = "Solo1";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute1:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Mute1";
            parameter.symbol     = "Mute1";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramAlt1:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Alt1";
            parameter.symbol     = "Alt1";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramVolume2:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Volume2";
            parameter.symbol     = "Volume2";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPanning2:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Panning2";
            parameter.symbol     = "Panning2";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = -1.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramSolo2:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Solo2";
            parameter.symbol     = "Solo2";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute2:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Mute2";
            parameter.symbol     = "Mute2";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramAlt2:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Alt2";
            parameter.symbol     = "Alt2";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramVolume3:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Volume3";
            parameter.symbol     = "Volume3";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPanning3:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Panning3";
            parameter.symbol     = "Panning3";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = -1.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramSolo3:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Solo3";
            parameter.symbol     = "Solo3";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramAlt3:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Alt3";
            parameter.symbol     = "Alt3";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute3:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Mute3";
            parameter.symbol     = "Mute3";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramVolume4:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Volume4";
            parameter.symbol     = "Volume4";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPanning4:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Panning4";
            parameter.symbol     = "Panning4";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = -1.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramSolo4:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Solo4";
            parameter.symbol     = "Solo4";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute4:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Mute4";
            parameter.symbol     = "Mute4";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramAlt4:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "Alt4";
            parameter.symbol     = "Alt4";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMasterVolume:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "MasterVolume";
            parameter.symbol     = "MasterVolume";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramAltVolume:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Alt Volume";
            parameter.symbol     = "AltVolume";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramTruePanning:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "True Panning";
            parameter.symbol     = "TruePanning";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPluginEnabled:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
            parameter.name       = "PluginEnabled";
            parameter.symbol     = "PluginEnabled";
            parameter.unit       = "";
            parameter.ranges.def = 1.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPostFader1Level:
            parameter.hints      = kParameterIsOutput;
            parameter.name       = "PostFader1Level";
            parameter.symbol     = "PostFader1Level";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPostFader2Level:
            parameter.hints      = kParameterIsOutput;
            parameter.name       = "PostFader2Level";
            parameter.symbol     = "PostFader2Level";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPostFader3Level:
            parameter.hints      = kParameterIsOutput;
            parameter.name       = "PostFader3Level";
            parameter.symbol     = "PostFader3Level";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramPostFader4Level:
            parameter.hints      = kParameterIsOutput;
            parameter.name       = "PostFader4Level";
            parameter.symbol     = "PostFader4Level";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMasterMonitorLevel:
            parameter.hints      = kParameterIsOutput;
            parameter.name       = "MasterMonitorLevel";
            parameter.symbol     = "MasterMonitorLevel";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramAltMonitorLevel:
            parameter.hints      = kParameterIsOutput;
            parameter.name       = "AltMonitorLevel";
            parameter.symbol     = "AltMonitorLevel";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
    }
}

// -----------------------------------------------------------------------
// Internal data

float Mixer::getParameterValue(uint32_t index) const
{
    switch (index)
    {
        case paramVolume1:
            return mixerChannel[0]->getVolume();
        case paramPanning1:
            return mixerChannel[0]->getPanning();
        case paramSolo1:
            return soloParam[0];
        case paramMute1:
            return muteParam[0];
        case paramAlt1:
            return mixerChannel[0]->getAltChannel();
        case paramVolume2:
            return mixerChannel[1]->getVolume();
        case paramPanning2:
            return mixerChannel[1]->getPanning();
        case paramSolo2:
            return soloParam[1];
        case paramMute2:
            return muteParam[1];
        case paramAlt2:
            return mixerChannel[1]->getAltChannel();
        case paramVolume3:
            return mixerChannel[2]->getVolume();
        case paramPanning3:
            return mixerChannel[2]->getPanning();
        case paramSolo3:
            return soloParam[2];
        case paramMute3:
            return muteParam[2];
        case paramAlt3:
            return mixerChannel[2]->getAltChannel();
        case paramVolume4:
            return mixerChannel[3]->getVolume();
        case paramPanning4:
            return mixerChannel[3]->getPanning();
        case paramSolo4:
            return soloParam[3];
        case paramMute4:
            return muteParam[3];
        case paramAlt4:
            return mixerChannel[3]->getAltChannel();
        case paramMasterVolume:
            return masterVolume;
        case paramAltVolume:
            return altVolume;
        case paramTruePanning:
            return truePanning;
        case paramPluginEnabled:
            return pluginEnabled;
        case paramPostFader1Level:
            return postFader1Level;
        case paramPostFader2Level:
            return postFader2Level;
        case paramPostFader3Level:
            return postFader3Level;
        case paramPostFader4Level:
            return postFader4Level;
        case paramMasterMonitorLevel:
            return masterMonitorLevel;
        case paramAltMonitorLevel:
            return altMonitorLevel;
    }
}

void Mixer::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
        case paramVolume1:
            mixerChannel[0]->setVolume(value);
            mixerChannel[1]->setVolume(value);
            break;
        case paramPanning1:
            setPanning(0, value);
            break;
        case paramSolo1:
            soloParam[0] = static_cast<bool>(value);
            channelHandler();
            break;
        case paramMute1:
            muteParam[0] = value;
            mixerChannel[0]->setMute(static_cast<bool>(value));
            mixerChannel[1]->setMute(static_cast<bool>(value));
            channelHandler();
            break;
        case paramAlt1:
            mixerChannel[0]->setAlt(static_cast<bool>(value));
            mixerChannel[1]->setAlt(static_cast<bool>(value));
            channelHandler();
            break;
        case paramVolume2:
            mixerChannel[2]->setVolume(value);
            mixerChannel[3]->setVolume(value);
            break;
        case paramPanning2:
            setPanning(2, value);
            break;
        case paramSolo2:
            soloParam[1] = static_cast<bool>(value);
            channelHandler();
            break;
        case paramMute2:
            muteParam[1] = value;
            mixerChannel[2]->setMute(static_cast<bool>(value));
            mixerChannel[3]->setMute(static_cast<bool>(value));
            channelHandler();
            break;
        case paramAlt2:
            mixerChannel[2]->setAlt(static_cast<bool>(value));
            mixerChannel[3]->setAlt(static_cast<bool>(value));
            channelHandler();
            break;
        case paramVolume3:
            mixerChannel[4]->setVolume(value);
            mixerChannel[5]->setVolume(value);
            break;
        case paramPanning3:
            setPanning(4, value);
            break;
        case paramSolo3:
            soloParam[2] = static_cast<bool>(value);
            channelHandler();
            break;
        case paramMute3:
            muteParam[2] = value;
            mixerChannel[4]->setMute(static_cast<bool>(value));
            mixerChannel[5]->setMute(static_cast<bool>(value));
            channelHandler();
            break;
        case paramAlt3:
            mixerChannel[4]->setAlt(static_cast<bool>(value));
            mixerChannel[5]->setAlt(static_cast<bool>(value));
            channelHandler();
            break;
        case paramVolume4:
            mixerChannel[6]->setVolume(value);
            mixerChannel[7]->setVolume(value);
            break;
        case paramPanning4:
            setPanning(6, value);
            break;
        case paramSolo4:
            soloParam[3] = static_cast<bool>(value);
            channelHandler();
            break;
        case paramMute4:
            muteParam[3] = value;
            mixerChannel[6]->setMute(static_cast<bool>(value));
            mixerChannel[7]->setMute(static_cast<bool>(value));
            channelHandler();
            break;
        case paramAlt4:
            mixerChannel[6]->setAlt(static_cast<bool>(value));
            mixerChannel[7]->setAlt(static_cast<bool>(value));
            channelHandler();
            break;
        case paramMasterVolume:
            masterVolume = value;
            masterSlider.setCoef(masterVolume);
            break;
        case paramAltVolume:
            altVolume = value;
            altSlider.setCoef(altVolume);
            break;
        case paramTruePanning:
            truePanning = (bool)value;
            break;
        case paramPluginEnabled:
            pluginEnabled = value;
            break;
    }
}

void Mixer::reset()
{
}

// -----------------------------------------------------------------------
// Process

void Mixer::activate()
{
}

void Mixer::deactivate()
{
}

void Mixer::channelHandler()
{
    bool solo_found = false;
    int params_checked = 0;

    while(params_checked < NUM_CHANNEL_STRIPS/2)
    {
        if (soloParam[params_checked] == 1) {
            int paramIndex = 0;
            for (unsigned s = 0; s < NUM_CHANNEL_STRIPS; s+=NUM_CHANNELS) {
                if (soloParam[paramIndex] != 1) {
                    mixerChannel[s]->setMute(1);
                    mixerChannel[s+1]->setMute(1);
                } else {
                    mixerChannel[s]->setMute(0);
                    mixerChannel[s+1]->setMute(0);
                }
                paramIndex++;
            }
            solo_found = true;
        }
        params_checked++;
    }
    if (!solo_found) {
        int paramIndex = 0;
        for (unsigned s = 0; s < NUM_CHANNEL_STRIPS; s+=NUM_CHANNELS) {
            mixerChannel[s]->setMute(muteParam[paramIndex]);
            mixerChannel[s+1]->setMute(muteParam[paramIndex]);
            paramIndex++;
        }
    }
}

void Mixer::setPanning(int index, float panningParamValue)
{
    float panning_l = (panningParamValue >= 0.0) ? ((panningParamValue * 1.0045) * 2.0) - 1.0 : -1.00;
    mixerChannel[index]->setPanning(((panning_l * 0.5) + 0.5) * 90.0);
    float panning_r = (panningParamValue <= 0.0) ? (panningParamValue * 2.0) + 1.0 : 1.0090;
    mixerChannel[index + 1]->setPanning(((panning_r * 0.5) + 0.5) * 90.0);
}


void Mixer::run(const float** inputs, float** outputs, uint32_t frames)
{

    float monitoredFrame[NUM_CHANNEL_STRIPS + 2][frames];

    // Main processing body
    for (uint32_t f = 0; f < frames; ++f)
    {
        for (unsigned c = 0; c < NUM_CHANNEL_STRIPS; c++)
        {
            mixerChannel[c]->process(inputs[c][f]);

            monitoredFrame[c][f] = (mixerChannel[c]->getSample(0) + mixerChannel[c]->getSample(1)) / 2.0;
            monitoredFrame[c][f] += (mixerChannel[c]->getSample(2) + mixerChannel[c]->getSample(3)) / 2.0;

            if (truePanning) {
                sampleL += mixerChannel[c]->getSample(0);
                sampleR += mixerChannel[c]->getSample(1);
                sampleAltL += mixerChannel[c]->getSample(2);
                sampleAltR += mixerChannel[c]->getSample(3);
            } else {
                if (c % 2 == 0) {
                    sampleL += mixerChannel[c]->getSample(0);
                    sampleR += 0.0;
                    sampleAltL += mixerChannel[c]->getSample(2);
                    sampleAltR += 0.0;
                } else {
                    sampleL += 0.0;
                    sampleR += mixerChannel[c]->getSample(1);
                    sampleAltL += 0.0;
                    sampleAltR += mixerChannel[c]->getSample(3);

                }
            }
        }

        if (pluginEnabled) {
            volumeCoef = masterSlider.getCoef();
            altCoef = altSlider.getCoef();
        } else {
            volumeCoef = 0.0;
            altCoef = 0.0;
        }

        float masterGain = onepole1.process(volumeCoef);
        float altGain = onepole2.process(altCoef);

        monitoredFrame[8][f] = masterGain * (sampleL + sampleR) / 2.0;
        monitoredFrame[9][f] = altGain * (sampleAltL + sampleAltR) / 2.0;

        outputs[0][f] = masterGain * sampleL;
        outputs[1][f] = masterGain * sampleR;
        outputs[2][f] = altGain * sampleAltL;
        outputs[3][f] = altGain * sampleAltR;

        sampleL = 0.0;
        sampleR = 0.0;
        sampleAltL = 0.0;
        sampleAltR = 0.0;
    }

    postFader1Level = levelMeter[0]->stereoProcess(monitoredFrame[0], monitoredFrame[1], frames);
    postFader2Level = levelMeter[1]->stereoProcess(monitoredFrame[2], monitoredFrame[3], frames);
    postFader3Level = levelMeter[2]->stereoProcess(monitoredFrame[4], monitoredFrame[5], frames);
    postFader4Level = levelMeter[3]->stereoProcess(monitoredFrame[6], monitoredFrame[7], frames);

    masterMonitorLevel = levelMeter[4]->process(monitoredFrame[8], frames);
    altMonitorLevel    = levelMeter[5]->process(monitoredFrame[9], frames);
}



// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new Mixer();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
