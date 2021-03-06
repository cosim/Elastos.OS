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

#include "elastos/droid/ext/frameworkdef.h"
#include "Elastos.Droid.View.h"
#include "elastos/droid/inputmethod/pinyin/Environment.h"
#include "elastos/droid/inputmethod/pinyin/SoftKeyboard.h"
#include "elastos/droid/inputmethod/pinyin/SoftKeyToggle.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace InputMethod {
namespace Pinyin {

const Int32 SoftKeyboard::KeyRow::ALWAYS_SHOW_ROW_ID = -1;
const Int32 SoftKeyboard::KeyRow::DEFAULT_ROW_ID = 0;

SoftKeyboard::SoftKeyboard(
    /* [in] */ Int32 skbXmlId,
    /* [in] */ SkbTemplate* skbTemplate,
    /* [in] */ Int32 skbWidth,
    /* [in] */ Int32 skbHeight)
    : mSkbXmlId(skbXmlId)
    , mCacheFlag(FALSE)
    , mStickyFlag(FALSE)
    , mCacheId(0)
    , mNewlyLoadedFlag(TRUE)
    , mSkbCoreWidth(skbWidth)
    , mSkbCoreHeight(skbHeight)
    , mSkbTemplate(skbTemplate)
    , mIsQwerty(FALSE)
    , mIsQwertyUpperCase(FALSE)
    , mEnabledRowId(0)
    , mKeyXMargin(0.f)
    , mKeyYMargin(0.f)
{
    CRect::New((IRect**)&mTmpRect);
}

void SoftKeyboard::SetFlags(
    /* [in] */ Boolean cacheFlag,
    /* [in] */ Boolean stickyFlag,
    /* [in] */ Boolean isQwerty,
    /* [in] */ Boolean isQwertyUpperCase)
{
    mCacheFlag = cacheFlag;
    mStickyFlag = stickyFlag;
    mIsQwerty = isQwerty;
    mIsQwertyUpperCase = isQwertyUpperCase;
}

Boolean SoftKeyboard::GetCacheFlag()
{
    return mCacheFlag;
}

void SoftKeyboard::SetCacheId(
    /* [in] */ Int32 cacheId)
{
    mCacheId = cacheId;
}

Boolean SoftKeyboard::GetStickyFlag()
{
    return mStickyFlag;
}

void SoftKeyboard::SetSkbBackground(
    /* [in] */ IDrawable* skbBg)
{
    mSkbBg = skbBg;
}

void SoftKeyboard::SetPopupBackground(
    /* [in] */ IDrawable* popupBg)
{
    mPopupBg = popupBg;
}

void SoftKeyboard::SetKeyBalloonBackground(
    /* [in] */ IDrawable* balloonBg)
{
    mBalloonBg = balloonBg;
}

void SoftKeyboard::SetKeyMargins(
    /* [in] */ Float xMargin,
    /* [in] */ Float yMargin)
{
    mKeyXMargin = xMargin;
    mKeyYMargin = yMargin;
}

Int32 SoftKeyboard::GetCacheId()
{
    return mCacheId;
}

void SoftKeyboard::Reset()
{
    mKeyRows.Clear();
}

void SoftKeyboard::SetNewlyLoadedFlag(
    /* [in] */ Boolean newlyLoadedFlag)
{
    mNewlyLoadedFlag = newlyLoadedFlag;
}

Boolean SoftKeyboard::GetNewlyLoadedFlag()
{
    return mNewlyLoadedFlag;
}

void SoftKeyboard::BeginNewRow(
    /* [in] */ Int32 rowId,
    /* [in] */ Float yStartingPos)
{
    AutoPtr<KeyRow> keyRow = new KeyRow();
    keyRow->mRowId = rowId;
    keyRow->mTopF = yStartingPos;
    keyRow->mBottomF = yStartingPos;
    mKeyRows.PushBack(keyRow);
}

Boolean SoftKeyboard::AddSoftKey(
    /* [in] */ SoftKey* softKey)
{
    if (mKeyRows.IsEmpty()) return FALSE;
    AutoPtr<KeyRow> keyRow = mKeyRows[mKeyRows.GetSize() - 1];
    if (NULL == keyRow) return FALSE;

    softKey->SetSkbCoreSize(mSkbCoreWidth, mSkbCoreHeight);
    keyRow->mSoftKeys.PushBack(softKey);
    if (softKey->mTopF < keyRow->mTopF) {
        keyRow->mTopF = softKey->mTopF;
    }
    if (softKey->mBottomF > keyRow->mBottomF) {
        keyRow->mBottomF = softKey->mBottomF;
    }
    return TRUE;
}

Int32 SoftKeyboard::GetSkbXmlId()
{
    return mSkbXmlId;
}

void SoftKeyboard::SetSkbCoreSize(
    /* [in] */ Int32 skbCoreWidth,
    /* [in] */ Int32 skbCoreHeight)
{
    if (skbCoreWidth == mSkbCoreWidth && skbCoreHeight == mSkbCoreHeight) {
        return;
    }
    List<AutoPtr<KeyRow> >::Iterator ator = mKeyRows.Begin();
    for (; ator != mKeyRows.End(); ++ator) {
        AutoPtr<KeyRow> keyRow = *ator;
        keyRow->mBottom = (Int32) (skbCoreHeight * keyRow->mBottomF);
        keyRow->mTop = (Int32) (skbCoreHeight * keyRow->mTopF);

        List<AutoPtr<SoftKey> >::Iterator ator2 = keyRow->mSoftKeys.Begin();
        for (; ator2 != keyRow->mSoftKeys.End(); ++ator2) {
            AutoPtr<SoftKey> softKey = *ator2;
            softKey->SetSkbCoreSize(skbCoreWidth, skbCoreHeight);
        }
    }
    mSkbCoreWidth = skbCoreWidth;
    mSkbCoreHeight = skbCoreHeight;
}

Int32 SoftKeyboard::GetSkbCoreWidth()
{
    return mSkbCoreWidth;
}

Int32 SoftKeyboard::GetSkbCoreHeight()
{
    return mSkbCoreHeight;
}

Int32 SoftKeyboard::GetSkbTotalWidth()
{
    AutoPtr<IRect> padding = GetPadding();
    Int32 left = 0, right = 0;
    padding->GetLeft(&left);
    padding->GetRight(&right);
    return mSkbCoreWidth + left + right;
}

Int32 SoftKeyboard::GetSkbTotalHeight()
{
    AutoPtr<IRect> padding = GetPadding();
    Int32 top = 0, bottom = 0;
    padding->GetTop(&top);
    padding->GetBottom(&bottom);
    return mSkbCoreHeight + top + bottom;
}

Int32 SoftKeyboard::GetKeyXMargin()
{
    AutoPtr<Environment> env = Environment::GetInstance();
    return (Int32) (mKeyXMargin * mSkbCoreWidth * env->GetKeyXMarginFactor());
}

Int32 SoftKeyboard::GetKeyYMargin()
{
    AutoPtr<Environment> env = Environment::GetInstance();
    return (Int32) (mKeyYMargin * mSkbCoreHeight * env->GetKeyYMarginFactor());
}

AutoPtr<IDrawable> SoftKeyboard::GetSkbBackground()
{
    if (NULL != mSkbBg) return mSkbBg;
    return mSkbTemplate->GetSkbBackground();
}

AutoPtr<IDrawable> SoftKeyboard::GetBalloonBackground()
{
    if (NULL != mBalloonBg) return mBalloonBg;
    return mSkbTemplate->GetBalloonBackground();
}

AutoPtr<IDrawable> SoftKeyboard::GetPopupBackground()
{
    if (NULL != mPopupBg) return mPopupBg;
    return mSkbTemplate->GetPopupBackground();
}

Int32 SoftKeyboard::GetRowNum()
{
    return mKeyRows.GetSize();
}

AutoPtr<SoftKeyboard::KeyRow> SoftKeyboard::GetKeyRowForDisplay(
    /* [in] */ Int32 row)
{
    if ((Int32)mKeyRows.GetSize() > row) {
        AutoPtr<KeyRow> keyRow = mKeyRows[row];
        if (KeyRow::ALWAYS_SHOW_ROW_ID == keyRow->mRowId
                || keyRow->mRowId == mEnabledRowId) {
            return keyRow;
        }
    }
    return NULL;
}

AutoPtr<SoftKey> SoftKeyboard::GetKey(
    /* [in] */ Int32 row,
    /* [in] */ Int32 location)
{
    if ((Int32)mKeyRows.GetSize() > row) {
        List< AutoPtr<SoftKey> >& softkeys = mKeyRows[row]->mSoftKeys;
        if ((Int32)softkeys.GetSize() > location) {
            return softkeys[location];
        }
    }
    return NULL;
}

AutoPtr<SoftKey> SoftKeyboard::MapToKey(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    // If the position is inside the rectangle of a certain key, return that
    // key.
    List<AutoPtr<KeyRow> >::Iterator ator = mKeyRows.Begin();
    for (; ator != mKeyRows.End(); ++ator) {
        AutoPtr<KeyRow> keyRow = *ator;
        if (KeyRow::ALWAYS_SHOW_ROW_ID != keyRow->mRowId
                && keyRow->mRowId != mEnabledRowId) continue;
        if (keyRow->mTop > y && keyRow->mBottom <= y) continue;

        List<AutoPtr<SoftKey> >::Iterator ator2 = keyRow->mSoftKeys.Begin();
        for (; ator2 != keyRow->mSoftKeys.End(); ++ator2) {
            AutoPtr<SoftKey> sKey = *ator2;
            if (sKey->mLeft <= x && sKey->mTop <= y && sKey->mRight > x
                    && sKey->mBottom > y) {
                return sKey;
            }
        }
    }

    // If the position is outside the rectangles of all keys, find the
    // nearest one.
    AutoPtr<SoftKey> nearestKey;
    Float nearestDis = Elastos::Core::Math::FLOAT_MAX_VALUE;
    List<AutoPtr<KeyRow> >::Iterator ator3 = mKeyRows.Begin();
    for (; ator3 != mKeyRows.End(); ++ator3) {
        AutoPtr<KeyRow> keyRow = *ator3;
        if (KeyRow::ALWAYS_SHOW_ROW_ID != keyRow->mRowId
                && keyRow->mRowId != mEnabledRowId) continue;
        if (keyRow->mTop > y && keyRow->mBottom <= y) continue;

        List<AutoPtr<SoftKey> >::Iterator ator4 = keyRow->mSoftKeys.Begin();
        for (; ator4 != keyRow->mSoftKeys.End(); ++ator4) {
            AutoPtr<SoftKey> sKey = *ator4;
            Int32 disx = (sKey->mLeft + sKey->mRight) / 2 - x;
            Int32 disy = (sKey->mTop + sKey->mBottom) / 2 - y;
            Float dis = disx * disx + disy * disy;
            if (dis < nearestDis) {
                nearestDis = dis;
                nearestKey = sKey;
            }
        }
    }
    return nearestKey;
}

void SoftKeyboard::SwitchQwertyMode(
    /* [in] */ Int32 toggle_state_id,
    /* [in] */ Boolean upperCase)
{
    if (!mIsQwerty) return;

    List<AutoPtr<KeyRow> >::Iterator ator = mKeyRows.Begin();
    for (; ator != mKeyRows.End(); ++ator) {
        AutoPtr<KeyRow> keyRow = *ator;
        List< AutoPtr<SoftKey> >::Iterator ator2 = keyRow->mSoftKeys.Begin();
        for (; ator2 != keyRow->mSoftKeys.End(); ++ator2) {
            AutoPtr<SoftKey> sKey = *ator2;
            if (sKey->Probe(EIID_ISoftKeyToggle) != NULL) {
                ((SoftKeyToggle*)sKey.Get())->EnableToggleState(toggle_state_id, TRUE);
            }
            if (sKey->mKeyCode >= IKeyEvent::KEYCODE_A
                    && sKey->mKeyCode <= IKeyEvent::KEYCODE_Z) {
                sKey->ChangeCase(upperCase);
            }
        }
    }
}

void SoftKeyboard::EnableToggleState(
    /* [in] */ Int32 toggleStateId,
    /* [in] */ Boolean resetIfNotFound)
{
    List<AutoPtr<KeyRow> >::Iterator ator = mKeyRows.Begin();
    for (; ator != mKeyRows.End(); ++ator) {
        AutoPtr<KeyRow> keyRow = *ator;
        List<AutoPtr<SoftKey> >::Iterator ator2 = keyRow->mSoftKeys.Begin();
        for (; ator2 != keyRow->mSoftKeys.End(); ++ator2) {
            AutoPtr<SoftKey> sKey = *ator2;
            if (sKey->Probe(EIID_ISoftKeyToggle) != NULL) {
                ((SoftKeyToggle*)sKey.Get())->EnableToggleState(toggleStateId, resetIfNotFound);
            }
        }
    }
}

void SoftKeyboard::DisableToggleState(
    /* [in] */ Int32 toggleStateId,
    /* [in] */ Boolean resetIfNotFound)
{
    List<AutoPtr<KeyRow> >::Iterator ator = mKeyRows.Begin();
    for (; ator != mKeyRows.End(); ++ator) {
        AutoPtr<KeyRow> keyRow = *ator;
        List<AutoPtr<SoftKey> >::Iterator ator2 = keyRow->mSoftKeys.Begin();
        for (; ator2 != keyRow->mSoftKeys.End(); ++ator2) {
            AutoPtr<SoftKey> sKey = *ator2;
            if (sKey->Probe(EIID_ISoftKeyToggle) != NULL) {
                ((SoftKeyToggle*)sKey.Get())->DisableToggleState(toggleStateId, resetIfNotFound);
            }
        }
    }
}

void SoftKeyboard::EnableToggleStates(
    /* [in] */ InputModeSwitcher::ToggleStates* toggleStates)
{
    if (NULL == toggleStates) return;

    EnableRow(toggleStates->mRowIdToEnable);

    Boolean isQwerty = toggleStates->mQwerty;
    Boolean isQwertyUpperCase = toggleStates->mQwertyUpperCase;
    Boolean needUpdateQwerty = (isQwerty && mIsQwerty && (mIsQwertyUpperCase != isQwertyUpperCase));
    AutoPtr<ArrayOf<Int32> > states = toggleStates->mKeyStates;
    Int32 statesNum = toggleStates->mKeyStatesNum;

    List<AutoPtr<KeyRow> >::Iterator ator = mKeyRows.Begin();
    for (; ator != mKeyRows.End(); ++ator) {
        AutoPtr<KeyRow> keyRow = *ator;
        if (KeyRow::ALWAYS_SHOW_ROW_ID != keyRow->mRowId
                && keyRow->mRowId != mEnabledRowId) {
            continue;
        }
        List<AutoPtr<SoftKey> >::Iterator ator2 = keyRow->mSoftKeys.Begin();
        for (; ator2 != keyRow->mSoftKeys.End(); ++ator2) {
            AutoPtr<SoftKey> sKey = *ator2;
            if (sKey->Probe(EIID_ISoftKeyToggle) != NULL) {
                for (Int32 statePos = 0; statePos < statesNum; statePos++) {
                    ((SoftKeyToggle*) sKey.Get())->EnableToggleState((*states)[statePos], statePos == 0);
                }
                if (0 == statesNum) {
                    ((SoftKeyToggle*) sKey.Get())->DisableAllToggleStates();
                }
            }
            if (needUpdateQwerty) {
                if (sKey->mKeyCode >= IKeyEvent::KEYCODE_A
                        && sKey->mKeyCode <= IKeyEvent::KEYCODE_Z) {
                    sKey->ChangeCase(isQwertyUpperCase);
                }
            }
        }
    }
    mIsQwertyUpperCase = isQwertyUpperCase;
}

AutoPtr<IRect> SoftKeyboard::GetPadding()
{
    mTmpRect->Set(0, 0, 0, 0);
    AutoPtr<IDrawable> skbBg = GetSkbBackground();
    if (NULL == skbBg) return mTmpRect;
    Boolean result = FALSE;
    skbBg->GetPadding(mTmpRect, &result);
    return mTmpRect;
}

Boolean SoftKeyboard::EnableRow(
    /* [in] */ Int32 rowId)
{
    if (KeyRow::ALWAYS_SHOW_ROW_ID == rowId) return FALSE;

    Boolean enabled = FALSE;
    List<AutoPtr<KeyRow> >::ReverseIterator reAtor = mKeyRows.RBegin();
    for (; reAtor != mKeyRows.REnd(); ++reAtor) {
        AutoPtr<KeyRow> sKey = *reAtor;
        if (sKey->mRowId == rowId) {
            enabled = TRUE;
            break;
        }
    }
    if (enabled) {
        mEnabledRowId = rowId;
    }
    return enabled;
}

ECode SoftKeyboard::ToString(
    /* [out] */ String* str)
{
    assert(0);
//    String str = "------------------SkbInfo----------------------\n";
//    String endStr = "-----------------------------------------------\n";
//    str += "Width: " + String.valueOf(mSkbCoreWidth) + "\n";
//    str += "Height: " + String.valueOf(mSkbCoreHeight) + "\n";
//    str += "KeyRowNum: " + mKeyRows == null ? "0" : String.valueOf(mKeyRows
//            .size())
//            + "\n";
//    if (null == mKeyRows) return str + endStr;
//    int rowNum = mKeyRows.size();
//    for (int row = 0; row < rowNum; row++) {
//        KeyRow keyRow = mKeyRows.get(row);
//        List<SoftKey> softKeys = keyRow.mSoftKeys;
//        int keyNum = softKeys.size();
//        for (int i = 0; i < softKeys.size(); i++) {
//            str += "-key " + String.valueOf(i) + ":"
//                    + softKeys.get(i).toString();
//        }
//    }
//    return str + endStr;
    return NOERROR;
}

String SoftKeyboard::ToShortString()
{
    String str;
    ToString(&str);
    return str;
}

} // namespace Pinyin
} // namespace InputMethod
} // namespace Droid
} // namespace Elastos
