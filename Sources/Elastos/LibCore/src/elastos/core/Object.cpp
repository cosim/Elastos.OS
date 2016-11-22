
#include "Object.h"
#include "Thread.h"
#include "ClassLoader.h"
#include <utils/Log.h>

using Elastos::Core::Thread;
using Elastos::Core::IThread;
using Elastos::Core::EIID_ISynchronize;

namespace Elastos {
namespace Core {

static Boolean DEBUG = FALSE;

Object::Object()
{
    IncrementDllLockCount();
    mNativeObject = NativeCreateObject();
    mNativeObject->mObjectObj = reinterpret_cast<Int64>(this);
}

Object::~Object()
{
    NativeDestroyObject(mNativeObject);
    DecrementDllLockCount();
}

ECode Object::constructor()
{
    return NOERROR;
}

UInt32 Object::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Object::Release()
{
    return ElRefBase::Release();
}

PInterface Object::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IObject*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_ISynchronize) {
        return (ISynchronize*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    return NULL;
}

ECode Object::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IObject*)this) {
        *iid = EIID_IObject;
    }
    else if (object == (IInterface*)(ISynchronize*)this) {
        *iid = EIID_ISynchronize;
    }
    else if (object == (IInterface*)(IWeakReferenceSource*)this) {
        *iid = EIID_IWeakReferenceSource;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode Object::Aggregate(
    /* [in] */ AggregateType type,
    /* [in] */ IInterface* object)
{
    return E_NOT_IMPLEMENTED;
}

ECode Object::GetDomain(
    /* [out] */ IInterface** object)
{
    return E_NOT_IMPLEMENTED;
}

ECode Object::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return E_NOT_IMPLEMENTED;
}

ECode Object::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

ECode Object::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (IObject*)this == IObject::Probe(other);
    return NOERROR;
}

ECode Object::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(TO_IINTERFACE(this), (IClassInfo**)&classInfo);
    String className("--");
    if (classInfo != NULL) {
        classInfo->GetName(&className);
    }
    info->AppendFormat("Object[0x%08x], Class[%s]", this, className.string());
    return NOERROR;
}

ECode Object::Lock()
{
    // the following codes should be remove
    AutoPtr<IThread> t;
    Thread::Attach((IThread**)&t);
    return NativeLockObject(mNativeObject);
}

ECode Object::Unlock()
{
    // the following codes should be remove
    AutoPtr<IThread> t;
    Thread::Attach((IThread**)&t);
    return NativeUnlockObject(mNativeObject);
}

ECode Object::Notify()
{
    return NativeObjectNotify(mNativeObject);
}

ECode Object::NotifyAll()
{
    return NativeObjectNotifyAll(mNativeObject);
}

ECode Object::Wait()
{
    return Wait(0, 0);
}

ECode Object::Wait(
    /* [in] */ Int64 millis)
{
    return Wait(millis, 0);
}

ECode Object::Wait(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return NativeObjectWait(mNativeObject, millis, nanos, TRUE);
}

ECode Object::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference);
    *weakReference = new WeakReferenceImpl((IObject*)this, CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

AutoPtr<IClassInfo> Object::GetClassInfo(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(obj, (IClassInfo**)&classInfo);
    if (classInfo == NULL) {
        if (DEBUG) ALOGD("error: failed to GetClassInfo with %s. It is not a Car class.", TO_CSTR(obj));
    }
    return classInfo;
}

AutoPtr<IClassLoader> Object::GetClassLoader(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IClassLoader> classLoader;
    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(obj, (IClassInfo**)&classInfo);
    if (classInfo) {
        classLoader = ClassLoader::GetClassLoader(classInfo);
    }
    else {
        if (DEBUG) ALOGW("error: Object::GetClassLoader failed to GetClassLoader with %s. It is not a Car class.", TO_CSTR(obj));
    }
    return classLoader;
}

String Object::GetClassName(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(obj, (IClassInfo**)&classInfo);
    String className;
    if (classInfo != NULL) {
        classInfo->GetName(&className);
    }
    else {
        if (DEBUG) ALOGD("error: failed to GetClassName with %s. It is not a Car class.", TO_CSTR(obj));
    }
    return className;
}

String Object::GetFullClassName(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(obj, (IClassInfo**)&classInfo);
    String fullClassName;
    if (classInfo != NULL) {
        String className, ns;
        classInfo->GetName(&className);
        classInfo->GetNamespace(&ns);
        fullClassName = ns + String(".") + className;
    }
    else {
        if (DEBUG) ALOGD("error: failed to GetFullClassName with %s. It is not a Car class.", TO_CSTR(obj));
    }
    return fullClassName;
}

String Object::GetNamespace(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(obj, (IClassInfo**)&classInfo);
    String ns;
    if (classInfo != NULL) {
        classInfo->GetNamespace(&ns);
    }
    else {
        if (DEBUG) ALOGD("error: failed to GetNamespace with %s. It is not a Car class.", TO_CSTR(obj));
    }
    return ns;
}

String Object::GetModulePath(
    /* [in] */ IInterface* obj)
{
    AutoPtr<IModuleInfo> moduleInfo;
    _CObject_ReflectModuleInfo(obj, (IModuleInfo**)&moduleInfo);
    String path;
    if (moduleInfo != NULL) {
        moduleInfo->GetPath(&path);
    }
    else {
        if (DEBUG) ALOGD("error: failed to GetModulePath with %s. It is not a Car class.", TO_CSTR(obj));
    }
    return path;
}


} // namespace Core
} // namespace Elastos
