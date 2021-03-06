// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#include "config.h"
#include "V8ServiceWorkerClients.h"

#include "bindings/v8/ExceptionState.h"
#include "bindings/v8/ScriptPromise.h"
#include "bindings/v8/ScriptState.h"
#include "bindings/v8/V8DOMConfiguration.h"
#include "bindings/v8/V8HiddenValue.h"
#include "bindings/v8/V8ObjectConstructor.h"
#include "core/dom/ContextFeatures.h"
#include "core/dom/Document.h"
#include "platform/RuntimeEnabledFeatures.h"
#include "platform/TraceEvent.h"
#include "wtf/GetPtr.h"
#include "wtf/RefPtr.h"

namespace WebCore {

static void initializeScriptWrappableForInterface(ServiceWorkerClients* object)
{
    if (ScriptWrappable::wrapperCanBeStoredInObject(object))
        ScriptWrappable::fromObject(object)->setTypeInfo(&V8ServiceWorkerClients::wrapperTypeInfo);
    else
        ASSERT_NOT_REACHED();
}

} // namespace WebCore

void webCoreInitializeScriptWrappableForInterface(WebCore::ServiceWorkerClients* object)
{
    WebCore::initializeScriptWrappableForInterface(object);
}

namespace WebCore {
const WrapperTypeInfo V8ServiceWorkerClients::wrapperTypeInfo = { gin::kEmbedderBlink, V8ServiceWorkerClients::domTemplate, V8ServiceWorkerClients::derefObject, 0, 0, 0, V8ServiceWorkerClients::installPerContextEnabledMethods, 0, WrapperTypeObjectPrototype, RefCountedObject };

namespace ServiceWorkerClientsV8Internal {

template <typename T> void V8_USE(T) { }

static void getServicedMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    ServiceWorkerClients* impl = V8ServiceWorkerClients::toNative(info.Holder());
    ScriptState* scriptState = ScriptState::current(info.GetIsolate());
    ScriptPromise result = impl->getServiced(scriptState);
    v8SetReturnValue(info, result.v8Value());
}

static void getServicedMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    TRACE_EVENT_SET_SAMPLING_STATE("Blink", "DOMMethod");
    ServiceWorkerClientsV8Internal::getServicedMethod(info);
    TRACE_EVENT_SET_SAMPLING_STATE("V8", "V8Execution");
}

} // namespace ServiceWorkerClientsV8Internal

static const V8DOMConfiguration::MethodConfiguration V8ServiceWorkerClientsMethods[] = {
    {"getServiced", ServiceWorkerClientsV8Internal::getServicedMethodCallback, 0, 0},
};

static void configureV8ServiceWorkerClientsTemplate(v8::Handle<v8::FunctionTemplate> functionTemplate, v8::Isolate* isolate)
{
    functionTemplate->ReadOnlyPrototype();

    v8::Local<v8::Signature> defaultSignature;
    if (!RuntimeEnabledFeatures::serviceWorkerEnabled())
        defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "", v8::Local<v8::FunctionTemplate>(), V8ServiceWorkerClients::internalFieldCount, 0, 0, 0, 0, 0, 0, isolate);
    else
        defaultSignature = V8DOMConfiguration::installDOMClassTemplate(functionTemplate, "ServiceWorkerClients", v8::Local<v8::FunctionTemplate>(), V8ServiceWorkerClients::internalFieldCount,
            0, 0,
            0, 0,
            V8ServiceWorkerClientsMethods, WTF_ARRAY_LENGTH(V8ServiceWorkerClientsMethods),
            isolate);
    v8::Local<v8::ObjectTemplate> instanceTemplate ALLOW_UNUSED = functionTemplate->InstanceTemplate();
    v8::Local<v8::ObjectTemplate> prototypeTemplate ALLOW_UNUSED = functionTemplate->PrototypeTemplate();

    // Custom toString template
    functionTemplate->Set(v8AtomicString(isolate, "toString"), V8PerIsolateData::from(isolate)->toStringTemplate());
}

v8::Handle<v8::FunctionTemplate> V8ServiceWorkerClients::domTemplate(v8::Isolate* isolate)
{
    return V8DOMConfiguration::domClassTemplate(isolate, const_cast<WrapperTypeInfo*>(&wrapperTypeInfo), configureV8ServiceWorkerClientsTemplate);
}

bool V8ServiceWorkerClients::hasInstance(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->hasInstance(&wrapperTypeInfo, v8Value);
}

v8::Handle<v8::Object> V8ServiceWorkerClients::findInstanceInPrototypeChain(v8::Handle<v8::Value> v8Value, v8::Isolate* isolate)
{
    return V8PerIsolateData::from(isolate)->findInstanceInPrototypeChain(&wrapperTypeInfo, v8Value);
}

ServiceWorkerClients* V8ServiceWorkerClients::toNativeWithTypeCheck(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
    return hasInstance(value, isolate) ? fromInternalPointer(v8::Handle<v8::Object>::Cast(value)->GetAlignedPointerFromInternalField(v8DOMWrapperObjectIndex)) : 0;
}

v8::Handle<v8::Object> wrap(ServiceWorkerClients* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8ServiceWorkerClients>(impl, isolate));
    return V8ServiceWorkerClients::createWrapper(impl, creationContext, isolate);
}

v8::Handle<v8::Object> V8ServiceWorkerClients::createWrapper(PassRefPtr<ServiceWorkerClients> impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    ASSERT(impl);
    ASSERT(!DOMDataStore::containsWrapper<V8ServiceWorkerClients>(impl.get(), isolate));
    if (ScriptWrappable::wrapperCanBeStoredInObject(impl.get())) {
        const WrapperTypeInfo* actualInfo = ScriptWrappable::fromObject(impl.get())->typeInfo();
        // Might be a XXXConstructor::wrapperTypeInfo instead of an XXX::wrapperTypeInfo. These will both have
        // the same object de-ref functions, though, so use that as the basis of the check.
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(actualInfo->derefObjectFunction == wrapperTypeInfo.derefObjectFunction);
    }

    v8::Handle<v8::Object> wrapper = V8DOMWrapper::createWrapper(creationContext, &wrapperTypeInfo, toInternalPointer(impl.get()), isolate);
    if (UNLIKELY(wrapper.IsEmpty()))
        return wrapper;

    installPerContextEnabledProperties(wrapper, impl.get(), isolate);
    V8DOMWrapper::associateObjectWithWrapper<V8ServiceWorkerClients>(impl, &wrapperTypeInfo, wrapper, isolate, WrapperConfiguration::Independent);
    return wrapper;
}

void V8ServiceWorkerClients::derefObject(void* object)
{
    fromInternalPointer(object)->deref();
}

template<>
v8::Handle<v8::Value> toV8NoInline(ServiceWorkerClients* impl, v8::Handle<v8::Object> creationContext, v8::Isolate* isolate)
{
    return toV8(impl, creationContext, isolate);
}

} // namespace WebCore
