#include "mocks.h"

#include "envoy/buffer/buffer.h"
#include "envoy/event/dispatcher.h"

using testing::_;
using testing::Invoke;
using testing::Return;
using testing::ReturnRef;
using testing::SaveArg;

namespace Http {

MockConnectionManagerConfig::MockConnectionManagerConfig() {
  ON_CALL(*this, routeConfig()).WillByDefault(ReturnRef(route_config_));
  ON_CALL(*this, generateRequestId()).WillByDefault(Return(true));
}

MockConnectionManagerConfig::~MockConnectionManagerConfig() {}

MockConnectionCallbacks::MockConnectionCallbacks() {}
MockConnectionCallbacks::~MockConnectionCallbacks() {}

MockServerConnectionCallbacks::MockServerConnectionCallbacks() {}
MockServerConnectionCallbacks::~MockServerConnectionCallbacks() {}

MockStreamDecoder::MockStreamDecoder() {}
MockStreamDecoder::~MockStreamDecoder() {}

MockStreamCallbacks::MockStreamCallbacks() {}
MockStreamCallbacks::~MockStreamCallbacks() {}

MockStream::MockStream() {
  ON_CALL(*this, addCallbacks(_))
      .WillByDefault(
          Invoke([this](StreamCallbacks& callbacks) -> void { callbacks_.push_back(&callbacks); }));

  ON_CALL(*this, removeCallbacks(_))
      .WillByDefault(
          Invoke([this](StreamCallbacks& callbacks) -> void { callbacks_.remove(&callbacks); }));

  ON_CALL(*this, resetStream(_))
      .WillByDefault(Invoke([this](StreamResetReason reason) -> void {
        for (StreamCallbacks* callbacks : callbacks_) {
          callbacks->onResetStream(reason);
        }
      }));
}

MockStream::~MockStream() {}

MockStreamEncoder::MockStreamEncoder() {
  ON_CALL(*this, getStream()).WillByDefault(ReturnRef(stream_));
}

MockStreamEncoder::~MockStreamEncoder() {}

MockServerConnection::MockServerConnection() {
  ON_CALL(*this, protocolString()).WillByDefault(ReturnRef(protocol_));
}

MockServerConnection::~MockServerConnection() {}

MockClientConnection::MockClientConnection() {}
MockClientConnection::~MockClientConnection() {}

MockFilterChainFactory::MockFilterChainFactory() {}
MockFilterChainFactory::~MockFilterChainFactory() {}

template <class T> static void initializeMockStreamFilterCallbacks(T& callbacks) {
  ON_CALL(callbacks, addResetStreamCallback(_))
      .WillByDefault(SaveArg<0>(&callbacks.reset_callback_));
  ON_CALL(callbacks, dispatcher()).WillByDefault(ReturnRef(callbacks.dispatcher_));
  ON_CALL(callbacks, requestInfo()).WillByDefault(ReturnRef(callbacks.request_info_));
  ON_CALL(callbacks, routeTable()).WillByDefault(ReturnRef(callbacks.route_table_));
}

MockStreamDecoderFilterCallbacks::MockStreamDecoderFilterCallbacks() {
  initializeMockStreamFilterCallbacks(*this);
}

MockStreamDecoderFilterCallbacks::~MockStreamDecoderFilterCallbacks() {}

MockStreamEncoderFilterCallbacks::MockStreamEncoderFilterCallbacks() {
  initializeMockStreamFilterCallbacks(*this);
}

MockStreamEncoderFilterCallbacks::~MockStreamEncoderFilterCallbacks() {}

MockStreamDecoderFilter::MockStreamDecoderFilter() {
  ON_CALL(*this, setDecoderFilterCallbacks(_))
      .WillByDefault(Invoke([this](StreamDecoderFilterCallbacks& callbacks) -> void {
        callbacks_ = &callbacks;
        callbacks_->addResetStreamCallback([this]() -> void { reset_stream_called_.ready(); });
      }));
}

MockStreamDecoderFilter::~MockStreamDecoderFilter() {}

MockStreamEncoderFilter::MockStreamEncoderFilter() {
  ON_CALL(*this, setEncoderFilterCallbacks(_))
      .WillByDefault(Invoke([this](StreamEncoderFilterCallbacks& callbacks)
                                -> void { callbacks_ = &callbacks; }));
}

MockStreamEncoderFilter::~MockStreamEncoderFilter() {}

MockAsyncClient::MockAsyncClient() {}
MockAsyncClient::~MockAsyncClient() {}

MockAsyncClientCallbacks::MockAsyncClientCallbacks() {}
MockAsyncClientCallbacks::~MockAsyncClientCallbacks() {}

MockAsyncClientRequest::MockAsyncClientRequest(MockAsyncClient* client) : client_(client) {}
MockAsyncClientRequest::~MockAsyncClientRequest() { client_->onRequestDestroy(); }

} // Http

namespace Http {
namespace ConnectionPool {

MockCancellable::MockCancellable() {}
MockCancellable::~MockCancellable() {}

MockInstance::MockInstance() {}
MockInstance::~MockInstance() {}

} // ConnectionPool
} // Http

namespace Http {
namespace AccessLog {

MockInstance::MockInstance() {}
MockInstance::~MockInstance() {}

MockRequestInfo::MockRequestInfo() {}
MockRequestInfo::~MockRequestInfo() {}

} // AccessLog
} // Http
