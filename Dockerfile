# Use an official Ubuntu base image
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    autoconf \
    automake \
    cmake \
    curl \
    g++ \
    git \
    libtool \
    make \
    pkg-config \
    unzip \
    ca-certificates \
    libssl-dev \
    protobuf-compiler \
    libprotobuf-dev \
    && apt-get clean

# Install gRPC
RUN git clone -b v1.42.0 https://github.com/grpc/grpc /grpc \
    && cd /grpc \
    && git submodule update --init \
    && mkdir -p cmake/build \
    && cd cmake/build \
    && cmake ../.. \
    && make \
    && make install


# Set the working directory
WORKDIR /app

# Copy the source code and proto files
COPY server/CMakeLists.txt /app/server/
COPY server/grpc_pointcloud_server.cpp /app/server/
COPY proto /app/proto


RUN mkdir -p /app/proto_generated

# Generate gRPC and Protobuf files

# RUN cd /app

RUN protoc -I=proto --grpc_out=/app/proto_generated --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` proto/pointcloud.proto \
    && protoc -I=proto --cpp_out=/app/proto_generated proto/pointcloud.proto

# RUN find /app -name pointcloud.grpc.pb.h | xargs echo



RUN mkdir -p /app/server/build \
    && cd /app/server/build \
    && cmake .. \
    && make



# # Build the gRPC server
# RUN g++ -std=c++17 -o /app/server_executable /app/server/grpc_pointcloud_server.cpp /app/server/pointcloud.grpc.pb.cc /app/server/pointcloud.pb.cc \
#     -lgrpc++ -lprotobuf -lgrpc -lgpr -lz -lssl -lcrypto -labsl_base -labsl_status -labsl_strings -labsl_synchronization -lpthread

# Expose the port for gRPC
EXPOSE 50051


# CMD [sleep 10000]

# Run the server
CMD ["/app/server/build/grpc_pointcloud_server"]
