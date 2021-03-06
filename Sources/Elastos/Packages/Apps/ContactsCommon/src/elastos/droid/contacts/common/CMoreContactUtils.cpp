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

#include "CMoreContactUtils.h"
#include "MoreContactUtils.h"

namespace Elastos{
namespace Apps{
namespace Contacts {
namespace Common {

CAR_INTERFACE_IMPL(CMoreContactUtils, Singleton, IMoreContactUtils)

CAR_SINGLETON_IMPL(CMoreContactUtils)

ECode CMoreContactUtils::ShouldCollapse(
    /* [in] */ ICharSequence* mimetype1,
    /* [in] */ ICharSequence* data1,
    /* [in] */ ICharSequence* mimetype2,
    /* [in] */ ICharSequence* data2,
    /* [out] */ Boolean* result)
{
    VALUE_NOT_NULL(result);

    *result = MoreContactUtils::ShouldCollapse(
            mimetype1, data1, mimetype2, data2);
    return NOERROR;
}

ECode CMoreContactUtils::GetTargetRectFromView(
    /* [in] */ IView* view,
    /* [out] */ IRect** rect)
{
    VALUE_NOT_NULL(rect);

    AutoPtr<IRect> result = MoreContactUtils::GetTargetRectFromView(view);
    *rect = result;
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

ECode CMoreContactUtils::CreateHeaderView(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textResourceId,
    /* [out] */ ITextView** textView)
{
    VALUE_NOT_NULL(textView);

    AutoPtr<ITextView> result = MoreContactUtils::CreateHeaderView(
                context, textResourceId);
    *textView = result;
    REFCOUNT_ADD(*textView);
    return NOERROR;
}

ECode CMoreContactUtils::SetHeaderViewBottomPadding(
    /* [in] */ IContext* context,
    /* [in] */ ITextView* textView,
    /* [in] */ Boolean isFirstRow)
{
    MoreContactUtils::SetHeaderViewBottomPadding(context, textView, isFirstRow);
    return NOERROR;
}

ECode CMoreContactUtils::GetInvitableIntent(
    /* [in] */ IAccountType* accountType,
    /* [in] */ IUri* lookupUri,
    /* [out] */ IIntent** intent)
{
    VALUE_NOT_NULL(intent);

    AutoPtr<IIntent> result = MoreContactUtils::GetInvitableIntent(
                accountType, lookupUri);
    *intent = result;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

} // Common
} // Contacts
} // Apps
} // Elastos
