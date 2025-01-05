gRPC Point Cloud Clustering Service - Robot Software
====================================================

Problem Overview
--------------

You are given a dataset of point clouds, each representing a brain scan. Your task is to write a service for clustering each point cloud into three distinct groups of points; the groups are:
1. The top layer of the scan ("Dura")
2. The bottom layer of the scan ("Cortical Surface")
3. Noise and other artifacts between the above two layers ("Unknown")

You **should** assume that the *negative-Z* direction is always *up*.

![scan_example](images/scan.png)


> **_Additional Scientific Context_**
>
> (not expected or required reading for this problem, just if you're interested)
>
> Each point cloud in the provided dataset corresponds to a volumetric scan of brain tissue captured with ["Optical Coherence Tomography" (OCT)](https://en.wikipedia.org/wiki/Optical_coherence_tomography). Specifically, each OCT volume includes a partial 3D scan of the brain through the burr hole of a craniectomy procedure (in which a circular portion of the skull is removed).
>
> The scan includes multiple interesting features at the surface of the brain, including:
>    * a section of the [dura mater](https://en.wikipedia.org/wiki/Dura_mater), a thick, protective membrane between the skull and brain.
>    * the [cortical surface](https://en.wikipedia.org/wiki/Cerebral_cortex), or the outer layer of the brain that controls functions like perception and movement.
>    * the [arachnoid mater](https://en.wikipedia.org/wiki/Arachnoid_mater), a web-like, protective membrane between the dura and cortex.
>
> The data we have provided you comes from *actual pre-clinical surgeries!*

Task Details and Data
-------------
Your task is to create a [gRPC service](https://grpc.io/docs/languages/cpp/basics/) that implements the protobuf definition
given in the file `pointcloud.proto` and a client to test it. The `Cluster` RPC endpoint should use a clustering algorithm of your
choice fit for the given dataset. You are free to use any third party libraries **except** to perform the clustering itself,
please cite any resources you've used in formulating your solution as comments in your code.

> **_NOTE_**
>
> We don't expect you to have any prior experience or proficiency in this area; however, we do expect you to be able to write a solution to this problem by doing a bit of research if you don't.

The `Cluster` endpoint is a streaming endpoint. When your server reaches the end of the stream, it should cluster the point cloud and return the cluster assignment of each point and its associated ID as a stream. You are free to design your own scheme for assigning IDs to points in your client.


Implement the server as if it will operate in a production environment and be subsequently used and worked on by a team of
engineers. If you make compromises to keep your solution simple and timely, please be sure to bring them up in the
discussion part of the deliverables.

The point cloud data is given as a [`.PCD`](https://pointclouds.org/documentation/tutorials/pcd_file_format.html) file and the labels as a text file with each line corresponding to integer labels for each point.

Language and Build Requirements
-------------------------------
Please use C++ or Rust for the implementation of the server. You may use any language of choice for the client. If you are not comfortable with C++ or Rust, you may use another **system** language of your choice.

Your solution server **must** build and run in a Docker container based on the provided `Dockerfile`. The dockerfile uses `ubuntu:22.04` as its base image, but you may choose a different one that suits your needs, so long as we can build and run it. Please feel free to otherwise modify the docker container however you like.

Please do not include any binaries or build artifacts in your solution submissions.

Deliverables
------------
1. A completed `Dockerfile` that can build all required dependencies and run the server by exposing the port `50051`.
2. A gRPC server that implements the NLPointCloudService interface that runs via the docker container.
3. A client that provides `--host`, `--port`, `--input`, and `--output` arguments and can be run from the top level directory of your submission via the command `./client --host <...> --port <....> --input <...> --output <...>`. It is okay if `./client` is a wrapper script.
    * the `input` argument takes a `.PCD` file with the point cloud data.
    * the `output` argument takes a path to the generated labels file, a text file mapping point ID (line number) to its dura, cortical surface, or unknown labels (proto enum numeric IDs).
4. A client setup script that can be run from the top level directory of your submission via the command `./client-setup` that builds or performs any other setup before `./client` can be run.
5. A discussion of limitations or known issues with your solution, how you'd change it for a production environment given more time and resources, and any other thoughts you have about the problem, your solution, or the tools you used to implement and/or evaluate it. Please include a description of how you arrived at your clustering approach in your discussion.

> **_NOTE_**
>
> If you utilize language models like ChatGPT in your solution, please submit a chat log. The inclusion of a chat log will not detract from your score.

Help & Resources
----------------
We have provided you with a simple Python script `help/visualize.py` that you can use to visualize the 3D OCT scans. We encourage you to use this tool as you brainstorm your solution. Please do not hesitate to reach out to the team with any clarifying questions you may have about the task.
