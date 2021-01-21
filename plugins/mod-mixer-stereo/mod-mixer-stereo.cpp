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

    sampleRateReductionFactor = 1;

    for (unsigned i = 0; i < NUM_CHANNEL_STRIPS; i++) {
        volumeParam[i] = 0.5;
        panningParam[i] = 0.0;
        soloParam[i] = 0.0;
        muteParam[i] = 0.0;
        mixerChannel[i] = new ChannelStrip(sampleRateReductionFactor);
        mixerChannel[i]->setVolume(0.5);
        mixerChannel[i]->setPanning(0.5 * 90.0);
        mixerChannel[i]->setMute(0.0);
    }

    sampleL = 0.0;
    sampleR = 0.0;
    sampleAltL = 0.0;
    sampleAltR = 0.0;

    prevMuteParam[0] = 0.0;
    prevMuteParam[1] = 0.0;
    prevMuteParam[2] = 0.0;
    prevMuteParam[3] = 0.0;
    prevSoloParam[0] = 0.0;
    prevSoloParam[1] = 0.0;
    prevSoloParam[2] = 0.0;
    prevSoloParam[3] = 0.0;
    prevVolumeParam[0] = 0.5;
    prevVolumeParam[1] = 0.5;
    prevVolumeParam[2] = 0.5;
    prevVolumeParam[3] = 0.5;
    prevPanningParam[0] = 0.0;
    prevPanningParam[1] = 0.0;
    prevPanningParam[2] = 0.0;
    prevPanningParam[3] = 0.0;

    reset();
}

Mixer::~Mixer()
{
    delete[] mixerChannel;
    mixerChannel = nullptr;
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
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Solo1";
            parameter.symbol     = "Solo1";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute1:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Mute1";
            parameter.symbol     = "Mute1";
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
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Solo2";
            parameter.symbol     = "Solo2";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute2:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Mute2";
            parameter.symbol     = "Mute2";
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
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Mute4";
            parameter.symbol     = "Mute4";
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
            return volumeParam[0];
        case paramPanning1:
            return panningParam[0];
        case paramSolo1:
            return soloParam[0];
        case paramMute1:
            return muteParam[0];
        case paramVolume2:
            return volumeParam[1];
        case paramPanning2:
            return panningParam[1];
        case paramSolo2:
            return soloParam[1];
        case paramMute2:
            return muteParam[1];
        case paramVolume3:
            return volumeParam[2];
        case paramPanning3:
            return panningParam[2];
        case paramSolo3:
            return soloParam[2];
        case paramMute3:
            return muteParam[2];
        case paramVolume4:
            return volumeParam[3];
        case paramPanning4:
            return panningParam[3];
        case paramSolo4:
            return soloParam[3];
        case paramMute4:
            return muteParam[3];
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
            volumeParam[0] = value;
            break;
        case paramPanning1:
            panningParam[0] = value;
            break;
        case paramSolo1:
            soloParam[0] = value;
            break;
        case paramMute1:
            muteParam[0] = value;
            break;
        case paramVolume2:
            volumeParam[1] = value;
            break;
        case paramPanning2:
            panningParam[1] = value;
            break;
        case paramSolo2:
            soloParam[1] = value;
            break;
        case paramMute2:
            muteParam[1] = value;
            break;
        case paramVolume3:
            volumeParam[2] = value;
            break;
        case paramPanning3:
            panningParam[2] = value;
            break;
        case paramSolo3:
            soloParam[2] = value;
            break;
        case paramMute3:
            muteParam[2] = value;
            break;
        case paramVolume4:
            volumeParam[3] = value;
            break;
        case paramPanning4:
            panningParam[3] = value;
            break;
        case paramSolo4:
            soloParam[3] = value;
            break;
        case paramMute4:
            muteParam[3] = value;
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


void Mixer::run(const float** inputs, float** outputs, uint32_t frames)
{
    channelHandler();

    int paramIndex = 0;

    for (unsigned c = 0; c < NUM_CHANNEL_STRIPS; c+=NUM_CHANNELS)
    {
        if (volumeParam[paramIndex] != prevVolumeParam[paramIndex]) {
            mixerChannel[c]->setVolume(volumeParam[paramIndex]);
            mixerChannel[c+1]->setVolume(volumeParam[paramIndex]);
            prevVolumeParam[paramIndex] = volumeParam[paramIndex];
        }
        if (panningParam[paramIndex] != prevPanningParam[paramIndex]) {
            float panning_l = (panningParam[paramIndex] >= 0) ? (panningParam[paramIndex] * 2.0) - 1.0 : -1.0;
            mixerChannel[c]->setPanning(((panning_l * 0.5) + 0.5) * 90.0);
            float panning_r = (panningParam[paramIndex] <= 0) ? (panningParam[paramIndex] * 2.0) + 1.0 : 1.0;
            mixerChannel[c+1]->setPanning(((panning_r * 0.5) + 0.5) * 90.0);
            prevPanningParam[paramIndex] = panningParam[paramIndex];
        }
        if (muteParam[paramIndex] != prevMuteParam[paramIndex]) {
            mixerChannel[c]->setMute(muteParam[paramIndex]);
            mixerChannel[c+1]->setMute(muteParam[paramIndex]);
            prevMuteParam[paramIndex] = muteParam[paramIndex];
        }
        paramIndex++;
    }

    // Main processing body
    for (uint32_t f = 0; f < frames; ++f)
    {
        for (unsigned c = 0; c < NUM_CHANNEL_STRIPS; c++)
        {
            mixerChannel[c]->process(inputs[c][f]);

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

        outputs[0][f] = masterGain * sampleL;
        outputs[1][f] = masterGain * sampleR;
        outputs[2][f] = altGain * sampleAltL;
        outputs[3][f] = altGain * sampleAltR;

        sampleL = 0.0;
        sampleR = 0.0;
        sampleAltL = 0.0;
        sampleAltR = 0.0;
    }

    //TODO this part if for testing, will need some improvements
    postFader1Level = fabs((mixerChannel[0]->getSample(0) + mixerChannel[0]->getSample(1)) / 2.0);
    postFader2Level = fabs((mixerChannel[1]->getSample(0) + mixerChannel[1]->getSample(1)) / 2.0);
    postFader3Level = fabs((mixerChannel[2]->getSample(0) + mixerChannel[2]->getSample(1)) / 2.0);
    postFader4Level = fabs((mixerChannel[3]->getSample(0) + mixerChannel[3]->getSample(1)) / 2.0);

    masterMonitorLevel = fabs((outputs[0][0] + outputs[1][0]) / 2.0);
    altMonitorLevel = fabs((outputs[2][0] + outputs[3][0]) / 2.0);
}



// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new Mixer();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
