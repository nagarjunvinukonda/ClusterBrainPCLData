#include <iostream>
#include <grpcpp/grpcpp.h>
#include "pointcloud.grpc.pb.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <random>
#include <numeric>
#include <thread>
#include <mutex>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;


class NLPointCloudServiceImpl final : public pointcloud::NLPointCloudService::Service {
public:
    //Read the data chunk from Client, Cluster the data and send back to Client. 
    Status Cluster(ServerContext* context, ServerReaderWriter<pointcloud::NLClusterResponse, pointcloud::NLChunkRequest>* stream) override {
        pointcloud::NLChunkRequest chunk;
        std::vector<pointcloud::NLPoint> points;

        // Read all incoming chunks
        while (stream->Read(&chunk)) {
            for (const auto& point : chunk.points()) {
                points.push_back(point);
            }
        }

        std::cout << "Received " << points.size() << " points from the client." << std::endl;
        
        // Perform clustering
        std::vector<pointcloud::NLClusterResponse> clustered_points = ClusterPointCloud(points);

        // Write the clustering response back to the client
        for (const auto& response : clustered_points) {
            // std::cout << "Point ID " << response.point_id() << " classified as " << response.label() << std::endl;
            stream->Write(response);
        }

        return Status::OK;
    }


private:

    // Helper function to calculate Euclidean distance between two points
    float CalculateDistance(const pointcloud::NLPoint& p1, const pointcloud::NLPoint& p2) {
        return std::sqrt(std::pow(p1.x() - p2.x(), 2) + std::pow(p1.y() - p2.y(), 2) + std::pow(p1.z() - p2.z(), 2));
    }

    std::vector<pointcloud::NLClusterResponse> ClusterPointCloud(const std::vector<pointcloud::NLPoint>& points) {
        std::vector<pointcloud::NLClusterResponse> responses;

        std::cout<<"Performing Clustering, Please wait ... "<<std::endl;  

        //Input params ti intialize the proper density cluster
        const float cluster_tolerance = 10.0;  // Increased distance threshold for clustering
        const int min_cluster_size = 5;        // Reduced minimum number of points to form a cluster
        const int max_cluster_size = 160000;    // Maximum number of points in a cluster

        // Initialize labels to UNKNOWN
        std::vector<int> labels(points.size(), -1);  // -1 indicates unvisited, -2 indicates noise
        int current_label = 0;
        std::vector<std::vector<size_t>> cluster_indices;
        std::vector<std::vector<size_t>> neighbors_list(points.size());
        std::mutex neighbors_mutex;

        // Perform neighbor search in parallel using multiple threads
        size_t num_threads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        size_t points_per_thread = points.size() / num_threads;

         // Thread-safe lambda function to perform neighbor search in parallel
        auto FindNeighbors = [&](size_t start_idx, size_t end_idx) {
            for (size_t i = start_idx; i < end_idx; ++i) {
                std::vector<size_t> neighbors;
                for (size_t j = 0; j < points.size(); ++j) {
                    if (i != j && CalculateDistance(points[i], points[j]) <= cluster_tolerance) {
                        neighbors.push_back(j);
                    }
                }
                // Protect shared resource
                std::lock_guard<std::mutex> lock(neighbors_mutex);
                neighbors_list[i] = neighbors;
            }
        };
    
        for (size_t t = 0; t < num_threads; ++t) {
            size_t start_idx = t * points_per_thread;
            size_t end_idx = (t == num_threads - 1) ? points.size() : (t + 1) * points_per_thread;
            threads.emplace_back(FindNeighbors, start_idx, end_idx);
        }


        for (auto& thread : threads) {
            thread.join();
        }

        // DBSCAN-like Clustering
        for (size_t i = 0; i < points.size(); ++i) {
            if (labels[i] != -1) {
                continue;  // Point is already visited
            }

            // Find neighbors of the current point
            const std::vector<size_t>& neighbors = neighbors_list[i];
            if (neighbors.size() < min_cluster_size) {
                labels[i] = -2;  // Mark as noise if th neighbor size is than our cluster size requirments 
                continue;
            }

            // Start a new cluster
            labels[i] = current_label;
            std::queue<size_t> to_visit;
            std::vector<size_t> cluster = {i};
            for (size_t neighbor : neighbors) {
                if (labels[neighbor] == -1) {
                    to_visit.push(neighbor);
                    labels[neighbor] = current_label;
                    cluster.push_back(neighbor);
                } else if (labels[neighbor] == -2) {
                    labels[neighbor] = current_label;  // Change noise to border point
                    cluster.push_back(neighbor);
                }
            }

            // Expand the cluster
            while (!to_visit.empty()) {
                size_t current_point = to_visit.front();
                to_visit.pop();

                // Find neighbors of the current point
                const std::vector<size_t>& current_neighbors = neighbors_list[current_point];
                if (current_neighbors.size() >= min_cluster_size) {
                    for (size_t neighbor : current_neighbors) {
                        if (labels[neighbor] == -1 || labels[neighbor] == -2) {
                            if (labels[neighbor] == -1) {
                                to_visit.push(neighbor);
                            }
                            labels[neighbor] = current_label;
                            cluster.push_back(neighbor);
                        }
                    }
                }
            }

            if (cluster.size() >= min_cluster_size && cluster.size() <= max_cluster_size) {
                cluster_indices.push_back(cluster);
                current_label++;
            } else {
                for (size_t idx : cluster) {
                    labels[idx] = -2;  // Revert to noise if cluster size is out of bounds
                }
            }
        }

        // Calculate centroid z-value for each cluster
        std::vector<float> cluster_z_values(cluster_indices.size(), 0.0);
        std::vector<int> cluster_sizes(cluster_indices.size(), 0);
        for (size_t i = 0; i < cluster_indices.size(); ++i) {
            for (const auto& idx : cluster_indices[i]) {
                cluster_z_values[i] += points[idx].z();
                cluster_sizes[i]++;
            }
            if (cluster_sizes[i] > 0) {
                cluster_z_values[i] /= cluster_sizes[i];  // Calculate centroid z-value for each cluster
            }
        }
        
        // Sort clusters by their centroid z-value
        std::vector<int> sorted_clusters(cluster_indices.size());
        std::iota(sorted_clusters.begin(), sorted_clusters.end(), 0);
        std::sort(sorted_clusters.begin(), sorted_clusters.end(), [&](int a, int b) {
            return cluster_z_values[a] < cluster_z_values[b];
        });

        int dura_label = sorted_clusters.front();  // Closest to z = 0 is Dura
        int cortical_label = sorted_clusters.back();  // Farthest from z = 0 is Cortical Surface

        // find the Z_threshold the is the avg z height of top-most cluster and bottom-most cluster
        float z_threshold =0.0;
        z_threshold= (cluster_z_values[dura_label] + cluster_z_values[cortical_label])/2;

        // Assign labels to clusters based on z-value threshold
        for (size_t i = 0; i < points.size(); ++i) {
            pointcloud::NLClusterResponse response;
            response.set_point_id(points[i].id());

            if (labels[i] == -1) {
                response.set_label(pointcloud::NLClusterLabel::UNKNOWN);  // Noise or unclassified
            } else if (cluster_z_values[labels[i]] >= z_threshold && cluster_sizes[labels[i]] > 500) {
                response.set_label(pointcloud::NLClusterLabel::CORTICAL_SURFACE);
            } else if (cluster_z_values[labels[i]] < z_threshold && cluster_sizes[labels[i]] > 500) {
                response.set_label(pointcloud::NLClusterLabel::DURA);
            } else {
                response.set_label(pointcloud::NLClusterLabel::UNKNOWN);
            }

            responses.push_back(response);
        }

        std::cout<<"Done Clustering "<<std::endl;   

        return responses;
    }

};
void RunServer() {
    std::string server_address("0.0.0.0:50051");
    NLPointCloudServiceImpl service;

    ServerBuilder builder;
    builder.SetMaxReceiveMessageSize(-1); // Unlimited message size
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}