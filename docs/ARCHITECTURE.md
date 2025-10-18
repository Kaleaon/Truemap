# Truemap Architecture

## Overview

Truemap is a native mobile application for iOS and Android that performs fully on-device 3D reconstruction from multiple YouTube videos. The application processes videos of a real-world location to generate textured 3D triangle meshes.

## System Architecture

### High-Level Components

```
┌─────────────────────────────────────────────────────────┐
│                    Mobile UI Layer                       │
│              (iOS: SwiftUI / Android: Jetpack)          │
└─────────────────────────────────────────────────────────┘
                          │
┌─────────────────────────────────────────────────────────┐
│                Platform-Specific Layer                   │
│  ┌─────────────────┐        ┌──────────────────┐       │
│  │  iOS Layer      │        │  Android Layer    │       │
│  │  - AVFoundation │        │  - MediaCodec     │       │
│  │  - SceneKit     │        │  - Filament       │       │
│  └─────────────────┘        └──────────────────┘       │
└─────────────────────────────────────────────────────────┘
                          │
┌─────────────────────────────────────────────────────────┐
│              Shared C++ Core Library                     │
│  ┌───────────────────────────────────────────────┐     │
│  │  Video Processing Module                      │     │
│  │  - Video download                             │     │
│  │  - Frame extraction coordination              │     │
│  └───────────────────────────────────────────────┘     │
│  ┌───────────────────────────────────────────────┐     │
│  │  Structure from Motion (SfM) Module           │     │
│  │  - Feature extraction (SIFT/ORB)              │     │
│  │  - Feature matching                           │     │
│  │  - Camera pose estimation                     │     │
│  │  - Sparse point cloud generation              │     │
│  │  - Based on OpenMVG                           │     │
│  └───────────────────────────────────────────────┘     │
│  ┌───────────────────────────────────────────────┐     │
│  │  Model Alignment Module                       │     │
│  │  - RANSAC-based initial alignment             │     │
│  │  - ICP (Iterative Closest Point)              │     │
│  │  - Coordinate system transformation           │     │
│  └───────────────────────────────────────────────┘     │
│  ┌───────────────────────────────────────────────┐     │
│  │  Global Optimization Module                   │     │
│  │  - Point cloud merging                        │     │
│  │  - Camera data unification                    │     │
│  │  - Global bundle adjustment                   │     │
│  └───────────────────────────────────────────────┘     │
│  ┌───────────────────────────────────────────────┐     │
│  │  Dense Reconstruction Module                  │     │
│  │  - Multi-View Stereo (MVS)                    │     │
│  │  - Dense point cloud generation               │     │
│  └───────────────────────────────────────────────┘     │
│  ┌───────────────────────────────────────────────┐     │
│  │  Mesh Generation Module                       │     │
│  │  - Poisson surface reconstruction             │     │
│  │  - Texture mapping                            │     │
│  │  - Export to OBJ/PLY format                   │     │
│  └───────────────────────────────────────────────┘     │
└─────────────────────────────────────────────────────────┘
```

## Processing Pipeline

### 1. Video Input & Preprocessing
- User inputs multiple YouTube URLs
- Videos are downloaded locally
- Frames are extracted at user-defined rate (1-2 fps)

### 2. Per-Video 3D Reconstruction
For each video:
1. **Feature Extraction**: SIFT or ORB features extracted from frames
2. **Feature Matching**: Corresponding features matched across frames
3. **SfM Processing**: 
   - Camera poses computed
   - Sparse 3D point cloud generated
4. **Sub-Model Creation**: Independent 3D model created for each video

### 3. Multi-Model Alignment
1. **Initial Alignment (RANSAC)**:
   - Robust initial registration using 3D feature descriptors
   - Outlier rejection
   - Coarse alignment estimation

2. **Fine Alignment (ICP)**:
   - Iterative refinement of alignment
   - Minimizes point-to-point distances
   - Converges to optimal transformation

### 4. Global Optimization
1. **Data Merging**:
   - All point clouds combined into unified dataset
   - Camera poses transformed to common coordinate system

2. **Bundle Adjustment**:
   - Global optimization across all cameras and points
   - Minimizes reprojection errors
   - Produces cohesive, optimized dataset

### 5. Dense Reconstruction & Meshing
1. **Dense Point Cloud Generation (MVS)**:
   - Multi-view stereo matching
   - Depth map computation
   - Dense point cloud creation

2. **Surface Reconstruction**:
   - Poisson surface reconstruction
   - Triangle mesh generation
   - Mesh smoothing and optimization

3. **Texturing**:
   - Texture coordinates computation
   - Color/texture mapping from original frames
   - Final textured mesh export (OBJ/PLY)

## Key Technologies

### iOS
- **Language**: Swift
- **UI Framework**: SwiftUI
- **Frame Extraction**: AVFoundation
- **3D Rendering**: SceneKit / RealityKit
- **Native Bindings**: C++ interop via Objective-C++

### Android
- **Language**: Kotlin
- **UI Framework**: Jetpack Compose
- **Frame Extraction**: MediaCodec
- **3D Rendering**: Filament / OpenGL ES
- **Native Bindings**: JNI to C++

### Shared Core (C++)
- **SfM Library**: OpenMVG
- **Feature Detection**: SIFT, ORB (OpenCV)
- **Alignment**: Custom ICP implementation with RANSAC
- **Bundle Adjustment**: Ceres Solver
- **MVS**: OpenMVS or custom implementation
- **Mesh Reconstruction**: CGAL Poisson reconstruction

## Build System

### iOS
- Xcode project with CocoaPods for dependencies
- CMake for C++ shared library compilation

### Android
- Gradle build system
- NDK for C++ compilation
- CMake integration for native code

## Data Flow

```
YouTube URLs → Download → Video Files
                              ↓
                     Frame Extraction
                              ↓
                    Image Frame Sets
                              ↓
                     SfM Processing
                              ↓
                 Sub-Models (Per Video)
         [Point Cloud + Camera Poses]
                              ↓
                  Model Alignment
              [RANSAC + ICP Alignment]
                              ↓
                 Merged Point Cloud
                              ↓
              Global Bundle Adjustment
                              ↓
                   Optimized Dataset
                              ↓
           Dense Reconstruction (MVS)
                              ↓
                  Dense Point Cloud
                              ↓
         Surface Reconstruction (Poisson)
                              ↓
                    Triangle Mesh
                              ↓
                      Texturing
                              ↓
              Final 3D Model (OBJ/PLY)
```

## Performance Considerations

1. **On-Device Processing**: All computation happens locally on the mobile device
2. **Progressive Processing**: UI remains responsive through background processing
3. **Memory Management**: Careful management of large point clouds and meshes
4. **Optimization**: Use of SIMD operations and GPU acceleration where possible
5. **Progress Tracking**: Real-time progress updates for long-running operations

## File Formats

### Input
- YouTube video URLs
- Downloaded video files (MP4, etc.)

### Intermediate
- Extracted frames (JPEG/PNG)
- Sparse point clouds (custom binary format)
- Camera pose files (JSON)

### Output
- Final mesh: OBJ or PLY format
- Texture files: PNG/JPEG
- Metadata: JSON with reconstruction info

## Future Enhancements

1. GPU acceleration for MVS
2. Real-time preview during processing
3. Cloud backup of reconstructed models
4. AR visualization of reconstructed models
5. Social sharing features
