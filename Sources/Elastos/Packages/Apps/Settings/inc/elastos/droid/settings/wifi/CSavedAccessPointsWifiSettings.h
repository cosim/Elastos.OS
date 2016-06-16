
#ifndef __ELASTOS_DROID_SETTINGS_WIFI_CSAVEDACCESSPOINTSWIFISETTINGS_H__
#define __ELASTOS_DROID_SETTINGS_WIFI_CSAVEDACCESSPOINTSWIFISETTINGS_H__

#include "_Elastos_Droid_Settings_Wifi_CSavedAccessPointsWifiSettings.h"
#include "elastos/droid/settings/wifi/SavedAccessPointsWifiSettings.h"

namespace Elastos {
namespace Droid {
namespace Settings {
namespace Wifi {

CarClass(CSavedAccessPointsWifiSettings)
    , public SavedAccessPointsWifiSettings
{
public:
    CAR_OBJECT_DECL();
};

} // namespace Wifi
} // namespace Settings
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SETTINGS_WIFI_CSAVEDACCESSPOINTSWIFISETTINGS_H__