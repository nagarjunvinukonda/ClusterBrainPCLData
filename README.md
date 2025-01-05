# Neuralink Programming Challenge

The aim of this project is develop a gRPC pipleine that reads the Brain data and clusters it to provide it's expected result. The dataset consists of point cloud distinguished into three distinct groups of points; the groups are:<br />
1. The top layer of the scan ("Dura")<br />
2. The bottom layer of the scan ("Cortical Surface")<br />
3. Noise and other artifacts between the above two layers ("Unknown")<br />

The project requires me to build a gRPC server-client. The client reads the data and sends a request to gRPC server. The server receives that data and clusters the point cloud to classify it into three distict groups of the brain. Then responds back to client with the classified labels for each point among the clusters. Further this data is written into a file called output.labels.<br />

I was able to work for 4-5 days during weekends only (due to heavy work on weekdays). Currently I am using DBSCAN like approach for clustering, which is not providing expected result *except* for 'oct-3-small.pcd' dataset. If given more time I can debug my algorithm to provide it's best result. As it was mentioned *not to use any external libraries for the clustering itself* my DBSCAN implementation has limitations. <br />


## Developer: 

**Nagarjun Vinukonda**

## Pre-requisites:

**Note:** This package is for Ubuntu 22.0.4.<br />

To run the package your system needs to install [Docker](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-22-04)

## Package structure:

```
neuralink_nagarjun_project/
│
├── Dockerfile                      # Dockerfile for the server
|
├── proto/
│   └── pointcloud.proto            # gRPC protobuf file defining the data structures 
|
├── client_setup.sh                 # Script for setting up and building the client
|
├── server/
│   ├── grpc_pointcloud_server.cpp  # gRPC server implementation
│   ├── CMakeLists.txt              # CMake configuration for the server build
│
├── client/
│   ├── grpc_client_cpp.cpp         # gRPC client implementation
│   ├── CMakeLists.txt              # CMake configuration for the client build
│
├── data/
│   ├── oct-1-small.labels          # Point cloud label files provided
│   ├── oct-1-small.pcd             # Point cloud data files provided
│   ├── oct-2-medium.labels         # Point cloud label files provided
│   ├── oct-2-medium.pcd            # Point cloud data files provided
│   ├── oct-2-small.labels          # Point cloud label files provided
│   ├── oct-2-small.pcd             # Point cloud data files provided
│   ├── oct-3-small.labels          # Point cloud label files provided
│   └── oct-3-small.pcd             # Point cloud data files provided
|
├── proto_generated/
│   ├── pointcloud.grpc.pb.cc       # generated pointcloud headers
│   ├── pointcloud.pb.cc            # generated pointcloud headers
│   ├── pointcloud.pb.h             # generated pointcloud headers
│   ├── pointcloud.grpc.pb.h        # generated pointcloud headers
|
├── help/
│   ├── requirements.txt            # Installation requirments to visualize 
│   ├── visualize,py                # Visualize the 3D point cloud from output.labels
|
└── README.md   
```

## Project building:

Go to your package:<br />

```
cd neuralink_nagarjun_project
```

Build the Dockerfile for the gRPC server:<br />

```
sudo docker build -t pointcloud_server -f Dockerfile .
```

Build the executable for gRPC client:<br />

```
sudo ./client_setup
```


## Running this Project:

Open terminal first run the server:<br />
```
sudo docker run -p 50051:50051 pointcloud_server 
```

Note: above I am mapping Host port to Container port.<br />


Open another terminal to run the client:<br />
```
./client --host localhost --port 50051 --input data/oct-1-small.pcd --output data/output.labels 
```

This should provide you output.labels in the data folder to visualize the 3D data classified.


## Advantages:
- Multi-threading is used for finding nearset neighbors and which speeds up the clustering process further. 
- Package works on taking huge dataset like 'oct-2-medium.pcd' with more than 200K points withut a crash. 

## Improvements & limitations with more time available:

- Build docker image using docker inside docker to make a smaller final image only holding the executable without any build dependencies<br />
- use `docker compose` to configure the port forwarding and potential other local resources<br />
- dockerize the client too<br />
- use code formatting tools<br />
- use address sanitizer when working on larger dataset for production env <br />
- follow modern C++ guidelines to at least try to avoid memory safety bugs<br />
- Use Rust:<br />
  - cargo allows easy cross compiling on the host for almost any host, static linked executable just needs to be copied<br />
  - weird tooling and guidelines are not needed like for C++, memory is a compiler guarantee<br />
  - for example grpc bindings will be generated using cargo via build.rs, no need to learn CMake or write shell scripts<br />
- The current implementation has limited error handling, particularly in the client-server communication. <br />
- The server processes the entire point cloud at once, leading to potential memory issues for large datasets.<br />
  - Stream processing to handle point cloud data in smaller chunks and use parallel processing for clustering.<br />


## Approach to clustering logic:

- Initially, I have used one of the popular methods known for clustering is K-means to cluster the data into K clusters. Once the centroids of the clusters were computed, we compute the avg Z value for each cluster, the cluster closer to Z=0 is classified as Dura and farway from Z=0 as Cortical and rest as Uknown. Limitation of this algorithm for our dataset is: K-means cant be used for dataset which has clusters not linearly seprable.<br />

- I have used Density based clustering (DBSCAN). This is best agorithm when clusters are non linearly seprable. After clustering using DBSCAN. One of Major cluster which is close to Z=0 should be DURA and other major cluster far way from Z=0 should be classified as CORTICAL and the rest noise should be classfied as UKNOWN. To explain in algorithimic way: I took the Z-threshold based on average of top and bottom most clusters centrod z-values. If a cluster is greater than Z-threshold with min of 500 points then it is classfied as Cortical. If a cluster is less than Z-threshold with min of 500 points then it is classfied as Dura. THe rest as UNKNOWN <br />

- **Note**: A documentation on my clustering approach with images are available in the file 'Clustering Algorithm my Approach.odt' inside results folder of the package. <br />

My limitation using DBSCAN:<br />
- Time complexity O(n^2). Even I have used Multi threading for finding neighbors, Without using 3rd party libraries performing DBSCAN is taking a huge time on large dataset.<br />
- I have failed to debug my DBSCAN algorithm for 'oct-3-small.pcd' dataset that yeilds correct dataset within the time I have used, but rest of dataset are working very well. Although, If I can use 3rd party library (using PCL/Kd-Tree/), the clustering is better than ground truth for 'oct-3-small.pcd' dataset. Also, the time Complexity can be brought to O(nlogn). <br />


## References:

1. [Implemeting K-means](https://reasonabledeviations.com/2019/10/02/k-means-in-cpp/)
2. [Kmeans Vs DBSCAN](https://nzlul.medium.com/clustering-method-using-k-means-hierarchical-and-dbscan-using-python-5ca5721bbfc3)
3. ChatGPT (log is provided in a seprate file called 'ChatGptlog' inside results folder): Setup installtion; Docker, gRPC, protbuff compiler, clustering algorithms.
4. [gRPC service](https://grpc.io/docs/languages/cpp/basics/)
5. I have also created an approach using kd-tree via 3rd party tools like PCL for clustering algorithm that yeilds better and faster results for all the given datasets, available in the result folder for any future references: 'grpc_pointcloud_server_pcl_used.cpp' file
