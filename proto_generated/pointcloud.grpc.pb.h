// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: pointcloud.proto
#ifndef GRPC_pointcloud_2eproto__INCLUDED
#define GRPC_pointcloud_2eproto__INCLUDED

#include "pointcloud.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/client_context.h>
#include <grpcpp/impl/codegen/completion_queue.h>
#include <grpcpp/impl/codegen/message_allocator.h>
#include <grpcpp/impl/codegen/method_handler.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/server_callback_handlers.h>
#include <grpcpp/impl/codegen/server_context.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace pointcloud {

class NLPointCloudService final {
 public:
  static constexpr char const* service_full_name() {
    return "pointcloud.NLPointCloudService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    // A request to cluster the given point cloud by the groups defined in NLClusterLabel
    // using an appropriate algorithm of your choosing.
    //
    // This is a streaming endpoint, and multiple NLChunkRequests may be sent as part of a single
    // request, just as multiple NLClusterResponses may be returned. Your implementation should start
    // sending cluster responses after all chunk requests have been received.
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>> Cluster(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>>(ClusterRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>> AsyncCluster(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>>(AsyncClusterRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>> PrepareAsyncCluster(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>>(PrepareAsyncClusterRaw(context, cq));
    }
    class async_interface {
     public:
      virtual ~async_interface() {}
      // A request to cluster the given point cloud by the groups defined in NLClusterLabel
      // using an appropriate algorithm of your choosing.
      //
      // This is a streaming endpoint, and multiple NLChunkRequests may be sent as part of a single
      // request, just as multiple NLClusterResponses may be returned. Your implementation should start
      // sending cluster responses after all chunk requests have been received.
      virtual void Cluster(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::pointcloud::NLChunkRequest,::pointcloud::NLClusterResponse>* reactor) = 0;
    };
    typedef class async_interface experimental_async_interface;
    virtual class async_interface* async() { return nullptr; }
    class async_interface* experimental_async() { return async(); }
   private:
    virtual ::grpc::ClientReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* ClusterRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* AsyncClusterRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* PrepareAsyncClusterRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());
    std::unique_ptr< ::grpc::ClientReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>> Cluster(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>>(ClusterRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>> AsyncCluster(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>>(AsyncClusterRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>> PrepareAsyncCluster(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>>(PrepareAsyncClusterRaw(context, cq));
    }
    class async final :
      public StubInterface::async_interface {
     public:
      void Cluster(::grpc::ClientContext* context, ::grpc::ClientBidiReactor< ::pointcloud::NLChunkRequest,::pointcloud::NLClusterResponse>* reactor) override;
     private:
      friend class Stub;
      explicit async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class async* async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* ClusterRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* AsyncClusterRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* PrepareAsyncClusterRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Cluster_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    // A request to cluster the given point cloud by the groups defined in NLClusterLabel
    // using an appropriate algorithm of your choosing.
    //
    // This is a streaming endpoint, and multiple NLChunkRequests may be sent as part of a single
    // request, just as multiple NLClusterResponses may be returned. Your implementation should start
    // sending cluster responses after all chunk requests have been received.
    virtual ::grpc::Status Cluster(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_Cluster : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithAsyncMethod_Cluster() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Cluster() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Cluster(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCluster(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Cluster<Service > AsyncService;
  template <class BaseClass>
  class WithCallbackMethod_Cluster : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithCallbackMethod_Cluster() {
      ::grpc::Service::MarkMethodCallback(0,
          new ::grpc::internal::CallbackBidiHandler< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>(
            [this](
                   ::grpc::CallbackServerContext* context) { return this->Cluster(context); }));
    }
    ~WithCallbackMethod_Cluster() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Cluster(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerBidiReactor< ::pointcloud::NLChunkRequest, ::pointcloud::NLClusterResponse>* Cluster(
      ::grpc::CallbackServerContext* /*context*/)
      { return nullptr; }
  };
  typedef WithCallbackMethod_Cluster<Service > CallbackService;
  typedef CallbackService ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Cluster : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithGenericMethod_Cluster() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Cluster() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Cluster(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Cluster : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawMethod_Cluster() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Cluster() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Cluster(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestCluster(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithRawCallbackMethod_Cluster : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service* /*service*/) {}
   public:
    WithRawCallbackMethod_Cluster() {
      ::grpc::Service::MarkMethodRawCallback(0,
          new ::grpc::internal::CallbackBidiHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
            [this](
                   ::grpc::CallbackServerContext* context) { return this->Cluster(context); }));
    }
    ~WithRawCallbackMethod_Cluster() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Cluster(::grpc::ServerContext* /*context*/, ::grpc::ServerReaderWriter< ::pointcloud::NLClusterResponse, ::pointcloud::NLChunkRequest>* /*stream*/)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::ServerBidiReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* Cluster(
      ::grpc::CallbackServerContext* /*context*/)
      { return nullptr; }
  };
  typedef Service StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef Service StreamedService;
};

}  // namespace pointcloud


#endif  // GRPC_pointcloud_2eproto__INCLUDED
