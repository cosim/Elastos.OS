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

#include "elastos/droid/launcher2/CPagedViewWidgetImageView.h"
#include "Elastos.Droid.Service.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

CAR_INTERFACE_IMPL(CPagedViewWidgetImageView, ImageView, IPagedViewWidgetImageView);

CAR_OBJECT_IMPL(CPagedViewWidgetImageView);

CPagedViewWidgetImageView::CPagedViewWidgetImageView()
    : mAllowRequestLayout(TRUE)
{
}

ECode CPagedViewWidgetImageView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ImageView::constructor(context, attrs);
}

ECode CPagedViewWidgetImageView::RequestLayout()
{
    if (mAllowRequestLayout) {
        return ImageView::RequestLayout();
    }
    return NOERROR;
}

void CPagedViewWidgetImageView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    Int32 tmp;
    canvas->Save(&tmp);
    Int32 x;
    GetScrollX(&x);
    Int32 pleft;
    GetPaddingLeft(&pleft);
    Int32 y;
    GetScrollY(&y);
    Int32 ptop;
    GetPaddingTop(&ptop);
    Int32 right;
    GetRight(&right);
    Int32 left;
    GetLeft(&left);
    Int32 pright;
    GetPaddingRight(&pright);
    Int32 bottom;
    GetBottom(&bottom);
    Int32 top;
    GetTop(&top);
    Int32 pbottom;
    GetPaddingBottom(&pbottom);
    Boolean isNonEmpty;
    canvas->ClipRect(x + pleft,
            y + ptop,
            x + right - left - pright,
            y + bottom - top - pbottom,
            &isNonEmpty);

    ImageView::OnDraw(canvas);
    canvas->Restore();
}

ECode CPagedViewWidgetImageView::GetAllowRequestLayout(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAllowRequestLayout;
    return NOERROR;
}

ECode CPagedViewWidgetImageView::SetAllowRequestLayout(
    /* [in] */ Boolean result)
{
    mAllowRequestLayout = result;
    return NOERROR;
}

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos