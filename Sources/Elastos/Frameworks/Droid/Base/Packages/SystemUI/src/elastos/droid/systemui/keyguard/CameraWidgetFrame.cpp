
#include "elastos/droid/systemui/keyguard/CameraWidgetFrame.h"
#include "Elastos.Droid.View.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Keyguard {

CameraWidgetFrame::FixedSizeFrameLayout::FixedSizeFrameLayout(
    /* [in] */ IContext* context)
    : mWidth(0)
    , mHeight(0)
{
    FrameLayout::constructor(context);
}

ECode CameraWidgetFrame::FixedSizeFrameLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    MeasureChildren(
            MeasureSpec::MakeMeasureSpec(width, IMeasureSpec::EXACTLY),
            MeasureSpec::MakeMeasureSpec(height, IMeasureSpec::EXACTLY));
    return SetMeasuredDimension(width, height);
}

ECode CameraWidgetFrame::MyRunnable::Run()
{
    mHost->TransitionToCamera();
    return NOERROR;
}

ECode CameraWidgetFrame::MyRunnable2::Run()
{
    if (!mHost->mTransitioning) {
        return NOERROR;
    }
    AutoPtr<IHandler> worker;
    GetWorkerHandler((IHandler**)&worker);
    if (worker == NULL) {
        worker = mHost->mHandler;
    }
    mHost->mLaunchCameraStart = SystemClock::uptimeMillis();
    if (DEBUG) {
        Logger::D(TAG, "Launching camera at %d", mHost->mLaunchCameraStart);
    }
    return mHost->mActivityLauncher->LaunchCamera(worker, mSecureCameraActivityStartedRunnable);
}

ECode CameraWidgetFrame::MyRunnable3::Run()
{
    return mHost->mHandler->Post(mTransitionToCameraEndAction);
}

ECode CameraWidgetFrame::MyRunnable4::Run()
{
    mHost->Recover();
    return NOERROR;
}

ECode CameraWidgetFrame::MyRunnable5::Run()
{
    mHost->OnSecureCameraActivityStarted();
    return NOERROR;
}

ECode CameraWidgetFrame::MyRunnable6::Run()
{
    mHost->TransitionToCamera();
    return NOERROR;
}

ECode CameraWidgetFrame::MyKeyguardUpdateMonitorCallback::OnKeyguardVisibilityChanged(
    /* [in] */ Boolean showing)
{
    if (mShowing == showing) {
        return NOERROR;
    }
    mShowing = showing;
    mHost->OnKeyguardVisibilityChanged(mShowing);
    return NOERROR;
}

const String CameraWidgetFrame::TAG("CameraWidgetFrame");// = CameraWidgetFrame.class.getSimpleName();
const Boolean CameraWidgetFrame::DEBUG = KeyguardConstants::DEBUG;
const Int32 CameraWidgetFrame::WIDGET_ANIMATION_DURATION = 250; // ms
const Int32 CameraWidgetFrame::WIDGET_WAIT_DURATION = 400; // ms
const Int32 CameraWidgetFrame::RECOVERY_DELAY = 1000; // ms

CAR_INTERFACE_IMPL(CameraWidgetFrame, KeyguardWidgetFrame, IViewOnClickListener)

CameraWidgetFrame::CameraWidgetFrame()
    : mLaunchCameraStart(0)
    , mActive(FALSE)
    , mTransitioning(FALSE)
    , mDown(FALSE)
    , mUseFastTransition(FALSE)
{
    CHandler::New((IHandler**)&mHandler);
    CPoint::New((Point**)&mRenderedSize);

    mTmpLoc = ArrayOf<Int32>::Alloc(2);

    CRect::New((IRect**)&mInsets);

    mTransitionToCameraRunnable = new MyRunnable(this);
    mTransitionToCameraEndAction = new MyRunnable2(this);
    mPostTransitionToCameraEndAction = new MyRunnable3(this);
    mRecoverRunnable = new MyRunnable4(this);
    mRenderRunnable = new MyRunnable5(this);
    mSecureCameraActivityStartedRunnable = new MyRunnable6(this);
    mCallback = new MyKeyguardUpdateMonitorCallback(this);
}

ECode CameraWidgetFrame::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ICameraWidgetFrameCallbacks* callbacks,
    /* [in] */ KeyguardActivityLauncher* activityLauncher,
    /* [in] */ CameraWidgetInfo* widgetInfo,
    /* [in] */ IView* previewWidget)
{
    KeyguardWidgetFrame::constructor(context);

    mCallbacks = callbacks;
    mActivityLauncher = activityLauncher;
    mWidgetInfo = widgetInfo;

    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::WINDOW_SERVICE, (IInterface**)&obj);
    mWindowManager = IWindowManager::Probe(obj);
    AutoPtr<KeyguardUpdateMonitor> monitor = KeyguardUpdateMonitor::GetInstance(context);
    monitor->RegisterCallback(mCallback);

    mPreview = new FixedSizeFrameLayout(context);
    mPreview->AddView(previewWidget);
    AddView(mPreview);

    AutoPtr<IView> clickBlocker;
    CView::New(context, (IView**)&IView);
    clickBlocker->SetBackgroundColor(IColor::TRANSPARENT);
    clickBlocker->SetOnClickListener(this);
    AddView(clickBlocker);

    String str;
    context->GetString(R::string::keyguard_accessibility_camera, &str);
    SetContentDescription(str);
    if (DEBUG) {
        Logger::D(TAG, "new CameraWidgetFrame instance %d", InstanceId());
    }
    return NOERROR;
}

AutoPtr<CameraWidgetFrame> CameraWidgetFrame::Create(
    /* [in] */ IContext* context,
    /* [in] */ ICameraWidgetFrameCallbacks* callbacks,
    /* [in] */ KeyguardActivityLauncher* launcher)
{
    if (context == NULL || callbacks == NULL || launcher == NULL)
        return NULL;

    AutoPtr<CameraWidgetInfo> widgetInfo;
    launcher->GetCameraWidgetInfo((CameraWidgetInfo**)&widgetInfo);
    if (widgetInfo == NULL) {
        return NULL;
    }
    AutoPtr<IView> previewWidget = GetPreviewWidget(context, widgetInfo);
    if (previewWidget == NULL) {
        return NULL;
    }

    AutoPtr<CameraWidgetFrame> frame = new CameraWidgetFrame();
    frame->constructor(context, callbacks, launcher, widgetInfo, previewWidget);
    return frame;
}

AutoPtr<IView> CameraWidgetFrame::GetPreviewWidget(
    /* [in] */ IContext* context,
    /* [in] */ CameraWidgetInfo* widgetInfo)
{
    return widgetInfo->mLayoutId > 0 ?
            InflateWidgetView(context, widgetInfo) :
            InflateGenericWidgetView(context);
}

AutoPtr<IView> CameraWidgetFrame::InflateWidgetView(
    /* [in] */ IContext* context,
    /* [in] */ CameraWidgetInfo* widgetInfo)
{
    if (DEBUG) Logger::D(TAG, "inflateWidgetView: %s" + (widgetInfo->mContextPackage).string());
    AutoPtr<IView> widgetView;
    //AutoPtr<IException> exception;
    //try {
    ECode ec = NOERROR;
    AutoPtr<IContext> cameraContext;
    FAIL_GOTO(ec = context->CreatePackageContext(widgetInfo->mContextPackage,
            IContext::CONTEXT_RESTRICTED, (IContext**)&cameraContext), ERROR)
    AutoPtr<IInterface> obj;
    FAIL_GOTO(ec = cameraContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&obj), ERROR)
    AutoPtr<ILayoutInflater> cameraInflater = ILayoutInflater::Probe(obj);
    FAIL_GOTO(ec = cameraInflater->CloneInContext(cameraContext, (ILayoutInflater**)&cameraInflater), ERROR)
    FAIL_GOTO(ec = cameraInflater->Inflate(widgetInfo->mLayoutId, NULL, FALSE, (IView**)&widgetView), ERROR)
    // } catch (NameNotFoundException e) {
    //     exception = e;
    // } catch (RuntimeException e) {
    //     exception = e;
    // }
ERROR:
    if (ec != NOERROR) {
        Logger::W(TAG, "Error creating camera widget view %d", ec);
    }
    return widgetView;
}

AutoPtr<IView> CameraWidgetFrame::InflateGenericWidgetView(
    /* [in] */ IContext* context)
{
    if (DEBUG) Logger::D(TAG, "inflateGenericWidgetView");
    AutoPtr<IImageView> iv;
    CImageView::New(context, (IImageView**)&iv);
    iv->SetImageResource(R::drawable::ic_lockscreen_camera);
    iv->SetScaleType(ScaleType.CENTER);
    iv->SetBackgroundColor(Color.argb(127, 0, 0, 0));
    AutoPtr<IView> tmp = IView::Probe(iv);
    return tmp;
}

void CameraWidgetFrame::Render()
{
    AutoPtr<IView> root = GetRootView();

    Int32 _width;
    root->GetWidth(&_width);
    Int32 right;
    mInsets->GetRight(&right);
    Int32 width = _width - right;    // leave room

    Int32 _height;
    root->GetHeight(&_height);
    Int32 bottom;
    mInsets->GetBottom(&bottom);
    Int32 height = _height - bottom; // for bars

    Int32 x;
    mRenderedSize->GetX(&x);
    Int32 y;
    mRenderedSize->GetY(&y);
    if (x == width && y == height) {
        if (DEBUG) {
            // Logger::D(TAG, "Already rendered at size=%sx%s %d%%",
            //     width, height, (int)(100*mPreview.getScaleX())));
        }
        return;
    }
    if (width == 0 || height == 0) {
        return;
    }

    mPreview->mWidth = width;
    mPreview->mHeight = height;
    mPreview->RequestLayout();

    Int32 tWidth;
    GetWidth(&tWidth);
    Int32 tLeft;
    GetPaddingLeft(&tLeft);
    Int32 tRight;
    GetPaddingRight(&tRight);
    Int32 thisWidth = tWidth - tLeft - tRight;

    Int32 tHeight;
    GetHeight(&tHeight);
    Int32 tTop;
    GetPaddingTop(&tTop);
    Int32 tBottom;
    GetPaddingBottom(&tBottom);
    Int32 thisHeight = tHeight - tTop - tBottom;

    Float pvScaleX = (Float) thisWidth / width;
    Float pvScaleY = (Float) thisHeight / height;
    Float pvScale = Math::Min(pvScaleX, pvScaleY);

    Int32 pvWidth = (Int32) (pvScale * width);
    Int32 pvHeight = (Int32) (pvScale * height);

    Float pvTransX = pvWidth < thisWidth ? (thisWidth - pvWidth) / 2 : 0;
    Float pvTransY = pvHeight < thisHeight ? (thisHeight - pvHeight) / 2 : 0;

    Int32 direction;
    mPreview->GetLayoutDirection(&direction);
    Boolean isRtl = direction == LAYOUT_DIRECTION_RTL;
    mPreview->SetPivotX(isRtl ? mPreview->mWidth : 0);
    mPreview->SetPivotY(0);
    mPreview->SetScaleX(pvScale);
    mPreview->SetScaleY(pvScale);
    mPreview->SetTranslationX((isRtl ? -1 : 1) * pvTransX);
    mPreview->SetTranslationY(pvTransY);

    mRenderedSize->Set(width, height);
    if (DEBUG) {
        // Log.d(TAG, String.format("Rendered camera widget size=%sx%s %d%% instance=%s",
        //     width, height, (int)(100*mPreview.getScaleX()), instanceId()));
    }
}

void CameraWidgetFrame::TransitionToCamera()
{
    if (mTransitioning || mDown) return;

    mTransitioning = TRUE;

    EnableWindowExitAnimation(FALSE);

    Int32 navHeight;
    mInsets->GetBottom(&navHeight);
    Int32 navWidth;
    mInsets->GetRight(&navWidth);

    mPreview->GetLocationInWindow(mTmpLoc);
    Int32 height;
    mPreview->GetHeight(&height);
    Int32 y;
    mPreview->GetScaleY(&y);
    Float pvHeight = height * y;
    Float pvCenter = (*mTmpLoc)[1] + pvHeight / 2f;

    AutoPtr<IView> _root = GetRootView();
    AutoPtr<IViewGroup> root = IViewGroup::Probe(_root);

    if (DEBUG) {
        StringBuilder sb;
        sb += "root = ";
        Int32 left;
        root->GetLeft(&left);
        sb += left;
        sb += ",";
        Int32 top;
        root->GetTop(&top);
        sb += top;
        sb += " ";
        Int32 width;
        root->GetWidth(&width);
        sb += width;
        sb += "x";
        Int32 height;
        root->GetHeight(&height);
        sb += height;
        Logger::D(TAG, sb.ToString());
    }

    if (mFullscreenPreview == NULL) {
        mFullscreenPreview = GetPreviewWidget(mContext, mWidgetInfo);
        mFullscreenPreview->SetClickable(FALSE);

        Int32 width;
        root->GetWidth(&width);
        Int32 height;
        root->GetHeight(&height);
        AutoPtr<IFrameLayoutLayoutParams> params;
        CFrameLayoutLayoutParams::New(width - navWidth, height- navHeight, (IFrameLayoutLayoutParams**)&params);
        root->AddView(mFullscreenPreview, params);
    }

    Int32 rheight;
    root->GetHeight(&rheight);
    Float fsHeight = rheight - navHeight;

    Int32 rtop;
    root->GetTop(&rtop);
    Float fsCenter = rtop + fsHeight / 2;

    Float fsScaleY;
    mPreview->GetScaleY(&fsScaleY);
    Float fsTransY = pvCenter - fsCenter;
    Float fsScaleX = fsScaleY;

    mPreview->SetVisibility(IView::GONE);
    mFullscreenPreview->SetVisibility(IView::VISIBLE);
    mFullscreenPreview->SetTranslationY(fsTransY);
    mFullscreenPreview->SetScaleX(fsScaleX);
    mFullscreenPreview->SetScaleY(fsScaleY);

    AutoPtr<IViewPropertyAnimator> animator;
    mFullscreenPreview->Animate((IViewPropertyAnimator**)&animator);
    animator->ScaleX(1)
    animator->ScaleY(1)
    animator->TranslationX(0)
    animator->TranslationY(0)
    animator->SetDuration(WIDGET_ANIMATION_DURATION)
    animator->WithEndAction(mPostTransitionToCameraEndAction)
    animator->Start();

    if (navHeight > 0 || navWidth > 0) {
        const Boolean atBottom = navHeight > 0;
        if (mFakeNavBar == NULL) {
            CView::New(mContext, (IView**)&mFakeNavBar);
            mFakeNavBar->SetBackgroundColor(IColor::BLACK);

            AutoPtr<IFrameLayoutLayoutParams> params;
            CFrameLayoutLayoutParams::New(
                    atBottom ? IFrameLayoutLayoutParams::MATCH_PARENT
                             : navWidth,
                    atBottom ? navHeight
                             : IFrameLayoutLayoutParams::MATCH_PARENT,
                    atBottom ? IGravity::BOTTOM | IGravity::FILL_HORIZONTAL
                             : IGravity::RIGHT | IGravity::FILL_VERTICAL
                    (IFrameLayoutLayoutParams**)&params);
            root->AddView(mFakeNavBar, params);
            mFakeNavBar->SetPivotY(navHeight);
            mFakeNavBar->SetPivotX(navWidth);
        }
        mFakeNavBar->SetAlpha(0f);
        if (atBottom) {
            mFakeNavBar->SetScaleY(0.5f);
        }
        else {
            mFakeNavBar->SetScaleX(0.5f);
        }
        mFakeNavBar->SetVisibility(IView::VISIBLE);

        AutoPtr<IViewPropertyAnimator> animator;
        mFakeNavBar->Animate((IViewPropertyAnimator**)&animator);
        animator->Alpha(1f)
        animator->ScaleY(1f)
        animator->ScaleY(1f)
        animator->SetDuration(WIDGET_ANIMATION_DURATION)
        animator->Start();
    }
    mCallbacks->OnLaunchingCamera();
}

void CameraWidgetFrame::Recover()
{
    if (DEBUG) Logger::D(TAG, "recovering at %d" + SystemClock::uptimeMillis());
    mCallbacks->OnCameraLaunchedUnsuccessfully();
    Reset();
}

ECode CameraWidgetFrame::SetOnLongClickListener(
    /* [in] */ IViewOnLongClickListener* l)
{
    // ignore
    return NOERROR;
}

ECode CameraWidgetFrame::OnClick(
    /* [in] */ IView* v)
{
    if (DEBUG) Logger::D(TAG, "clicked");
    if (mTransitioning) return NOERROR;
    if (mActive) {
        CancelTransitionToCamera();
        TransitionToCamera();
    }
    return NOERROR;
}

ECode CameraWidgetFrame::OnDetachedFromWindow()
{
    if (DEBUG) {
        StringBuilder sb;
        sb += "onDetachedFromWindow: instance ";
        sb += instanceId();
        sb += " at ";
        sb += SystemClock::uptimeMillis()
        Logger::D(TAG, sb.ToString());
    }
    KeyguardWidgetFrame::OnDetachedFromWindow();
    AutoPtr<KeyguardUpdateMonitor> monitor = KeyguardUpdateMonitor::GetInstance(mContext);
    monitor->RemoveCallback(mCallback);
    CancelTransitionToCamera();
    return mHandler->RemoveCallbacks(mRecoverRunnable);
}

ECode CameraWidgetFrame::OnActive(
    /* [in] */ Boolean isActive)
{
    mActive = isActive;
    if (mActive) {
        RescheduleTransitionToCamera();
    }
    else {
        Reset();
    }
    return NOERROR;
}

ECode CameraWidgetFrame::OnUserInteraction(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    if (mTransitioning) {
        if (DEBUG) Logger::D(TAG, "onUserInteraction eaten: mTransitioning");
        *result = TRUE;
        return NOERROR;
    }

    GetLocationOnScreen(mTmpLoc);
    Int32 height;
    GetHeight(&height);
    Int32 rawBottom = (*mTmpLoc)[1] + height;
    Int32 y;
    if ((event->GetRawY(&y), y) > rawBottom) {
        if (DEBUG) Logger::D(TAG, "onUserInteraction eaten: below widget");
        *result = TRUE;
        return NOERROR;
    }

    Int32 action;
    event->GetAction(&action);
    mDown = action == IMotionEvent::ACTION_DOWN || action == IMotionEvent::ACTION_MOVE;
    if (mActive) {
        RescheduleTransitionToCamera();
    }
    if (DEBUG) Logger::D(TAG, "onUserInteraction observed, not eaten");
    *result = FALSE;
    return NOERROR;
}

ECode CameraWidgetFrame::OnFocusLost()
{
    if (DEBUG) Logger::D(TAG, "onFocusLost at %d", SystemClock::uptimeMillis());
    CancelTransitionToCamera();
    KeyguardWidgetFrame::OnFocusLost();
    return NOERROR;
}

ECode CameraWidgetFrame::OnScreenTurnedOff()
{
    if (DEBUG) Logger::D(TAG, "onScreenTurnedOff");
    Reset();
    return NOERROR;
}

void CameraWidgetFrame::RescheduleTransitionToCamera()
{
    if (DEBUG) Logger::D(TAG, "rescheduleTransitionToCamera at %d", SystemClock::UptimeMillis());
    mHandler->RemoveCallbacks(mTransitionToCameraRunnable);
    Int64 duration = mUseFastTransition ? 0 : WIDGET_WAIT_DURATION;
    mHandler->PostDelayed(mTransitionToCameraRunnable, duration);
}

void CameraWidgetFrame::CancelTransitionToCamera()
{
    if (DEBUG) Logger::D(TAG, "cancelTransitionToCamera at %d". SystemClock::UptimeMillis());
    mHandler->RemoveCallbacks(mTransitionToCameraRunnable);
}

void CameraWidgetFrame::OnCameraLaunched()
{
    mCallbacks->OnCameraLaunchedSuccessfully();
    Reset();
}

void CameraWidgetFrame::Reset()
{
    if (DEBUG) Logger::D(TAG, "reset at %d", SystemClock::UptimeMillis());
    mLaunchCameraStart = 0;
    mTransitioning = FALSE;
    mDown = FALSE;
    CancelTransitionToCamera();
    mHandler->RemoveCallbacks(mRecoverRunnable);
    mPreview->SetVisibility(IView::VISIBLE);
    if (mFullscreenPreview != NULL) {
        AutoPtr<IViewPropertyAnimator> animator;
        mFullscreenPreview->Animate((IViewPropertyAnimator**)&animator);
        animator->Cancel();
        mFullscreenPreview->SetVisibility(IView::GONE);
    }
    if (mFakeNavBar != NULL) {
        AutoPtr<IViewPropertyAnimator> animator;
        mFakeNavBar->Animate((IViewPropertyAnimator**)&animator);
        animator->Cancel();
        mFakeNavBar->SetVisibility(IView::GONE);
    }
    EnableWindowExitAnimation(TRUE);
}

ECode CameraWidgetFrame::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (DEBUG) {
        // Log.d(TAG, String.format("onSizeChanged new=%sx%s old=%sx%s at %s",
        //     w, h, oldw, oldh, SystemClock.uptimeMillis()));
    }
    if ((w != oldw && oldw > 0) || (h != oldh && oldh > 0)) {
        // we can't trust the old geometry anymore; force a re-render
        mRenderedSize->SetY(-1);
        mRenderedSize->SetX(-1);
    }
    mHandler->Post(mRenderRunnable);
    KeyguardWidgetFrame::OnSizeChanged(w, h, oldw, oldh);
    return NOERROR;
}

ECode CameraWidgetFrame::OnBouncerShowing(
    /* [in] */ Boolean showing)
{
    if (showing) {
        mTransitioning = FALSE;
        mHandler->Post(mRecoverRunnable);
    }
    return NOERROR;
}

void CameraWidgetFrame::EnableWindowExitAnimation(
    /* [in] */ Boolean isEnabled)
{
    AutoPtr<IView> root;
    GetRootView((IView**)&root);
    AutoPtr<IViewGroupLayoutParams> lp;
    root->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (IWindowManagerLayoutParams::Probe(lp) == NULL) return;

    AutoPtr<IWindowManagerLayoutParams> wlp = IWindowManagerLayoutParams::Probe(lp);
    Int32 newWindowAnimations = isEnabled ? R::style::Animation_LockScreen : 0;
    if (newWindowAnimations != wlp->mWindowAnimations) {
        if (DEBUG) {
            StringBuilder sb;
            sb += "setting windowAnimations to: ";
            sb += newWindowAnimations;
            sb += " at ";
            sb += SystemClock::UptimeMillis();
            Logger::D(TAG, sb.ToString());
        }
        wlp->mWindowAnimations = newWindowAnimations;
        mWindowManager->UpdateViewLayout(root, wlp);
    }
}

void CameraWidgetFrame::OnKeyguardVisibilityChanged(
    /* [in] */ Boolean showing)
{
    if (DEBUG) {
        StringBuilder sb;
        sb += "onKeyguardVisibilityChanged ";
        sb += showing;
        sb += " at ";
        sb += SystemClock::UptimeMillis();
        Logger::D(TAG, sb.ToString());
    }
    if (mTransitioning && !showing) {
        mTransitioning = FALSE;
        mHandler->RemoveCallbacks(mRecoverRunnable);
        if (mLaunchCameraStart > 0) {
            Int64 launchTime = SystemClock::UptimeMillis() - mLaunchCameraStart;
            if (DEBUG) Logger::D(TAG, "Camera took %dms to launch", launchTime);
            mLaunchCameraStart = 0;
            OnCameraLaunched();
        }
    }
}

void CameraWidgetFrame::OnSecureCameraActivityStarted()
{
    if (DEBUG) Logger::D(TAG, "onSecureCameraActivityStarted at %d", SystemClock::UptimeMillis());
    mHandler->PostDelayed(mRecoverRunnable, RECOVERY_DELAY);
}

String CameraWidgetFrame::InstanceId()
{
    Int32 code;
    HashCode(&code);
    return StringUtils::ToHexString(code);
}

ECode CameraWidgetFrame::SetInsets(
    /* [in] */ IRect* insets)
{
    if (DEBUG) Logger::D(TAG, "setInsets: %s", TO_CSTR(insets));
    return mInsets->Set(insets);
}

ECode CameraWidgetFrame::SetUseFastTransition(
    /* [in] */ Boolean useFastTransition)
{
    mUseFastTransition = useFastTransition;
    return NOERROR;
}

} // namespace Keyguard
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos