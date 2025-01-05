cmake_minimum_required(VERSION 3.10)
project(grpc_pointcloud_server)

# Find Protobuf, gRPC, and PCL
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED PATHS /usr/local/lib/cmake/grpc) # Add gRPC installation path here if needed
# find_package(PCL 1.10 REQUIRED)

# Include directories
# include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS} ../proto_generated ${PCL_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS} ../proto_generated)

# Server source files
set(SERVER_SRCS
    grpc_pointcloud_server.cpp
    ../proto_generated/pointcloud.grpc.pb.cc
    ../proto_generated/pointcloud.pb.cc
)

# Add the executable
add_executable(grpc_pointcloud_server ${SERVER_SRCS})

# Link with gRPC, Protobuf, and PCL
# target_link_libraries(grpc_pointcloud_server ${Protobuf_LIBRARIES} gRPC::grpc++ gRPC::grpc ${PCL_LIBRARIES})
target_link_libraries(grpc_pointcloud_server ${Protobuf_LIBRARIES} gRPC::grpc++ gRPC::grpc)
