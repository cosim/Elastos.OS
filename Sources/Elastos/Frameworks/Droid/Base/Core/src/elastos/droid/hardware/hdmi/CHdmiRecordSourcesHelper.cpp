
#include "elastos/droid/hardware/hdmi/CHdmiRecordSourcesHelper.h"
#include "elastos/droid/hardware/hdmi/HdmiRecordSources.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Hdmi {

CAR_INTERFACE_IMPL(CHdmiRecordSourcesHelper, Singleton, IHdmiRecordSourcesHelper)

CAR_SINGLETON_IMPL(CHdmiRecordSourcesHelper)

ECode CHdmiRecordSourcesHelper::OfOwnSource(
    /* [out] */ IHdmiRecordSourcesOwnSource** source)
{
    VALIDATE_NOT_NULL(source);

    return HdmiRecordSources::OfOwnSource(source);
}

ECode CHdmiRecordSourcesHelper::OfDigitalChannelId(
    /* [in] */ Int32 broadcastSystem,
    /* [in] */ IHdmiRecordSourcesDigitalChannelData* data,
    /* [out] */ IHdmiRecordSourcesDigitalServiceSource** source)
{
    VALIDATE_NOT_NULL(source);

    return HdmiRecordSources::OfDigitalChannelId(broadcastSystem,
            data, source);
}

ECode CHdmiRecordSourcesHelper::OfArib(
    /* [in] */ Int32 aribType,
    /* [in] */ IHdmiRecordSourcesAribData* data,
    /* [out] */ IHdmiRecordSourcesDigitalServiceSource** source)
{
    VALIDATE_NOT_NULL(source);

    return HdmiRecordSources::OfArib(aribType, data, source);
}

ECode CHdmiRecordSourcesHelper::OfAtsc(
    /* [in] */ Int32 atscType,
    /* [in] */ IHdmiRecordSourcesAtscData* data,
    /* [out] */ IHdmiRecordSourcesDigitalServiceSource** source)
{
    VALIDATE_NOT_NULL(source);

    return HdmiRecordSources::OfAtsc(atscType, data, source);
}

ECode CHdmiRecordSourcesHelper::OfDvb(
    /* [in] */ Int32 dvbType,
    /* [in] */ IHdmiRecordSourcesDvbData* data,
    /* [out] */ IHdmiRecordSourcesDigitalServiceSource** source)
{
    VALIDATE_NOT_NULL(source);

    return HdmiRecordSources::OfDvb(dvbType, data, source);
}

ECode CHdmiRecordSourcesHelper::OfAnalogue(
    /* [in] */ Int32 broadcastType,
    /* [in] */ Int32 frequency,
    /* [in] */ Int32 broadcastSystem,
    /* [out] */ IHdmiRecordSourcesAnalogueServiceSource** source)
{
    VALIDATE_NOT_NULL(source);

    return HdmiRecordSources::OfAnalogue(broadcastType, frequency,
            broadcastSystem,source);
}

ECode CHdmiRecordSourcesHelper::OfExternalPlug(
    /* [in] */ Int32 plugNumber,
    /* [out] */ IHdmiRecordSourcesExternalPlugData** data)
{
    VALIDATE_NOT_NULL(data);

    return HdmiRecordSources::OfExternalPlug(plugNumber, data);
}

ECode CHdmiRecordSourcesHelper::OfExternalPhysicalAddress(
    /* [in] */ Int32 physicalAddress,
    /* [out] */ IHdmiRecordSourcesExternalPhysicalAddress** address)
{
    VALIDATE_NOT_NULL(address);

    return HdmiRecordSources::OfExternalPhysicalAddress(physicalAddress,
            address);
}

ECode CHdmiRecordSourcesHelper::CheckRecordSource(
    /* [in] */ ArrayOf<Byte>* recordSource,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    return HdmiRecordSources::CheckRecordSource(recordSource, result);
}

} // namespace Hdmi
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
