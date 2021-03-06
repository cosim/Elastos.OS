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

#ifndef __ELASTOS_DROID_INPUTMETHOD_PINYIN_CPINYINENVIRONMENT_H__
#define __ELASTOS_DROID_INPUTMETHOD_PINYIN_CPINYINENVIRONMENT_H__

#include "_Elastos.Droid.InputMethod.Pinyin.h"
#include "Elastos.Droid.Content.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace InputMethod {
namespace Pinyin {

/**
 * Global environment configurations for showing soft keyboard and candidate
 * view. All original dimension values are defined in Float, and the real size
 * is calculated from the Float values of and screen size. In this way, this
 * input method can work even when screen size is changed.
 */
class Environment : public Object
{
public:
    static CARAPI_(AutoPtr<Environment>) GetInstance();

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig,
        /* [in] */ IContext* context);

    CARAPI_(AutoPtr<IConfiguration>) GetConfiguration();

    CARAPI_(Int32) GetScreenWidth();

    CARAPI_(Int32) GetScreenHeight();

    CARAPI_(Int32) GetHeightForCandidates();

    CARAPI_(Float) GetKeyXMarginFactor();

    CARAPI_(Float) GetKeyYMarginFactor();

    CARAPI_(Int32) GetKeyHeight();

    CARAPI_(Int32) GetKeyBalloonWidthPlus();

    CARAPI_(Int32) GetKeyBalloonHeightPlus();

    CARAPI_(Int32) GetSkbHeight();

    CARAPI_(Int32) GetKeyTextSize(
        /* [in] */ Boolean isFunctionKey);

    CARAPI_(Int32) GetBalloonTextSize(
        /* [in] */ Boolean isFunctionKey);

    CARAPI_(Boolean) HasHardKeyboard();

    CARAPI_(Boolean) NeedDebug();

private:
    Environment();

private:
    /**
     * The key height for portrait mode. It is relative to the screen height.
     */
    static const Float KEY_HEIGHT_RATIO_PORTRAIT;

    /**
     * The key height for landscape mode. It is relative to the screen height.
     */
    static const Float KEY_HEIGHT_RATIO_LANDSCAPE;

    /**
     * The height of the candidates area for portrait mode. It is relative to
     * screen height.
     */
    static const Float CANDIDATES_AREA_HEIGHT_RATIO_PORTRAIT;

    /**
     * The height of the candidates area for portrait mode. It is relative to
     * screen height.
     */
    static const Float CANDIDATES_AREA_HEIGHT_RATIO_LANDSCAPE;

    /**
     * How much should the balloon width be larger than width of the real key.
     * It is relative to the smaller one of screen width and height.
     */
    static const Float KEY_BALLOON_WIDTH_PLUS_RATIO;

    /**
     * How much should the balloon height be larger than that of the real key.
     * It is relative to the smaller one of screen width and height.
     */
    static const Float KEY_BALLOON_HEIGHT_PLUS_RATIO;

    /**
     * The text size for normal keys. It is relative to the smaller one of
     * screen width and height.
     */
    static const Float NORMAL_KEY_TEXT_SIZE_RATIO;

    /**
     * The text size for function keys. It is relative to the smaller one of
     * screen width and height.
     */
    static const Float FUNCTION_KEY_TEXT_SIZE_RATIO;

    /**
     * The text size balloons of normal keys. It is relative to the smaller one
     * of screen width and height.
     */
    static const Float NORMAL_BALLOON_TEXT_SIZE_RATIO;

    /**
     * The text size balloons of function keys. It is relative to the smaller
     * one of screen width and height.
     */
    static const Float FUNCTION_BALLOON_TEXT_SIZE_RATIO;

    static AutoPtr<Environment> sInstance;

    Int32 mScreenWidth;
    Int32 mScreenHeight;
    Int32 mKeyHeight;
    Int32 mCandidatesAreaHeight;
    Int32 mKeyBalloonWidthPlus;
    Int32 mKeyBalloonHeightPlus;
    Int32 mNormalKeyTextSize;
    Int32 mFunctionKeyTextSize;
    Int32 mNormalBalloonTextSize;
    Int32 mFunctionBalloonTextSize;
    AutoPtr<IConfiguration> mConfig;
    Boolean mDebug;
};

} // namespace Pinyin
} // namespace InputMethod
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_INPUTMETHOD_PINYIN_CPINYINENVIRONMENT_H__
