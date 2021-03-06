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

#ifndef __ELASTOS_DROID_SYSTEMUI_RECENT_RECENTTASKSLOADER_H__
#define __ELASTOS_DROID_SYSTEMUI_RECENT_RECENTTASKSLOADER_H__

#include "_Elastos.Droid.SystemUI.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Graphics.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.Droid.View.h"
#include "Elastos.CoreLibrary.Core.h"
#include "Elastos.CoreLibrary.Utility.h"
#include <elastos/core/Runnable.h>
#include "elastos/droid/os/AsyncTask.h"
#include "elastos/droid/systemui/recent/RecentsPanelView.h"
#include <elastos/core/Thread.h>

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::AsyncTask;
using Elastos::Droid::Os::IHandler;
using Elastos::Core::Runnable;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnTouchListener;
using Elastos::Core::ICharSequence;
using Elastos::Core::IRunnable;
using Elastos::Core::Thread;
using Elastos::Utility::IArrayList;
using Elastos::Utility::Concurrent::IBlockingQueue;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

class RecentTasksLoader
    : public Object
    , public IViewOnTouchListener
    , public IRecentTasksLoader
{
public:
    class PreloadTasksRunnable
        : public Runnable
    {
    public:
        PreloadTasksRunnable(
            /* [in] */ RecentTasksLoader* host);

        CARAPI Run();

    private:
        RecentTasksLoader* mHost;
    };

private:
    class BgLoadThread
        : public Thread
    {
    public:
        BgLoadThread(
            /* [in] */ RecentTasksLoader* host);

        CARAPI Run();

    private:
        RecentTasksLoader* mHost;
    };

    class TaskLoaderAsyncTask
        : public AsyncTask
    {
    public:
        TaskLoaderAsyncTask(
            /* [in] */ IBlockingQueue* queue,
            /* [in] */ RecentTasksLoader* host);

    protected:
        CARAPI OnProgressUpdate(
            /* [in] */ ArrayOf<IInterface*>* values);

        CARAPI DoInBackground(
                /* [in] */ ArrayOf<IInterface*>* params,
                /* [out] */ IInterface** result);

    private:
        AutoPtr<IBlockingQueue> mTasksWaitingForThumbnails;
        RecentTasksLoader* mHost;
    };

    class ThumbnailLoaderAsyncTask
        : public AsyncTask
    {
    public:
        ThumbnailLoaderAsyncTask(
            /* [in] */ IBlockingQueue* queue,
            /* [in] */ RecentTasksLoader* host)
            : mTasksWaitingForThumbnails(queue)
            , mHost(host)
        {}

    protected:
        CARAPI OnProgressUpdate(
            /* [in] */ ArrayOf<IInterface*>* values);

        CARAPI DoInBackground(
                /* [in] */ ArrayOf<IInterface*>* params,
                /* [out] */ IInterface** result);

    private:
        AutoPtr<IBlockingQueue> mTasksWaitingForThumbnails;
        RecentTasksLoader* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    static CARAPI_(AutoPtr<RecentTasksLoader>) GetInstance(
        /* [in] */ IContext* context);

    CARAPI SetRecentsPanel(
        /* [in] */ IRecentsPanelView* newRecentsPanel,
        /* [in] */ IRecentsPanelView* caller);

    CARAPI GetRecentsPanel(
        /* [out] */ IRecentsPanelView** rp);

    CARAPI GetDefaultThumbnail(
        /* [out] */ IDrawable** drawable);

    CARAPI GetDefaultIcon(
        /* [out] */ IDrawable** drawable);

    CARAPI GetLoadedTasks(
        /* [out, callee] */ IArrayList** tasks);

    CARAPI Remove(
        /* [in] */ ITaskDescription* td);

    CARAPI IsFirstScreenful(
        /* [out] */ Boolean* result);

    CARAPI CreateTaskDescription(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 persistentTaskId,
        /* [in] */ IIntent* baseIntent,
        /* [in] */ IComponentName* origActivity,
        /* [in] */ ICharSequence* description,
        /* [in] */ Int32 userId,
        /* [out] */ ITaskDescription** td);

    CARAPI LoadThumbnailAndIcon(
        /* [in] */ ITaskDescription* td);

    CARAPI GetFullResDefaultActivityIcon(
        /* [out] */ IDrawable** drawable);

    CARAPI GetFullResIcon(
        /* [in] */ IResources* resources,
        /* [in] */ Int32 iconId,
        /* [out] */ IDrawable** drawable);

    CARAPI OnTouch(
        /* [in] */ IView* v,
        /* [in] */ IMotionEvent* ev,
        /* [out] */ Boolean* result);

    CARAPI PreloadRecentTasksList();

    CARAPI CancelPreloadingRecentTasksList();

    CARAPI CancelLoadingThumbnailsAndIcons(
        /* [in] */ IRecentsPanelView* caller);

    CARAPI PreloadFirstTask();

    CARAPI CancelPreloadingFirstTask();

    CARAPI GetFirstTask(
        /* [out] */ ITaskDescription** des);

    CARAPI LoadFirstTask(
        /* [out] */ ITaskDescription** des);

    CARAPI LoadTasksInBackground();

    CARAPI LoadTasksInBackground(
        /* [in] */ Boolean zeroeth);

private:
    RecentTasksLoader();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI_(Boolean) IsCurrentHomeActivity(
        /* [in] */ IComponentName* component,
        /* [in] */ IActivityInfo* homeInfo);

    CARAPI_(AutoPtr<IDrawable>) GetFullResIcon(
        /* [in] */ IResolveInfo* info,
        /* [in] */ IPackageManager* packageManager);

    CARAPI_(void) CancelLoadingThumbnailsAndIcons();

    CARAPI_(void) ClearFirstTask();

    CARAPI_(void) LoadThumbnailsAndIconsInBackground(
        /* [in] */ IBlockingQueue* tasksWaitingForThumbnails);

private:
    static const Boolean DEBUG;

    static const Int32 DISPLAY_TASKS;
    static const Int32 MAX_TASKS;

    AutoPtr<IContext> mContext;
    // RecentsPanelView has it's reference
    AutoPtr<IWeakReference> mRecentsPanel; //IRecentsPanelView*

    Object mFirstTaskLock;
    AutoPtr<ITaskDescription> mFirstTask;
    Boolean mFirstTaskLoaded;

    AutoPtr<TaskLoaderAsyncTask> mTaskLoader;
    AutoPtr<ThumbnailLoaderAsyncTask> mThumbnailLoader;
    AutoPtr<IHandler> mHandler;

    Int32 mIconDpi;
    AutoPtr<IDrawable> mDefaultThumbnailBackground;
    AutoPtr<IDrawable> mDefaultIconBackground;
    Int32 mNumTasksInFirstScreenful;

    Boolean mFirstScreenful;
    AutoPtr<IArrayList> mLoadedTasks;

    typedef enum {
        State_LOADING,
        State_LOADED,
        State_CANCELLED
    } State;
    State mState;

    static AutoPtr<RecentTasksLoader> sInstance;

    AutoPtr<IRunnable> mPreloadTasksRunnable;

    Boolean mPreloadingFirstTask;
    Boolean mCancelPreloadingFirstTask;
};

}// namespace Recent
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_RECENT_RECENTTASKSLOADER_H__
