#include "mod-mixer.hpp"

START_NAMESPACE_DISTRHO


// -----------------------------------------------------------------------

Mixer::Mixer()
    : Plugin(paramCount, 1, 0) // 1 program, 0 states
{
    loadProgram(0);

    sampleRate = (float)getSampleRate();

    masterVolume = 0.5;

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
            parameter.hints      = kParameterIsAutomable;
            parameter.name       = "Solo3";
            parameter.symbol     = "Solo3";
            parameter.unit       = "";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.f;
            break;
        case paramMute3:
            parameter.hints      = kParameterIsAutomable;
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
    channelHandler();

    // Main processing body
    for (uint32_t f = 0; f < frames; ++f)
    {
        for (unsigned c = 0; c < NUM_CHANNEL_STRIPS; c++) 
        {
            if (volumeParam[c] != prevVolumeParam[c]) {
                mixerChannel[c]->setVolume(volumeParam[c]);
                prevVolumeParam[c] = volumeParam[c];
            }
            if (panningParam[c] != prevPanningParam[c]) {
                mixerChannel[c]->setPanning(((panningParam[c] * 0.5) + 0.5) * 90.0);
                prevPanningParam[c] = panningParam[c];
            }
            if (muteParam[c] != prevMuteParam[c]) {
                mixerChannel[c]->setMute(muteParam[c]);
                prevMuteParam[c] = muteParam[c];
            }

            mixerChannel[c]->process(inputs[c][f]);

            sampleL += mixerChannel[c]->getSample(0);
            sampleR += mixerChannel[c]->getSample(1);
            sampleAltL += mixerChannel[c]->getSample(2);
            sampleAltR += mixerChannel[c]->getSample(3);
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
