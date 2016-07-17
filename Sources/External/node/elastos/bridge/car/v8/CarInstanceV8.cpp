
#include "ElNode.h"
#include "config.h"

#if ENABLE(CAR_BRIDGE)

#include <wtf/OwnArrayPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/text/CString.h>
#include <wtf/text/StringBuilder.h>

#include "etl/HashSet.h"
#include "elastos.h"

#if PLATFORM(ANDROID)
#include "npruntime_impl.h"
#include "npruntime_priv.h"
#endif // PLATFORM(ANDROID)

#include "CarUtilityPrivate.h"
#include "CarInstanceV8.h"
#include "CarClassV8.h"
#include "CarMethod.h"
#include "CarMethodV8.h"

#include <utils/Log.h>

EXTERN void (*cbEnqueueUIMessage)(void*, void*, void*);

using namespace JSC::Bindings;

CarInstanceV8::CarInstanceV8(CobjectWrapper* objectWrapper, bool requireAnnotation)
{
    mInstance = objectWrapper;
    mRequireAnnotation = requireAnnotation;
}

void CarInstanceV8::begin()
{
    // getJNIEnv()->PushLocalFrame(NUM_LOCAL_REFS);
}

void CarInstanceV8::end()
{
    // getJNIEnv()->PopLocalFrame(0);
}

CarKlass* CarInstanceV8::getClass() const
{
    if (!mClass) {
        mClass = adoptPtr(new CarClassV8(mInstance->get(), mRequireAnnotation));
    }
    return mClass.get();
}

IInterface* CarInstanceV8::carInstance() const
{
    return mInstance->mInstance;
}

void CarInstanceV8::invokeMethod(const CarMethod* method, CarValue* args, bool* didRaiseUncaughtException)
{
    *didRaiseUncaughtException = false;

    ASSERT(getClass()->methodsNamed(method->name().utf8().data()).find(method) != notFound);

    Int32 numParams = method->numParameters();
    AutoPtr<IInterface> object = carInstance();

    ECode ec = NOERROR;
    AutoPtr<IClassInfo> classInfo;
    AutoPtr<IInterfaceInfo> interfaceInfo;

    AutoPtr<IMethodInfo> aMethod;
    aMethod = method->methodInfo();

    AutoPtr<IArgumentList> argumentList = NULL;
    if (numParams > 0) {
        ec = aMethod->CreateArgumentList((IArgumentList**)&argumentList);
        if (FAILED(ec)) {
            ALOGD("CarInstanceV8::invokeMethod unable to create argumentList on method %p", method);
            *didRaiseUncaughtException = true;
            return;
        }
        else {
           ALOGD("CarInstanceV8::invokeMethod create argumentList on method %p success!", method);
        }

        ArrayOf<IParamInfo*>* paramInfos = ArrayOf<IParamInfo*>::Alloc(numParams);
        aMethod->GetAllParamInfos(paramInfos);

        for (Int32 i = 0; i < numParams; ++i) {
            const char* tmpType = ClassNameFromCarDataType((CarDataType)(args[i].mType));

            AutoPtr<IParamInfo> aParameter = (*paramInfos)[i];
            switch (args[i].mIOAttribute) {
                case ParamIOAttribute_In:
                    switch (args[i].mType) {
                        case CarDataType_Int16:
                            ec = argumentList->SetInputArgumentOfInt16(i, args[i].mInt16Value);
                            break;
                        case CarDataType_Int32:
                            ec = argumentList->SetInputArgumentOfInt32(i, args[i].mInt32Value);
                            break;
                        case CarDataType_Int64:
                            ec = argumentList->SetInputArgumentOfInt64(i, args[i].mInt64Value);
                            break;
                        case CarDataType_Byte:
                            ec = argumentList->SetInputArgumentOfByte(i, args[i].mByteValue);
                            break;
                        case CarDataType_Char32:
                            ec = argumentList->SetInputArgumentOfChar(i, args[i].mCharValue);
                            break;
                        case CarDataType_Float:
                            ec = argumentList->SetInputArgumentOfFloat(i, args[i].mFloatValue);
                            break;
                        case CarDataType_Double:
                            ec = argumentList->SetInputArgumentOfDouble(i, args[i].mDoubleValue);
                            break;
                        case CarDataType_String:
                            ec = argumentList->SetInputArgumentOfString(i, args[i].mStringValue);
                            break;
                        case CarDataType_Boolean:
                            ec = argumentList->SetInputArgumentOfBoolean(i, args[i].mBooleanValue);
                            break;
                        case CarDataType_EMuid:
                            ec = argumentList->SetInputArgumentOfEMuid(i, &args[i].mIid);
                            break;
                        case CarDataType_EGuid:
                            ec = argumentList->SetInputArgumentOfEGuid(i, &args[i].mCid);
                            break;
                        case CarDataType_ECode:
                            ec = argumentList->SetInputArgumentOfECode(i, args[i].mECodeValue);
                            break;
                        case CarDataType_Enum:
                            ec = argumentList->SetInputArgumentOfEnum(i, args[i].mEnumValue);
                            break;
                        case CarDataType_ArrayOf:
                            ec = argumentList->SetInputArgumentOfCarArray(i, args[i].mCarQuintet);
                            break;
                        case CarDataType_LocalPtr:  //deprecated
                        {
                            CarValue* pCarValue = (CarValue*)args[i].mLocalPtr;
                            ec = argumentList->SetInputArgumentOfLocalPtr(i, pCarValue->mCarQuintet ); //normal
                            break;
                        }
                        case CarDataType_Interface:
                        {
                            if (args[i].mObjectWrapper) {
                                AutoPtr<IInterface> tmpInstance = args[i].mObjectWrapper->getInstance();
                                ec = argumentList->SetInputArgumentOfObjectPtr(i, tmpInstance.Get());
                            }
                            else {
                                ec = argumentList->SetInputArgumentOfObjectPtr(i, NULL);
                            }
                            break;
                        }
                        default:
                        {
                            ALOGD("CarInstanceV8::invokeMethod ParamIOAttribute_In CarDataType_Other:%d",args[i].mType);
                            *didRaiseUncaughtException = true;
                            break;
                        }
                    }
                    break;

                case ParamIOAttribute_CalleeAllocOut:
                    switch (args[i].mType) {
                        case CarDataType_LocalPtr:  //deprecated
                        {
                            ec = argumentList->SetOutputArgumentOfLocalPtrPtr(i, &args[i].mLocalPtr);
                            break;
                        }
                        case CarDataType_ArrayOf:
                            ec = argumentList->SetOutputArgumentOfCarArrayPtrPtr(i, &args[i].mCarQuintet);
                            break;
                        default:
                        {
                            ALOGD("CarInstanceV8::invokeMethod ParamIOAttribute_CalleeAllocOut CarDataType_Other:%d", args[i].mType);
                            *didRaiseUncaughtException = true;
                            break;
                        }
                    }
                    break;

                case ParamIOAttribute_CallerAllocOut:
                    switch (args[i].mType) {
                        case CarDataType_Int16:
                            ec = argumentList->SetOutputArgumentOfInt16Ptr(i, &args[i].mInt16Value);
                            break;
                        case CarDataType_Int32:
                            ec = argumentList->SetOutputArgumentOfInt32Ptr(i, &args[i].mInt32Value);
                            break;
                        case CarDataType_Int64:
                            ec = argumentList->SetOutputArgumentOfInt64Ptr(i, &args[i].mInt64Value);
                            break;
                        case CarDataType_Byte:
                            ec = argumentList->SetOutputArgumentOfBytePtr(i, &args[i].mByteValue);
                            break;
                        case CarDataType_Char32:
                            //ec = argumentList->SetOutputArgumentOfChar32Ptr(i, &args[i].mCharValue);
                            ec = argumentList->SetOutputArgumentOfCharPtr(i, &args[i].mCharValue);
                            break;
                        case CarDataType_Float:
                            ec = argumentList->SetOutputArgumentOfFloatPtr(i, &args[i].mFloatValue);
                            break;
                        case CarDataType_Double:
                            ec = argumentList->SetOutputArgumentOfDoublePtr(i, &args[i].mDoubleValue);
                            break;
                        case CarDataType_String:
                            ec = argumentList->SetOutputArgumentOfStringPtr(i, &args[i].mStringValue);
                            break;
                        case CarDataType_Boolean:
                            ec = argumentList->SetOutputArgumentOfBooleanPtr(i, &args[i].mBooleanValue);
                            break;
                        case CarDataType_EMuid:
                            ec = argumentList->SetOutputArgumentOfEMuidPtr(i, &args[i].mIid);
                            break;
                        case CarDataType_EGuid:
                            ec = argumentList->SetOutputArgumentOfEGuidPtr(i, &args[i].mCid);
                            break;
                        case CarDataType_ECode:
                            ec = argumentList->SetOutputArgumentOfECodePtr(i, &args[i].mECodeValue);
                            break;
                        case CarDataType_Enum:
                            ec = argumentList->SetOutputArgumentOfEnumPtr(i, &args[i].mEnumValue);
                            break;
                        case CarDataType_ArrayOf:
                            ec = argumentList->SetOutputArgumentOfCarArrayPtr(i, args[i].mCarQuintet);
                            break;
                        case CarDataType_LocalPtr:  //deprecated
                            ec = argumentList->SetOutputArgumentOfLocalPtrPtr(i, &args[i].mLocalPtr);
                            break;
                        case CarDataType_Interface:
                        {
                            ec = argumentList->SetOutputArgumentOfObjectPtrPtr(i, (IInterface **)&args[i].mObjectValue);
                            break;
                        }
                        default:
                        {
                            ALOGD("CarInstanceV8::invokeMethod ParamIOAttribute_CallerAllocOut CarDataType_Other:%d",args[i].mType);
                            *didRaiseUncaughtException = true;
                            break;
                        }
                    }
                    break;

                default:
                {
                    ALOGD("CarInstanceV8::invokeMethod unknown ParamIOAttribute");
                    *didRaiseUncaughtException = true;
                    break;
                }
            }

            if ( FAILED(ec) || *didRaiseUncaughtException) {
                Elastos::String nameBuf;
                aParameter->GetName(&nameBuf);
                //LOG_ERROR("CarInstanceV8::invokeMethod SetArgument error %s", (const char*)nameBuf);
                ALOGD("CarInstanceV8::invokeMethod SetArgument error %s", (const char*)nameBuf);
                return;
            }

            //aParameter->Release();
            //aParameter = NULL;
        }
        //ArrayOf<IParamInfo*>::Free(paramInfos);
    }

    if(method->isRunOnUiThread()) {
        cbEnqueueUIMessage(object, aMethod, argumentList);
    }
    else {
        ec = aMethod->Invoke(object, argumentList);
        if (FAILED(ec)) {
            Elastos::String methodNameBuf;
            aMethod->GetName(&methodNameBuf);
            *didRaiseUncaughtException = true;
            return;
        }
    }

    return;
}

static void appendClassName(WTF::StringBuilder& builder, const char* className)
{
    char* c = fastStrDup(className);
    char* result = c;
    while (*c) {
        if (*c == '.')
            *c = '/';
        c++;
    }
    builder.append(result);
    fastFree(result);
}

CarValue CarInstanceV8::getField(const CarField* field)
{
/*
    //TODO:
    ASSERT(getClass()->fieldNamed(field->name().utf8().data()) == field);

    WTF::StringBuilder signature;
    signature.append(signatureFromJavaType(field->type()));
    if (field->type() == JavaTypeObject || field->type() == JavaTypeString) {
        appendClassName(signature, field->typeClassName());
        signature.append(';');
     }
 #if PLATFORM(ANDROID)
    return jvalueToJavaValue(getJNIField(javaInstance(), field->type(), field->name().utf8().data(), signature.toString().utf8().data()), field->type(), m_requireAnnotation);
 #else
    return jvalueToJavaValue(getJNIField(javaInstance(), field->type(), field->name().utf8().data(), signature.toString().utf8().data()), field->type());
 #endif
*/
     return CarValue();
}

#endif // ENABLE(CAR_BRIDGE)
