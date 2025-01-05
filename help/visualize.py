import argparse

import numpy as np
import open3d as o3d


def read_pcd(path: str) -> np.array:
    """
    This is an error-prone bad implementation of PCD reading which ignores all metadata. Do NOT re-use this function in
    your implementation of the client.

    :param path:
    :return:
    """
    with open(path, "r") as f:
        lines = [line.replace("\n", "") for line in f.readlines()]

    data = lines[10:]
    points = []
    for point in data:
        coords = [float(num) for num in point.split(" ")]
        points.append(coords)
    return np.array(points)


def visualize(points: np.array, labels: np.array = None) -> None:
    pcd = o3d.geometry.PointCloud()
    pcd.points = o3d.utility.Vector3dVector(points)

    assert points.shape[0] == labels.shape[0]

    if labels is not None:
        # We generate a unique color for every unique label
        colors = [
            (0.584, 0.804, 0.843)  # Light blue, unknown
            if label == 0
            else (0.106, 0.424, 0.636)  # Dark blue cortical surface
            if label == 1
            else (0.498, 0.306, 0.27)  # Brown, dura
            if label == 2
            else None
            for label in labels
        ]
        assert not any(map(lambda x: x is None, colors)), "This visualizer only supports labels 0-2"
        assert points.shape[0] == len(
            colors
        ), f"The number of points must match the number of labels. {points.shape[0]} != {len(labels)}"

        # Assign the colors to the pointcloud
        pcd.colors = o3d.utility.Vector3dVector(colors)

    # Display the point cloud
    o3d.visualization.draw_geometries(
        [
            pcd,
            o3d.geometry.TriangleMesh.create_coordinate_frame(size=100, origin=[0, 0, 0]),
        ]
    )


def parse_label(path: str) -> np.array:
    """
    Assumes that the labels have the same length and same order as the points in the PCD input file
    It also assumes that labels are real integer values.

    :param path:
    :return:
    """
    with open(path, "r") as f:
        labels = [int(label.replace("\n", "")) for label in f.readlines()]
        return np.array(labels)


def parse_arguments():
    ap = argparse.ArgumentParser(description="Visualize PCD file")
    ap.add_argument("-i", "--path-to-pcd", required=True, type=str, help="Input PCD file")
    ap.add_argument(
        "-l",
        "--labels",
        required=False,
        type=str,
        help="Cluster label file. Text file where each line contains one int label per point",
    )
    return ap.parse_args()


if __name__ == "__main__":
    args = parse_arguments()

    labels = parse_label(args.labels) if args.labels else None
    points = read_pcd(args.path_to_pcd)
    visualize(points, labels=labels)
