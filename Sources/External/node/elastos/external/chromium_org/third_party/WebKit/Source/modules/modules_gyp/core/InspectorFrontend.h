// File is generated by Source/core/inspector/CodeGeneratorInspector.py

// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef InspectorFrontend_h
#define InspectorFrontend_h

#include "InspectorTypeBuilder.h"
#include "core/inspector/InspectorFrontendChannel.h"
#include "platform/JSONValues.h"
#include "wtf/PassRefPtr.h"
#include "wtf/text/WTFString.h"

namespace WebCore {

typedef String ErrorString;

class InspectorFrontend {
public:
    InspectorFrontend(InspectorFrontendChannel*);
    InspectorFrontendChannel* channel() { return m_inspectorFrontendChannel; }

    class Inspector {
    public:
        Inspector(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void evaluateForTestInFrontend(int testCallId, const String& script);
        void inspect(PassRefPtr<TypeBuilder::Runtime::RemoteObject> object, PassRefPtr<JSONObject> hints);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Inspector* inspector() { return &m_inspector; }

    class Memory {
    public:
        Memory(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Memory* memory() { return &m_memory; }

    class Page {
    public:
        Page(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void domContentEventFired(double timestamp);
        void loadEventFired(double timestamp);
        void frameAttached(const TypeBuilder::Page::FrameId& frameId, const TypeBuilder::Page::FrameId& parentFrameId);
        void frameNavigated(PassRefPtr<TypeBuilder::Page::Frame> frame);
        void frameDetached(const TypeBuilder::Page::FrameId& frameId);
        void frameStartedLoading(const TypeBuilder::Page::FrameId& frameId);
        void frameStoppedLoading(const TypeBuilder::Page::FrameId& frameId);
        void frameScheduledNavigation(const TypeBuilder::Page::FrameId& frameId, double delay);
        void frameClearedScheduledNavigation(const TypeBuilder::Page::FrameId& frameId);
        void frameResized();
        void javascriptDialogOpening(const String& message);
        void javascriptDialogClosed();
        void scriptsEnabled(bool isEnabled);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Page* page() { return &m_page; }

    class Runtime {
    public:
        Runtime(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void executionContextCreated(PassRefPtr<TypeBuilder::Runtime::ExecutionContextDescription> context);
        void executionContextDestroyed(int executionContextId);
        void executionContextsCleared();

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Runtime* runtime() { return &m_runtime; }

    class Console {
    public:
        Console(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void messageAdded(PassRefPtr<TypeBuilder::Console::ConsoleMessage> message);
        void messageRepeatCountUpdated(int count, double timestamp);
        void messagesCleared();

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Console* console() { return &m_console; }

    class Network {
    public:
        Network(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void requestWillBeSent(const TypeBuilder::Network::RequestId& requestId, const TypeBuilder::Page::FrameId& frameId, const TypeBuilder::Network::LoaderId& loaderId, const String& documentURL, PassRefPtr<TypeBuilder::Network::Request> request, double timestamp, PassRefPtr<TypeBuilder::Network::Initiator> initiator, PassRefPtr<TypeBuilder::Network::Response> redirectResponse);
        void requestServedFromCache(const TypeBuilder::Network::RequestId& requestId);
        void responseReceived(const TypeBuilder::Network::RequestId& requestId, const TypeBuilder::Page::FrameId& frameId, const TypeBuilder::Network::LoaderId& loaderId, double timestamp, TypeBuilder::Page::ResourceType::Enum type, PassRefPtr<TypeBuilder::Network::Response> response);
        void dataReceived(const TypeBuilder::Network::RequestId& requestId, double timestamp, int dataLength, int encodedDataLength);
        void loadingFinished(const TypeBuilder::Network::RequestId& requestId, double timestamp, double encodedDataLength);
        void loadingFailed(const TypeBuilder::Network::RequestId& requestId, double timestamp, TypeBuilder::Page::ResourceType::Enum type, const String& errorText, const bool* const canceled);
        void webSocketWillSendHandshakeRequest(const TypeBuilder::Network::RequestId& requestId, double timestamp, PassRefPtr<TypeBuilder::Network::WebSocketRequest> request);
        void webSocketHandshakeResponseReceived(const TypeBuilder::Network::RequestId& requestId, double timestamp, PassRefPtr<TypeBuilder::Network::WebSocketResponse> response);
        void webSocketCreated(const TypeBuilder::Network::RequestId& requestId, const String& url);
        void webSocketClosed(const TypeBuilder::Network::RequestId& requestId, double timestamp);
        void webSocketFrameReceived(const TypeBuilder::Network::RequestId& requestId, double timestamp, PassRefPtr<TypeBuilder::Network::WebSocketFrame> response);
        void webSocketFrameError(const TypeBuilder::Network::RequestId& requestId, double timestamp, const String& errorMessage);
        void webSocketFrameSent(const TypeBuilder::Network::RequestId& requestId, double timestamp, PassRefPtr<TypeBuilder::Network::WebSocketFrame> response);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Network* network() { return &m_network; }

    class Database {
    public:
        Database(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void addDatabase(PassRefPtr<TypeBuilder::Database::Database> database);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Database* database() { return &m_database; }

    class IndexedDB {
    public:
        IndexedDB(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    IndexedDB* indexeddb() { return &m_indexeddb; }

    class DOMStorage {
    public:
        DOMStorage(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void domStorageItemsCleared(PassRefPtr<TypeBuilder::DOMStorage::StorageId> storageId);
        void domStorageItemRemoved(PassRefPtr<TypeBuilder::DOMStorage::StorageId> storageId, const String& key);
        void domStorageItemAdded(PassRefPtr<TypeBuilder::DOMStorage::StorageId> storageId, const String& key, const String& newValue);
        void domStorageItemUpdated(PassRefPtr<TypeBuilder::DOMStorage::StorageId> storageId, const String& key, const String& oldValue, const String& newValue);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    DOMStorage* domstorage() { return &m_domstorage; }

    class ApplicationCache {
    public:
        ApplicationCache(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void applicationCacheStatusUpdated(const TypeBuilder::Page::FrameId& frameId, const String& manifestURL, int status);
        void networkStateUpdated(bool isNowOnline);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    ApplicationCache* applicationcache() { return &m_applicationcache; }

    class FileSystem {
    public:
        FileSystem(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    FileSystem* filesystem() { return &m_filesystem; }

    class DOM {
    public:
        DOM(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void documentUpdated();
        void inspectNodeRequested(int nodeId);
        void setChildNodes(int parentId, PassRefPtr<TypeBuilder::Array<TypeBuilder::DOM::Node> > nodes);
        void attributeModified(int nodeId, const String& name, const String& value);
        void attributeRemoved(int nodeId, const String& name);
        void inlineStyleInvalidated(PassRefPtr<TypeBuilder::Array<int> > nodeIds);
        void characterDataModified(int nodeId, const String& characterData);
        void childNodeCountUpdated(int nodeId, int childNodeCount);
        void childNodeInserted(int parentNodeId, int previousNodeId, PassRefPtr<TypeBuilder::DOM::Node> node);
        void childNodeRemoved(int parentNodeId, int nodeId);
        void shadowRootPushed(int hostId, PassRefPtr<TypeBuilder::DOM::Node> root);
        void shadowRootPopped(int hostId, int rootId);
        void pseudoElementAdded(int parentId, PassRefPtr<TypeBuilder::DOM::Node> pseudoElement);
        void pseudoElementRemoved(int parentId, int pseudoElementId);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    DOM* dom() { return &m_dom; }

    class CSS {
    public:
        CSS(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void mediaQueryResultChanged();
        void styleSheetChanged(const TypeBuilder::CSS::StyleSheetId& styleSheetId);
        void styleSheetAdded(PassRefPtr<TypeBuilder::CSS::CSSStyleSheetHeader> header);
        void styleSheetRemoved(const TypeBuilder::CSS::StyleSheetId& styleSheetId);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    CSS* css() { return &m_css; }

    class Timeline {
    public:
        Timeline(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void eventRecorded(PassRefPtr<TypeBuilder::Timeline::TimelineEvent> record);
        void progress(double count);
        void started(const bool* const consoleTimeline);
        void stopped(const bool* const consoleTimeline);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Timeline* timeline() { return &m_timeline; }

    class Debugger {
    public:
        Debugger(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void globalObjectCleared();
        void scriptParsed(const TypeBuilder::Debugger::ScriptId& scriptId, const String& url, int startLine, int startColumn, int endLine, int endColumn, const bool* const isContentScript, const String* const sourceMapURL, const bool* const hasSourceURL);
        void scriptFailedToParse(const String& url, const String& scriptSource, int startLine, int errorLine, const String& errorMessage);
        void breakpointResolved(const TypeBuilder::Debugger::BreakpointId& breakpointId, PassRefPtr<TypeBuilder::Debugger::Location> location);
        // Named after parameter 'reason' while generating command/event paused.
        struct Reason {
            enum Enum {
                XHR = 7,
                DOM = 85,
                EventListener = 86,
                Exception = 87,
                Assert = 50,
                CSPViolation = 88,
                DebugCommand = 89,
                Other = 35,
            };
        }; // struct Reason

        void paused(PassRefPtr<TypeBuilder::Array<TypeBuilder::Debugger::CallFrame> > callFrames, Reason::Enum reason, PassRefPtr<JSONObject> data, PassRefPtr<TypeBuilder::Array<String> > hitBreakpoints, PassRefPtr<TypeBuilder::Debugger::StackTrace> asyncStackTrace);
        void resumed();

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Debugger* debugger() { return &m_debugger; }

    class DOMDebugger {
    public:
        DOMDebugger(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    DOMDebugger* domdebugger() { return &m_domdebugger; }

    class Profiler {
    public:
        Profiler(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void consoleProfileStarted(const String& id, PassRefPtr<TypeBuilder::Debugger::Location> location, const String* const title);
        void consoleProfileFinished(const String& id, PassRefPtr<TypeBuilder::Debugger::Location> location, PassRefPtr<TypeBuilder::Profiler::CPUProfile> profile, const String* const title);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Profiler* profiler() { return &m_profiler; }

    class HeapProfiler {
    public:
        HeapProfiler(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void addHeapSnapshotChunk(const String& chunk);
        void resetProfiles();
        void reportHeapSnapshotProgress(int done, int total, const bool* const finished);
        void lastSeenObjectId(int lastSeenObjectId, double timestamp);
        void heapStatsUpdate(PassRefPtr<TypeBuilder::Array<int> > statsUpdate);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    HeapProfiler* heapprofiler() { return &m_heapprofiler; }

    class Worker {
    public:
        Worker(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void workerCreated(int workerId, const String& url, bool inspectorConnected);
        void workerTerminated(int workerId);
        void dispatchMessageFromWorker(int workerId, PassRefPtr<JSONObject> message);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Worker* worker() { return &m_worker; }

    class Canvas {
    public:
        Canvas(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void contextCreated(const TypeBuilder::Page::FrameId& frameId);
        void traceLogsRemoved(const TypeBuilder::Page::FrameId* const frameId, const TypeBuilder::Canvas::TraceLogId* const traceLogId);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Canvas* canvas() { return &m_canvas; }

    class Input {
    public:
        Input(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Input* input() { return &m_input; }

    class LayerTree {
    public:
        LayerTree(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }
        void layerTreeDidChange(PassRefPtr<TypeBuilder::Array<TypeBuilder::LayerTree::Layer> > layers);
        void layerPainted(const TypeBuilder::LayerTree::LayerId& layerId, PassRefPtr<TypeBuilder::DOM::Rect> clip);

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    LayerTree* layertree() { return &m_layertree; }

    class Geolocation {
    public:
        Geolocation(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Geolocation* geolocation() { return &m_geolocation; }

    class DeviceOrientation {
    public:
        DeviceOrientation(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    DeviceOrientation* deviceorientation() { return &m_deviceorientation; }

    class Tracing {
    public:
        Tracing(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Tracing* tracing() { return &m_tracing; }

    class Power {
    public:
        Power(InspectorFrontendChannel* inspectorFrontendChannel) : m_inspectorFrontendChannel(inspectorFrontendChannel) { }

        void flush() { m_inspectorFrontendChannel->flush(); }
    private:
        InspectorFrontendChannel* m_inspectorFrontendChannel;
    };

    Power* power() { return &m_power; }


private:
    InspectorFrontendChannel* m_inspectorFrontendChannel;
    Inspector m_inspector;
    Memory m_memory;
    Page m_page;
    Runtime m_runtime;
    Console m_console;
    Network m_network;
    Database m_database;
    IndexedDB m_indexeddb;
    DOMStorage m_domstorage;
    ApplicationCache m_applicationcache;
    FileSystem m_filesystem;
    DOM m_dom;
    CSS m_css;
    Timeline m_timeline;
    Debugger m_debugger;
    DOMDebugger m_domdebugger;
    Profiler m_profiler;
    HeapProfiler m_heapprofiler;
    Worker m_worker;
    Canvas m_canvas;
    Input m_input;
    LayerTree m_layertree;
    Geolocation m_geolocation;
    DeviceOrientation m_deviceorientation;
    Tracing m_tracing;
    Power m_power;
};

} // namespace WebCore
#endif // !defined(InspectorFrontend_h)
