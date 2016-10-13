
#ifndef __ORG_APACHE_HARMONY_SECURITY_UTILS_CARRAYHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_UTILS_CARRAYHELPER_H__

#include "_Org_Apache_Harmony_Security_Utils_CArrayHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

/**
 * Utility class for arrays
 *
 */
CarClass(CArrayHelper)
    , public Singleton
    , public IArrayHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetBytesAsString(
        /* [in] */ ArrayOf<Byte> * pData,
        /* [out] */ String * pStr);

    CARAPI ToString(
        /* [in] */ ArrayOf<Byte> * pArray,
        /* [in] */ const String& prefix,
        /* [out] */ String * pStr);
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_UTILS_CARRAYHELPER_H__
