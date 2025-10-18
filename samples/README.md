# Sample PLY Files for Testing

This directory contains sample .ply files for testing the Gaussian Splatting viewer.

## Available Samples

Due to the repository size constraints, we don't include large sample files. However, you can create your own test files or download Gaussian Splatting datasets from:

- [NeRF Synthetic Dataset](https://github.com/bmild/nerf)
- [Mip-NeRF 360 Dataset](https://jonbarron.info/mipnerf360/)
- [Gaussian Splatting Official Samples](https://repo-sam.inria.fr/fungraph/3d-gaussian-splatting/)

## Creating a Test File

You can create a simple test PLY file with the following content:

```ply
ply
format ascii 1.0
comment Simple colored cube
element vertex 8
property float x
property float y
property float z
property uchar red
property uchar green
property uchar blue
end_header
-1.0 -1.0 -1.0 255 0 0
1.0 -1.0 -1.0 0 255 0
1.0 1.0 -1.0 0 0 255
-1.0 1.0 -1.0 255 255 0
-1.0 -1.0 1.0 255 0 255
1.0 -1.0 1.0 0 255 255
1.0 1.0 1.0 255 255 255
-1.0 1.0 1.0 128 128 128
```

Save this as `cube.ply` and load it in the viewer.

## Converting Other Formats

If you have point clouds in other formats, you can convert them to PLY using:

- **Python**: Use `open3d` or `trimesh` libraries
- **CloudCompare**: Open source point cloud processing software
- **MeshLab**: 3D mesh processing software

## Binary PLY Files

For better performance with large datasets, use binary PLY format. The viewer supports both:
- `format binary_little_endian 1.0`
- `format ascii 1.0`
