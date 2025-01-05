#include <iostream>
#include <fstream>
#include <sstream>
#include <grpcpp/grpcpp.h>
#include "pointcloud.grpc.pb.h"
#include <vector>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::ClientReaderWriter;

grpc::ChannelArguments channel_args;

class NLPointCloudClient {
public:
    NLPointCloudClient(std::shared_ptr<Channel> channel) : stub_(pointcloud::NLPointCloudService::NewStub(channel)) {}

    bool loadPointCloud(const std::string& input_file, std::vector<std::tuple<float, float, float>>& points) {
        std::ifstream input(input_file);
        if (!input.is_open()) {
            std::cerr << "Failed to open input file: " << input_file << std::endl;
            return false;
        }

        std::string line;
        bool data_section = false;

        while (std::getline(input, line)) {
            if (data_section) {
                std::istringstream iss(line);
                float x, y, z;
                if (iss >> x >> y >> z) {
                    points.emplace_back(x, y, z);
                }
            } else if (line == "DATA ascii") {
                // After this line, the point data starts
                data_section = true;
            }
        }

        input.close();
        return true;
    }

    void ClusterPointCloud(const std::string& input_file, const std::string& output_file) {
        ClientContext context;
        std::shared_ptr<ClientReaderWriter<pointcloud::NLChunkRequest, pointcloud::NLClusterResponse>> stream(
            stub_->Cluster(&context));

        // Load points from input file
        std::vector<std::tuple<float, float, float>> points;
        if (!loadPointCloud(input_file, points)) {
            std::cerr << "Error loading point cloud data." << std::endl;
            return;
        }

        pointcloud::NLChunkRequest chunk;
        int point_id = 0;

        for (const auto& [x, y, z] : points) {
            pointcloud::NLPoint* point = chunk.add_points();
            point->set_x(x);
            point->set_y(y);
            point->set_z(z);
            point->set_id(point_id++);
        }

        std::cout << "Sending " << chunk.points_size() << " points to the server." << std::endl;

        // Send chunk to server
        stream->Write(chunk);
        stream->WritesDone();

        std::cout << "Sent " << chunk.points_size() << " points to the server." << std::endl;

        // Receive clustering response
        std::ofstream output(output_file);
        pointcloud::NLClusterResponse response;
        while (stream->Read(&response)) {
            // output << response.point_id() << " " << response.label() << std::endl;
            output << response.label() << std::endl;
        }
        output.close();

        Status status = stream->Finish();
        if (!status.ok()) {
            std::cerr << "Cluster rpc failed." << std::endl;
        }

        std::cout << "Client put the data in data/output.labels file"<< std::endl;
    }

private:
    std::unique_ptr<pointcloud::NLPointCloudService::Stub> stub_;
};

int main(int argc, char** argv) {
    if (argc != 9) {
        std::cerr << "Usage: " << argv[0] << " --host <host> --port <port> --input <input_file> --output <output_file>" << std::endl;
        return 1;
    }

    std::string host = argv[2];
    std::string port = argv[4];
    std::string input_file = argv[6];
    std::string output_file = argv[8];
    std::string server_address = host + ":" + port;

    channel_args.SetMaxReceiveMessageSize(-1); // Unlimited message size

    NLPointCloudClient client(grpc::CreateCustomChannel(server_address, grpc::InsecureChannelCredentials(),channel_args));
    std::cout << "Connecting to server at " << server_address << std::endl;
    client.ClusterPointCloud(input_file, output_file);

    return 0;
}
