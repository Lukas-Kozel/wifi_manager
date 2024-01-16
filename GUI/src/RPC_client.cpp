#include "RPC_client.h"


RPCclient::RPCclient(std::shared_ptr<grpc::Channel> channel): stub_(service::TemperatureService::NewStub(channel)){}

void RPCclient::setTemperatureDataCallback(TemperatureDataCallback callback) {
    temperatureDataCallback_ = callback;
}


void RPCclient::streamData(){
        service::StreamRequest request;
        grpc::ClientContext context;

        auto reader = stub_->streamData(&context,request);
        service::TemperatureData data;

        while(reader->Read(&data)){
            std::cout << "Received temperature: " << data.temperature()
            << ", Timestamp: " << data.timestamp() << std::endl;
            if (temperatureDataCallback_) {
                temperatureDataCallback_(data.temperature(), data.timestamp());
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            std::cerr << "StreamTemperature RPC failed." << std::endl;
        }
}