#include "mod-mixer-stereo.hpp"
#include <iostream>

START_NAMESPACE_DISTRHO


// -----------------------------------------------------------------------

Mixer::Mixer()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    loadProgram(0);

    sampleRate = (float)getSampleRate();

    volume1 = 0.5;
    panning1 = 0.0;
    solo1 = 0.0;
    mute1 = 0.0;
    volume2 = 0.5;
    panning2 = 0.0;
    solo2 = 0.0;
    mute2 = 0.0;
    volume3 = 0.5;
    panning3 =0.0;
    solo3 = 0.0;
    mute3 = 0.0;
    volume4 = 0.5;
    panning4 = 0.0;
    solo4 = 0.0;
    mute4 = 0.0;
    masterVolume = 0.5;

    mixerChannel = new ChannelStrip*[NUM_CHANNEL_STRIPS];

    int channel = 0;
    for (unsigned i = 0; i < NUM_CHANNEL_STRIPS; i++) {
        mixerChannel[i] = new ChannelStrip(channel);
        channel ^= 1;
    }

    sampleL = 0.0;
    sampleR = 0.0;
    sampleAltL = 0.0;
    sampleAltR = 0.0;

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
            parameter.ranges.def = -1.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramSolo4:
            parameter.hints      = kParameterIsAutomable | kParameterIsBoolean;
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
        case paramMasterVolume:
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "MasterVolume";
            parameter.symbol     = "MasterVolume";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
    }
}

void Mixer::initProgramName(uint32_t index, String& programName)
{
    if (index != 0)
        return;

    programName = "Default";
}

// -----------------------------------------------------------------------
// Internal data

float Mixer::getParameterValue(uint32_t index) const
{
    switch (index)
    {
        case paramVolume1:
            return volume1;
        case paramPanning1:
            return panning1;
        case paramSolo1:
            return solo1;
        case paramMute1:
            return mute1;
        case paramVolume2:
            return volume2;
        case paramPanning2:
            return panning2;
        case paramSolo2:
            return solo2;
        case paramMute2:
            return mute2;
        case paramVolume3:
            return volume3;
        case paramPanning3:
            return panning3;
        case paramSolo3:
            return solo3;
        case paramMute3:
            return mute3;
        case paramVolume4:
            return volume4;
        case paramPanning4:
            return panning4;
        case paramSolo4:
            return solo4;
        case paramMute4:
            return mute4;
        case paramMasterVolume:
            return masterVolume;
    }
}

void Mixer::setParameterValue(uint32_t index, float value)
{
    switch (index)
    {
        case paramVolume1:
            volume1 = value;
            break;
        case paramPanning1:
            panning1 = value;
            break;
        case paramSolo1:
            solo1 = value;
            break;
        case paramMute1:
            mute1 = value;
            break;
        case paramVolume2:
            volume2 = value;
            break;
        case paramPanning2:
            panning2 = value;
            break;
        case paramSolo2:
            solo2 = value;
            break;
        case paramMute2:
            mute2 = value;
            break;
        case paramVolume3:
            volume3 = value;
            break;
        case paramPanning3:
            panning3 = value;
            break;
        case paramSolo3:
            solo3 = value;
            break;
        case paramMute3:
            mute3 = value;
            break;
        case paramVolume4:
            volume4 = value;
            break;
        case paramPanning4:
            panning4 = value;
            break;
        case paramSolo4:
            solo4 = value;
            break;
        case paramMute4:
            mute4 = value;
            break;
        case paramMasterVolume:
            masterVolume = value;
            break;
    }
}

void Mixer::loadProgram(uint32_t index)
{
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

    while(!solo_found && params_checked < NUM_CHANNEL_STRIPS)
    {
        if (soloParam[params_checked] == 1) {
            for (unsigned s = 0; s < NUM_CHANNEL_STRIPS; s++) {
                if (soloParam[s] != 1) {
                    muteParam[s] = 1;
                }
            }
        }
        params_checked++;
    }
}


void Mixer::run(const float** inputs, float** outputs, uint32_t frames)
{
    muteParam[0] = mute1;
    muteParam[1] = mute2;
    muteParam[2] = mute3;
    muteParam[3] = mute4;
    soloParam[0] = solo1;
    soloParam[1] = solo2;
    soloParam[2] = solo3;
    soloParam[3] = solo4;
    volumeParam[0] = volume1;
    volumeParam[1] = volume2;
    volumeParam[2] = volume3;
    volumeParam[3] = volume4;
    panningParam[0] = panning1;
    panningParam[1] = panning2;
    panningParam[2] = panning3;
    panningParam[3] = panning4;

    channelHandler();

    // Main processing body
    for (uint32_t f = 0; f < frames; ++f)
    {
        for (unsigned s = 0; s < NUM_CHANNEL_STRIPS; s+=2) {
            for (unsigned c = 0; c < 2; c++) {
                mixerChannel[s + c]->setVolume(volumeParam[s / 2]);

                float s_panning[2];
                s_panning[0] = (volumeParam[s / 2] > 0.0) ? (volumeParam[s / 2] * 2) - 1 : -1;
                s_panning[1] = (volumeParam[s / 2] < 0.0) ? (volumeParam[s / 2] * 2) + 1 : 1;

                mixerChannel[s + c]->setPanning(s_panning[c] * 0.768);
                mixerChannel[s + c]->setMute(muteParam[s / 2]);
                mixerChannel[s + c]->process(inputs[s + c][f]);

                sampleL += mixerChannel[s + c]->getSample(0);
                sampleR += mixerChannel[s + c]->getSample(1);
                sampleAltL += mixerChannel[s + c]->getSample(2);
                sampleAltR += mixerChannel[s + c]->getSample(3);
            }
        }

        outputs[0][f] = sampleL;
        outputs[1][f] = sampleR;
        outputs[2][f] = sampleAltL;
        outputs[3][f] = sampleAltR;

        sampleL = 0.0;
        sampleR = 0.0;
        sampleAltL = 0.0;
        sampleAltR = 0.0;
    }
}



// -----------------------------------------------------------------------

Plugin* createPlugin()
{
    return new Mixer();
}

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
