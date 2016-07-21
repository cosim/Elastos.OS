
#include "elastos/droid/systemui/keyguard/LiftToActivateListener.h"
#include "Elastos.Droid.View.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Keyguard {

CAR_INTERFACE_IMPL(LiftToActivateListener, Object, IViewOnHoverListener)

LiftToActivateListener::LiftToActivateListener()
    : mCachedClickableState(FALSE)
{
}

ECode LiftToActivateListener::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::ACCESSIBILITY_SERVICE, (IInterface**)&obj);
    mAccessibilityManager = IAccessibilityManager::Probe(obj);
    return NOERROR;
}

ECode LiftToActivateListener::OnHover(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    // When touch exploration is turned on, lifting a finger while
    // inside the view bounds should perform a click action.
    Boolean res1, res2;
    if ((mAccessibilityManager->IsEnabled(&res1), res1)
            && (mAccessibilityManager->IsTouchExplorationEnabled(&res2), res2)) {
        Int32 mask;
        event->GetActionMasked(&mask);
        switch (mask) {
            case IMotionEvent::ACTION_HOVER_ENTER:
                // Lift-to-type temporarily disables double-tap
                // activation by setting the view as not clickable.
                v->IsClickable(&mCachedClickableState);
                v->SetClickable(FALSE);
                break;
            case IMotionEvent::ACTION_HOVER_EXIT:
            {
                Float _x;
                event->GetX(&_x);
                Int32 x = (Int32)_x;

                Float _y;
                event->GetY(&_y);
                Int32 y = (Int32)_y;

                Int32 left, top, width, right, height, bottom;
                if ((x > (v->GetPaddingLeft(&left), left))
                        && (y > (v->GetPaddingTop(&top), top))
                        && (x < (v->GetWidth(&width), width) - (v->GetPaddingRight(&right), right))
                        && (y < (v->GetHeight(&height), height) - (v->GetPaddingBottom(&bottom), bottom))) {
                    v->PerformClick();
                }
                v->SetClickable(mCachedClickableState);
                break;
            }
        }
    }

    // Pass the event to View.onHoverEvent() to handle accessibility.
    v->OnHoverEvent(event);

    // Consume the event so it doesn't fall through to other views.
    *result = TRUE;
    return NOERROR;
}

} // namespace Keyguard
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos