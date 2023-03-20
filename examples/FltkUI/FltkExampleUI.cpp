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

// needed for IDE
#include "DistrhoPluginInfo.h"

#include "DistrhoUI.hpp"

#include "FltkTestUI.h"

#include <memory>

#if _WIN32
#include <windows.h>
#endif

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class FltkExampleUI : public UI
{
    std::unique_ptr<FltkTestUi> fUiInstance;
    Fl_Double_Window *fWindow;

public:
    FltkExampleUI()
        : UI(512, 256), fUiInstance(nullptr), fWindow(nullptr)
    {
        fUiInstance = std::make_unique<FltkTestUi>();
        fWindow = fUiInstance->make_window();

        fWindow->show();

        _reparentWindow();
    }

    ~FltkExampleUI()
    {
        if (fWindow != nullptr) {
            delete fWindow;
        }
    }

protected:
   /* --------------------------------------------------------------------------------------------------------
    * DSP/Plugin Callbacks */

   /**
      A parameter has changed on the plugin side.
      This is called by the host to inform the UI about parameter changes.
    */
    void parameterChanged(uint32_t index, float value) override
    {
        d_stdout("parameterChanged %u %f", index, value);

        switch (index)
        {
        case kParameterWidth:
            setWidth(static_cast<int>(value + 0.5f));
            break;
        case kParameterHeight:
            setHeight(static_cast<int>(value + 0.5f));
            break;
        }
    }

   /* --------------------------------------------------------------------------------------------------------
    * External Window overrides */

    void focus() override
    {
        d_stdout("focus");

        DISTRHO_SAFE_ASSERT_RETURN(fWindow != nullptr,);

        fWindow->focus(fWindow);
    }

    uintptr_t getNativeWindowHandle() const noexcept override
    {
#if defined(DISTRHO_OS_HAIKU)
#elif defined(DISTRHO_OS_MAC)
        return (uintptr_t)fView;
#elif defined(DISTRHO_OS_WINDOWS)
        return (uintptr_t)fWindow;
#else
        return fl_xid(fWindow);
#endif
    }

    void sizeChanged(uint width, uint height) override
    {
        d_stdout("sizeChanged %u %u", width, height);
        UI::sizeChanged(width, height);

        if (fWindow != 0)
            fWindow->size(width, height);
    }

    void titleChanged(const char* const title) override
    {
        d_stdout("titleChanged %s", title);

        DISTRHO_SAFE_ASSERT_RETURN(fWindow != 0,);
        
        // NOT Implemented
    }

    void transientParentWindowChanged(const uintptr_t winId) override
    {
        d_stdout("transientParentWindowChanged %lu", winId);

        // NOT implemented
    }

    void visibilityChanged(const bool visible) override
    {
        d_stdout("visibilityChanged %d", visible);

        DISTRHO_SAFE_ASSERT_RETURN(fWindow != 0,);

        if (visible)
            fWindow->show();
        else
            fWindow->hide();
    }

    void uiIdle() override
    {
        Fl::check();
        Fl::flush();
    }

    // -------------------------------------------------------------------------------------------------------

private:

    void _reparentWindow()
    {
#if defined(_WIN32)
        // Reparent window
        SetParent((HWND)getNativeWindowHandle(), (HWND)getParentWindowHandle());
        // Reset window style (no border)
        SetWindowLongPtrA((HWND)getNativeWindowHandle(), GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_SYSMENU);
        // Reset window position and size
        fWindow->position(0, 0);
#elif defined(__APPLE__)
#warning "Apple embed window feature is not yet implemented"
        return;
#else
#warning "X11 embed window feature is not yet implemented"
#endif
    }

   /**
      Set our UI class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FltkExampleUI)
};

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new FltkExampleUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
