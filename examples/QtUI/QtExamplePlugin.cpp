/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2021 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"
#include <QApplication>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin to show how to get some basic information sent to the UI.
 */
class EmbedExternalExamplePlugin : public Plugin
{
public:
    EmbedExternalExamplePlugin()
        : Plugin(kParameterCount, 0, 0),
          fWidth(512.0f),
          fHeight(384.0f)
    {
        qAppInstantiate();
    }

    ~EmbedExternalExamplePlugin()
    {
        qAppDestroy();
    }

   /* --------------------------------------------------------------------------------------------------------
    * Qt application instance management */

    /**
      Instantiate QApplication instance.
      This function will be called on the initialization of the plugin, since the QApplication lifecycle is during
      the run of the plugin.
    */
    void qAppInstantiate(void)
    {
        if (qApp == nullptr && g_qapp_instance == nullptr)
        {
            static int s_argc = 1;
            static const char *s_argv[] = { "Qt Example UI", nullptr };  // Must have at least one parameter
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
            ::_putenv_s("QT_NO_GLIB", "1"); // Avoid glib event-loop...
#else
            ::setenv("QT_NO_GLIB", "1", 1); // Avoid glib event-loop...
#endif
            g_qapp_instance = new QApplication(s_argc, (char **) s_argv);
        }

        if (g_qapp_instance)
            ++g_qapp_refcount;
    }

    /**
      Destroy QApplication instance.
      This function will be called when unloading the plugin.
    */
    void qAppDestroy(void)
    {
        /**
          Hosts usually run plugin within their main process, so plugin instances may share one QApplication instance.
          A reference counter is essential. It will prevent possible crashes when loading/unloading multiple plugins.
        */
        if (g_qapp_instance && --g_qapp_refcount == 0)
        {
            delete g_qapp_instance;
            g_qapp_instance = nullptr;
        }
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * Information */

   /**
      Get the plugin label.
      This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
    */
    const char* getLabel() const override
    {
        return "QtUI";
    }

   /**
      Get an extensive comment/description about the plugin.
    */
    const char* getDescription() const override
    {
        return "Plugin to show how to use a Qt-based DPF UI.";
    }

   /**
      Get the plugin author/maker.
    */
    const char* getMaker() const override
    {
        return "DISTRHO";
    }

   /**
      Get the plugin homepage.
    */
    const char* getHomePage() const override
    {
        return "https://github.com/DISTRHO/DPF";
    }

   /**
      Get the plugin license name (a single line of text).
      For commercial plugins this should return some short copyright information.
    */
    const char* getLicense() const override
    {
        return "ISC";
    }

   /**
      Get the plugin version, in hexadecimal.
    */
    uint32_t getVersion() const override
    {
        return d_version(1, 0, 0);
    }

   /**
      Get the plugin unique Id.
      This value is used by LADSPA, DSSI and VST plugin formats.
    */
    int64_t getUniqueId() const override
    {
        return d_cconst('d', 'b', 'q', 't');
    }

   /* --------------------------------------------------------------------------------------------------------
    * Init */

   /**
      Initialize the audio port @a index.@n
      This function will be called once, shortly after the plugin is created.
    */
    void initAudioPort(bool input, uint32_t index, AudioPort& port) override
    {
        // treat meter audio ports as stereo
        port.groupId = kPortGroupStereo;

        // everything else is as default
        Plugin::initAudioPort(input, index, port);
    }

   /**
      Initialize the parameter @a index.
      This function will be called once, shortly after the plugin is created.
    */
    void initParameter(uint32_t index, Parameter& parameter) override
    {
        switch (index)
        {
        case kParameterWidth:
            parameter.hints      = kParameterIsAutomatable|kParameterIsInteger;
            parameter.ranges.def = 512.0f;
            parameter.ranges.min = 256.0f;
            parameter.ranges.max = 4096.0f;
            parameter.name   = "Width";
            parameter.symbol = "width";
            parameter.unit   = "px";
            break;
        case kParameterHeight:
            parameter.hints      = kParameterIsAutomatable|kParameterIsInteger;
            parameter.ranges.def = 384.0f;
            parameter.ranges.min = 256.0f;
            parameter.ranges.max = 4096.0f;
            parameter.name   = "Height";
            parameter.symbol = "height";
            parameter.unit   = "px";
            break;
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Internal data */

   /**
      Get the current value of a parameter.
      The host may call this function from any context, including realtime processing.
    */
    float getParameterValue(uint32_t index) const override
    {
        switch (index)
        {
        case kParameterWidth:
            return fWidth;
        case kParameterHeight:
            return fHeight;
        }

        return 0.0f;

    }

   /**
      Change a parameter value.
      The host may call this function from any context, including realtime processing.
      When a parameter is marked as automatable, you must ensure no non-realtime operations are performed.
      @note This function will only be called for parameter inputs.
    */
    void setParameterValue(uint32_t index, float value) override
    {
        switch (index)
        {
        case kParameterWidth:
            fWidth = value;
            break;
        case kParameterHeight:
            fHeight = value;
            break;
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * Audio/MIDI Processing */

   /**
      Run/process function for plugins without MIDI input.
      @note Some parameters might be null if there are no audio inputs or outputs.
    */
    void run(const float** inputs, float** outputs, uint32_t frames) override
    {
       /**
          This plugin does nothing, it just demonstrates information usage.
          So here we directly copy inputs over outputs, leaving the audio untouched.
          We need to be careful in case the host re-uses the same buffer for both inputs and outputs.
        */
        if (outputs[0] != inputs[0])
            std::memcpy(outputs[0], inputs[0], sizeof(float)*frames);
        if (outputs[1] != inputs[1])
            std::memcpy(outputs[1], inputs[1], sizeof(float)*frames);
    }

    // -------------------------------------------------------------------------------------------------------

private:
    // Parameters
    float fWidth, fHeight;

    // Qt application instance
    static QApplication *g_qapp_instance;
    static unsigned int g_qapp_refcount;

   /**
      Set our plugin class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EmbedExternalExamplePlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin* createPlugin()
{
    return new EmbedExternalExamplePlugin();
}

/* ------------------------------------------------------------------------------------------------------------
 * QApp instantiate and destroy. */

QApplication *EmbedExternalExamplePlugin::g_qapp_instance = nullptr;
unsigned int  EmbedExternalExamplePlugin::g_qapp_refcount = 0;

// -----------------------------------------------------------------------------------------------------------


END_NAMESPACE_DISTRHO
