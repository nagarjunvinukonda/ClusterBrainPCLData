cmake_minimum_required(VERSION 3.10)
project(grpc_pointcloud_client)

# Find Protobuf and gRPC
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED PATHS /usr/local/lib/cmake/grpc)

# Include directories
# include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS} ../proto_generated ${PCL_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS} ../proto_generated)

# Client source files
set(CLIENT_SRCS
    grpc_client_cpp.cpp
    ../proto_generated/pointcloud.grpc.pb.cc
    ../proto_generated/pointcloud.pb.cc
)

# Add the executable
add_executable(grpc_pointcloud_client ${CLIENT_SRCS})

# Link with gRPC and Protobuf
target_link_libraries(grpc_pointcloud_client ${Protobuf_LIBRARIES} gRPC::grpc++ gRPC::grpc)
