#include <iostream>
#include <grpcpp/grpcpp.h>
#include "pointcloud.grpc.pb.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <random>
#include <numeric>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/segmentation/extract_clusters.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;


class NLPointCloudServiceImpl final : public pointcloud::NLPointCloudService::Service {
public:
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

        std::cout<<"Started Clustering, Please wait ... "<<std::endl;   

        // Convert input points to PCL point cloud
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        for (const auto& point : points) {
            pcl::PointXYZ pcl_point;
            pcl_point.x = point.x();
            pcl_point.y = point.y();
            pcl_point.z = point.z();
            cloud->points.push_back(pcl_point);
        }

        // Create a KD-Tree
        pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
        if (cloud->points.empty()) {
            return responses;  // No points to process
        }
        tree->setInputCloud(cloud);

        // Perform DBSCAN clustering using EuclideanClusterExtraction
        std::vector<pcl::PointIndices> cluster_indices;
        pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
        ec.setClusterTolerance(10);  // Increased distance threshold for clustering
        ec.setMinClusterSize(5);     // Reduced minimum number of points to form a cluster
        ec.setMaxClusterSize(160000);   // Maximum number of points in a cluster
        ec.setSearchMethod(tree);
        ec.setInputCloud(cloud);
        ec.extract(cluster_indices);

        // Calculate centroid z-value for each cluster
        std::vector<float> cluster_z_values(cluster_indices.size(), 0.0);
        std::vector<int> cluster_sizes(cluster_indices.size(), 0);
        for (size_t i = 0; i < cluster_indices.size(); ++i) {
            for (const auto& idx : cluster_indices[i].indices) {
                cluster_z_values[i] += cloud->points[idx].z;
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

        float z_threshold =0.0;
        z_threshold= (cluster_z_values[dura_label] + cluster_z_values[cortical_label])/2;

        std::vector<int> labels(points.size(), -1);  // Initialize all points as unclassified
        for(int i=0;i<cluster_indices.size();i++){
            if((cluster_z_values[i]>=z_threshold)&&(cluster_sizes[i]>500)){
                for( const auto& idx : cluster_indices[i].indices){
                    labels[idx] = 1;
                }
            }
            else if((cluster_z_values[i]<z_threshold)&&(cluster_sizes[i]>500)){
                for( const auto& idx : cluster_indices[i].indices){
                    labels[idx] = 2;
                }
            }
        }

        for (size_t i = 0; i < points.size(); ++i) {
            pointcloud::NLClusterResponse response;
            response.set_point_id(points[i].id());

            if (labels[i] == -1) {
                response.set_label(pointcloud::NLClusterLabel::UNKNOWN);  // Noise or unclassified
            } else if (labels[i] == 2) {
                response.set_label(pointcloud::NLClusterLabel::DURA);
            } else if (labels[i] == 1) {
                response.set_label(pointcloud::NLClusterLabel::CORTICAL_SURFACE);
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
    // PointCloudServiceImpl service;

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