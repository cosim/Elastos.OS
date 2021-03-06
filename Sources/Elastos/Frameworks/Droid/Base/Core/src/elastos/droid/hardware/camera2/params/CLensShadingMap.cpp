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

#include "elastos/droid/hardware/camera2/params/CLensShadingMap.h"
#include "elastos/droid/hardware/camera2/params/CRggbChannelVector.h"
#include "elastos/droid/hardware/camera2/utils/HashCodeHelpers.h"
#include "elastos/droid/internal/utility/Preconditions.h"
#include <elastos/core/Math.h>
#include <elastos/utility/Arrays.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Internal::Utility::Preconditions;
using Elastos::Droid::Hardware::Camera2::Params::CRggbChannelVector;
using Elastos::Droid::Hardware::Camera2::Utils::HashCodeHelpers;
using Elastos::Core::Math;
using Elastos::Utility::Arrays;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Params {

CAR_INTERFACE_IMPL(CLensShadingMap, Object, ILensShadingMap)

CAR_OBJECT_IMPL(CLensShadingMap)

CLensShadingMap::CLensShadingMap()
    : mRows(0)
    , mColumns(0)
{
}

CLensShadingMap::~CLensShadingMap()
{
}

ECode CLensShadingMap::constructor()
{
    return NOERROR;
}

ECode CLensShadingMap::constructor(
    /* [in] */ ArrayOf<Float>* elements,
    /* [in] */ Int32 rows,
    /* [in] */ Int32 columns)
{
    FAIL_RETURN(Preconditions::CheckArgumentPositive(rows, String("rows must be positive")))
    FAIL_RETURN(Preconditions::CheckArgumentPositive(columns, String("columns must be positive")))
    if (elements == NULL) {
        Logger::E("CLensShadingMap", "elements must not be null");
        return E_NULL_POINTER_EXCEPTION;
    }

    mRows = rows;
    mColumns = columns;
    mElements = elements;

    Int32 value;
    GetGainFactorCount(&value);
    if (elements->GetLength() != value) {
        Logger::E("CLensShadingMap", "elements must be %d length, received %d", value, elements->GetLength());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // Every element must be finite and >= 1.0f
    return Preconditions::CheckArrayElementsInRange(elements, ILensShadingMap::MINIMUM_GAIN_FACTOR,
            Elastos::Core::Math::FLOAT_MAX_VALUE, String("elements"));
}

ECode CLensShadingMap::GetRowCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mRows;
    return NOERROR;
}

ECode CLensShadingMap::GetColumnCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mColumns;
    return NOERROR;
}

ECode CLensShadingMap::GetGainFactorCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mRows * mColumns * IRggbChannelVector::COUNT;
    return NOERROR;
}

ECode CLensShadingMap::GetGainFactor(
    /* [in] */ Int32 colorChannel,
    /* [in] */ Int32 column,
    /* [in] */ Int32 row,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    if (colorChannel < 0 || colorChannel > IRggbChannelVector::COUNT) {
        Logger::E("CLensShadingMap", "colorChannel out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else if (column < 0 || column >= mColumns) {
        Logger::E("CLensShadingMap", "column out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else if (row < 0 || row >= mRows) {
        Logger::E("CLensShadingMap", "row out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *value = (*mElements)[colorChannel + (row * mColumns +  column) * IRggbChannelVector::COUNT ];
    return NOERROR;
}

ECode CLensShadingMap::GetGainFactorVector(
    /* [in] */ Int32 column,
    /* [in] */ Int32 row,
    /* [out] */ IRggbChannelVector** outrggb)
{
    VALIDATE_NOT_NULL(outrggb);

    if (column < 0 || column >= mColumns) {
        Logger::E("CLensShadingMap", "column out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else if (row < 0 || row >= mRows) {
        Logger::E("CLensShadingMap", "row out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    const Int32 offset = (row * mColumns +  column) * IRggbChannelVector::COUNT;

    const Float red =
            (*mElements)[IRggbChannelVector::RED + offset];
    const Float greenEven =
            (*mElements)[IRggbChannelVector::GREEN_EVEN + offset];
    const Float greenOdd =
            (*mElements)[IRggbChannelVector::GREEN_ODD + offset];
    const Float blue =
            (*mElements)[IRggbChannelVector::BLUE + offset];

    return CRggbChannelVector::New(red, greenEven, greenOdd, blue, outrggb);
}

ECode CLensShadingMap::CopyGainFactors(
    /* [in] */ ArrayOf<Float>* destination,
    /* [in] */ Int32 offset)
{
    FAIL_RETURN(Preconditions::CheckArgumentNonnegative(offset, String("offset must not be negative")))
    FAIL_RETURN(Preconditions::CheckNotNull(destination, String("destination must not be null")))
    if (destination == NULL) {
        Logger::E("CLensShadingMap", "destination must not be null");
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 count;
    GetGainFactorCount(&count);
    if (destination->GetLength() + offset < count) {
        Logger::E("CLensShadingMap", "destination too small to fit elements");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return destination->Copy(/*srcPos*/0, mElements, offset, count);
}

ECode CLensShadingMap::Equals(
    /* [in] */ IInterface *obj,
    /* [out] */ Boolean *equal)
{
    VALIDATE_NOT_NULL(equal);

    if (obj == NULL) {
        *equal = FALSE;
        return NOERROR;
    }
    else if (TO_IINTERFACE(this) == TO_IINTERFACE(obj)) {
        *equal = TRUE;
        return NOERROR;
    }
    else if (ILensShadingMap::Probe(obj) != NULL) {
        const AutoPtr<CLensShadingMap> other = (CLensShadingMap*)ILensShadingMap::Probe(obj);
        *equal = mRows == other->mRows
                && mColumns == other->mColumns
                && Arrays::Equals(mElements, other->mElements);
        return NOERROR;
    }
    *equal = FALSE;
    return NOERROR;
}

ECode CLensShadingMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);

    Int32 elemsHash;
    HashCodeHelpers::GetHashCode(mElements, &elemsHash);
    return HashCodeHelpers::GetHashCode(mRows, mColumns, elemsHash, hashCode);
}

} // namespace Params
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
