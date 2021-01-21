#ifndef DISTRHO_PLUGIN_SLPLUGIN_HPP_INCLUDED
#define DISTRHO_PLUGIN_SLPLUGIN_HPP_INCLUDED

#include "DistrhoPlugin.hpp"
#include "channelStrip.hpp"
#include "volumeSlider.hpp"
#include "onepole.hpp"

#define NUM_CHANNELS 2
#define NUM_CHANNEL_STRIPS 4

START_NAMESPACE_DISTRHO

class Mixer : public Plugin
{
public:
    enum Parameters
    {
        paramVolume1 = 0,
        paramPanning1,
        paramSolo1,
        paramMute1,
        paramVolume2,
        paramPanning2,
        paramSolo2,
        paramMute2,
        paramVolume3,
        paramPanning3,
        paramSolo3,
        paramMute3,
        paramVolume4,
        paramPanning4,
        paramSolo4,
        paramMute4,
        paramMasterVolume,
        paramPluginEnabled,
        paramCount
    };

    Mixer();
    ~Mixer();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return "Mixer Mono";
    }

    const char* getDescription() const override
    {
        return "";
    }

    const char* getMaker() const noexcept override
    {
        return "MOD";
    }

    const char* getHomePage() const override
    {
        return "http://";
    }

    const char* getLicense() const noexcept override
    {
        return "Custom";
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(1, 0, 0);
    }

    int64_t getUniqueId() const noexcept override
    {
        return d_cconst('m', 'm', 'x', 'm');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void  setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // Process
    void activate() override;
    void deactivate() override;
    void run(const float** inputs, float** outputs, uint32_t frames) override;

    void channelHandler();

private:

    ChannelStrip **mixerChannel;
    VolumeSlider masterSlider;
    OnePole      onepole;

    float muteParam[NUM_CHANNEL_STRIPS];
    float soloParam[NUM_CHANNEL_STRIPS];
    float volumeParam[NUM_CHANNEL_STRIPS];
    float panningParam[NUM_CHANNEL_STRIPS];
    float prevMuteParam[NUM_CHANNEL_STRIPS];
    float prevSoloParam[NUM_CHANNEL_STRIPS];
    float prevVolumeParam[NUM_CHANNEL_STRIPS];
    float prevPanningParam[NUM_CHANNEL_STRIPS];
    float sampleRate;
    float sampleL;
    float sampleR;
    float sampleAltL;
    float sampleAltR;
    float masterVolume;
    bool pluginEnabled;

    int sampleRateReductionFactor;

    void reset();

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mixer)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO

#endif  // DISTRHO_PLUGIN_SLPLUGIN_HPP_INCLUDED



