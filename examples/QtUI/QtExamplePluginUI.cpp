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

#include "DistrhoUtils.hpp"
#include "PluginWindow.h"
#include <QApplication>
#include <QWindow>
#include <cstddef>

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class EmbedExternalExampleUI : public UI
{
    ExamplePluginWindow *fWindow;
    WId fParent;
    WId fWinId;

public:
    EmbedExternalExampleUI()
        : UI(512, 384),
          fWindow(nullptr)
    {
        const bool standalone = isStandalone();
        d_stdout("isStandalone %d", (int)standalone);

        fParent = (WId) getParentWindowHandle();
        d_stdout("Parent window: native window handle = %d, fParent = %d", getParentWindowHandle(), fParent);
    
        fWindow = new ExamplePluginWindow();

        /**
            Obtain Qt window's WinId before manipulating its window handle.
            This can prevent crash when setting its parent window, or getting its WinId.

            Do NOT forget this function call.
        */
        fWinId = fWindow->winId();

        if (isEmbed() && fParent)
        {
            fWindow->windowHandle()->setParent(QWindow::fromWinId(fParent));
        }

        fWindow->show();

        d_stdout("created external window with size %u %u", getWidth(), getHeight());
    }

    ~EmbedExternalExampleUI()
    {
        /**
            You may need to reset parent window to default if you want to suppress
            "must be a top level window" warnings from Qt.
        */
        fWindow->windowHandle()->setParent(0);

        delete fWindow;
        fWindow = nullptr;
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

        fWindow->setFocus();
    }

    uintptr_t getNativeWindowHandle() const noexcept override
    {
        return (uintptr_t)fWindow->windowHandle()->winId();
    }

    void sizeChanged(uint width, uint height) override
    {
        d_stdout("sizeChanged %u %u", width, height);
        UI::sizeChanged(width, height);

        if (fWindow != 0)
            fWindow->resize(width, height);
    }

    void titleChanged(const char* const title) override
    {
        d_stdout("titleChanged %s", title);

        DISTRHO_SAFE_ASSERT_RETURN(fWindow != 0,);
        fWindow->setWindowTitle(QString(title));
    }

    void transientParentWindowChanged(const uintptr_t winId) override
    {
        d_stdout("transientParentWindowChanged %lu", winId);

        DISTRHO_SAFE_ASSERT_RETURN(fWindow != 0,);
        // NOTICE: Seems not implemented by Qt
    }

    void visibilityChanged(const bool visible) override
    {
        d_stdout("visibilityChanged %d", visible);

        DISTRHO_SAFE_ASSERT_RETURN(fWindow != 0,);

        if (visible)
        {
            fWindow->show();
            fWindow->raise();
            fWindow->activateWindow();
        }
        else
            fWindow->hide();
    }

    void uiIdle() override
    {
        // d_stdout("uiIdle");

        if (fWindow)
        {
            QApplication::processEvents();
            return;
        }
    }

    // -------------------------------------------------------------------------------------------------------

   /**
      Set our UI class as non-copyable and add a leak detector just in case.
    */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EmbedExternalExampleUI)
};

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI* createUI()
{
    return new EmbedExternalExampleUI();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
