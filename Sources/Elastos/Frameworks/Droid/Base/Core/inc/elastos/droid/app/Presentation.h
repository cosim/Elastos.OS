//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#ifndef __ELASTOS_DROID_APP_PRESENTATION_H__
#define __ELASTOS_DROID_APP_PRESENTATION_H__

#include "Elastos.Droid.Hardware.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/app/Dialog.h"
#include "elastos/droid/view/ContextThemeWrapper.h"

using Elastos::Droid::Os::Handler;
using Elastos::Droid::App::Dialog;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IContextThemeWrapper;
using Elastos::Droid::View::ContextThemeWrapper;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::Display::IDisplayListener;
using Elastos::Droid::Hardware::Display::IDisplayManager;

namespace Elastos {
namespace Droid {
namespace App {

class ECO_PUBLIC Presentation
    : public Dialog
    , public IPresentation
{
private:
    class PresentationDisplayListener
        : public Object
        , public IDisplayListener
    {
    public:
        CAR_INTERFACE_DECL()

        PresentationDisplayListener(
            /* [in] */ Presentation* host)
            : mHost(host)
        {}

        CARAPI OnDisplayAdded(
            /* [in] */ Int32 displayId);

        CARAPI OnDisplayRemoved(
            /* [in] */ Int32 displayId);

        CARAPI OnDisplayChanged(
            /* [in] */ Int32 displayId);

    private:
        Presentation* mHost;
    };

    class PresentationHandler
        : public Handler
    {
    public:
        TO_STRING_IMPL("Presentation::PresentationHandler")

        PresentationHandler(
            /* [in] */ Presentation* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        Presentation* mHost;
    };

    class PresentationContextThemeWrapper
        : public ContextThemeWrapper
    {
    public:
        PresentationContextThemeWrapper();

        CARAPI constructor(
            /* [in] */ IContext* base,
            /* [in] */ Int32 theme,
            /* [in] */ IWindowManager* displayWindowManager);

        CARAPI GetSystemService(
            /* [in] */ const String& name,
            /* [out] */ IInterface** object);

    private:
        AutoPtr<IWindowManager> mDisplayWindowManager;
    };

public:
    CAR_INTERFACE_DECL()

    Presentation();

    virtual ~Presentation();

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IDisplay * display);

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IDisplay * display,
        /* [in] */ Int32 theme);

    /**
     * Gets the {@link Display} that this presentation appears on.
     *
     * @return The display.
     */
    CARAPI GetDisplay(
        /* [out] */ IDisplay **display);

    /**
     * Gets the {@link Resources} that should be used to inflate the layout of this presentation.
     * This resources object has been configured according to the metrics of the
     * display that the presentation appears on.
     *
     * @return The presentation resources object.
     */
    CARAPI GetResources(
        /* [out] */ IResources **resources);

    /**
     * Called by the system when the {@link Display} to which the presentation
     * is attached has been removed.
     *
     * The system automatically calls {@link #cancel} to dismiss the presentation
     * after sending this event.
     *
     * @see #getDisplay
     */
    CARAPI OnDisplayRemoved();

    /**
     * Called by the system when the properties of the {@link Display} to which
     * the presentation is attached have changed.
     *
     * If the display metrics have changed (for example, if the display has been
     * resized or rotated), then the system automatically calls
     * {@link #cancel} to dismiss the presentation.
     *
     * @see #getDisplay
     */
    CARAPI OnDisplayChanged();

    /**
     * Inherited from {@link Dialog#show}. Will throw
     * {@link android.view.WindowManager.InvalidDisplayException} if the specified secondary
     * {@link Display} can't be found.
     */
    //@Override
    // CARAPI Show();

protected:

    CARAPI OnStart();

    CARAPI OnStop();

private:
    CARAPI_(void) HandleDisplayRemoved();

    CARAPI_(void) HandleDisplayChanged();

    CARAPI_(Boolean) IsConfigurationStillValid();

    static CARAPI_(AutoPtr<IContext>) CreatePresentationContext(
        /* [in] */ IContext* outerContext,
        /* [in] */ IDisplay* display,
        /* [in] */ Int32 theme);

private:
    static const String TAG; // = "Presentation";

    AutoPtr<IDisplay> mDisplay;
    AutoPtr<IDisplayManager> mDisplayManager;
    AutoPtr<PresentationDisplayListener> mDisplayListener;
    AutoPtr<PresentationHandler> mHandler;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_PRESENTATION_H__
