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

#ifndef __ELASTOS_DROID_DIALERBIND_OBJECTFACTORY_H__
#define __ELASTOS_DROID_DIALERBIND_OBJECTFACTORY_H__

#include "_Elastos.Droid.Dialer.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Content.h"
#include "elastos/droid/dialer/calllog/CallLogAdapter.h"
#include "elastos/droid/dialer/calllog/ContactInfoHelper.h"

using Elastos::Droid::App::IDialogFragment;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Dialer::CallLog::CallLogAdapter;
using Elastos::Droid::Dialer::CallLog::ICallFetcher;
using Elastos::Droid::Dialer::CallLog::ICallItemExpandedListener;
using Elastos::Droid::Dialer::CallLog::ICallLogAdapterOnReportButtonClickListener;
using Elastos::Droid::Dialer::CallLog::ContactInfoHelper;
using Elastos::Droid::Dialer::Service::ICachedNumberLookupService;

namespace Elastos {
namespace Droid {
namespace DialerBind {

/**
 * Default static binding for various objects.
 */
class ObjectFactory
{
public:
    static CARAPI_(AutoPtr<ICachedNumberLookupService>) NewCachedNumberLookupService();

    static CARAPI_(AutoPtr<CallLogAdapter>) NewCallLogAdapter(
        /* [in] */ IContext* context,
        /* [in] */ ICallFetcher* callFetcher,
        /* [in] */ ContactInfoHelper* contactInfoHelper,
        /* [in] */ ICallItemExpandedListener* callItemExpandedListener,
        /* [in] */ ICallLogAdapterOnReportButtonClickListener* onReportButtonClickListener,
        /* [in] */ Boolean isCallLog);

    static CARAPI_(AutoPtr<IDialogFragment>) GetReportDialogFragment(
        /* [in] */ const String& name);
};

} // DialerBind
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_DIALERBIND_OBJECTFACTORY_H__
