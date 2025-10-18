# Truemap

A native mobile application for iOS and Android that performs fully on-device 3D reconstruction from multiple YouTube videos.

## Overview

Truemap allows users to create detailed 3D models of real-world locations by processing multiple YouTube videos. The app downloads videos, extracts frames, and uses advanced computer vision techniques to generate a textured 3D mesh that can be viewed and exported.

### Key Features

- **Multi-Video Reconstruction**: Process multiple YouTube videos of the same location
- **Fully On-Device**: All processing happens locally on your mobile device
- **Advanced 3D Reconstruction**: 
  - Structure from Motion (SfM) for camera pose estimation
  - Feature-based alignment using RANSAC and ICP
  - Global bundle adjustment for optimization
  - Multi-View Stereo (MVS) for dense reconstruction
  - Poisson surface reconstruction for mesh generation
- **Export Options**: Export final 3D models as OBJ or PLY files
- **Native Performance**: Written with Swift (iOS) and Kotlin (Android) with C++ core

## Technology Stack

### Cross-Platform
- **Core Library**: C++17 with CMake build system
- **3D Reconstruction**: OpenMVG, OpenCV, Eigen
- **Algorithms**: SIFT/ORB features, RANSAC, ICP, Bundle Adjustment, MVS, Poisson Reconstruction

### iOS
- **Language**: Swift
- **UI**: SwiftUI
- **Video Processing**: AVFoundation
- **3D Rendering**: SceneKit/RealityKit
- **Minimum Version**: iOS 15.0+

### Android
- **Language**: Kotlin
- **UI**: Jetpack Compose
- **Video Processing**: MediaCodec
- **3D Rendering**: Filament/OpenGL ES
- **Minimum SDK**: API 26 (Android 8.0+)

## Getting Started

### Prerequisites

- **For iOS Development**:
  - macOS (Monterey or later)
  - Xcode 14.0+
  - CocoaPods
  - CMake 3.20+

- **For Android Development**:
  - Android Studio (Electric Eel or later)
  - Android SDK (API 26+)
  - Android NDK (r25+)
  - CMake 3.20+

### Installation

See [SETUP.md](docs/SETUP.md) for detailed setup instructions.

Quick start:

```bash
# Clone repository
git clone https://github.com/Kaleaon/Truemap.git
cd Truemap

# Initialize submodules
git submodule update --init --recursive

# For iOS
cd ios && pod install
open Truemap.xcworkspace

# For Android
# Open 'android' folder in Android Studio
```

## Architecture

The application follows a layered architecture:

1. **UI Layer**: Native iOS (SwiftUI) and Android (Compose) interfaces
2. **Service Layer**: Platform-specific services for video and frame processing
3. **Core Library**: Shared C++ implementation of 3D reconstruction pipeline

See [ARCHITECTURE.md](docs/ARCHITECTURE.md) for detailed architecture documentation.

## Usage

1. **Add Videos**: Input YouTube URLs of the location you want to reconstruct
2. **Configure**: Optionally adjust reconstruction parameters (frame rate, features, etc.)
3. **Start Reconstruction**: Begin the automatic processing pipeline
4. **Monitor Progress**: Watch real-time progress updates
5. **Export**: Save the final 3D mesh as OBJ or PLY file
6. **View**: Render and interact with the 3D model

## Reconstruction Pipeline

1. **Video Download**: Download YouTube videos locally
2. **Frame Extraction**: Extract frames at specified rate (1-2 fps)
3. **Feature Detection**: Extract SIFT or ORB features from each frame
4. **Feature Matching**: Match features across frames
5. **Structure from Motion**: Compute camera poses and sparse 3D points
6. **Model Alignment**: Align multiple sub-models using RANSAC + ICP
7. **Bundle Adjustment**: Global optimization of all cameras and points
8. **Dense Reconstruction**: Multi-View Stereo for dense point cloud
9. **Mesh Generation**: Poisson surface reconstruction
10. **Texturing**: Apply textures from original frames
11. **Export**: Save as OBJ or PLY format

## Project Structure

```
Truemap/
├── ios/                    # iOS application
│   └── Truemap/
│       ├── Views/          # SwiftUI views
│       ├── ViewModels/     # View models
│       └── Services/       # Platform services
├── android/                # Android application
│   └── app/
│       └── src/main/
│           ├── kotlin/     # Kotlin source
│           └── cpp/        # JNI bridge
├── shared/                 # Shared code
│   └── cpp/                # C++ core library
│       ├── include/        # Public headers
│       └── src/            # Implementation
│           ├── core/       # Core functionality
│           ├── sfm/        # Structure from Motion
│           ├── alignment/  # Model alignment
│           ├── optimization/ # Bundle adjustment
│           ├── mvs/        # Multi-View Stereo
│           └── mesh/       # Mesh generation
└── docs/                   # Documentation
```

## Performance Considerations

- **On-Device Processing**: All computation is local, no cloud dependency
- **Memory Management**: Efficient handling of large point clouds
- **Progress Tracking**: Real-time updates for long-running operations
- **Cancellation**: Ability to cancel operations at any time
- **Optimization**: SIMD and platform-specific optimizations

## Limitations

- Processing time depends on device capabilities
- Large videos may require significant storage
- Memory-intensive operations may be limited on older devices
- Requires good camera overlap between videos
- Best results with videos of static scenes

## Future Enhancements

- GPU acceleration for MVS and meshing
- Real-time preview during processing
- Cloud backup and sharing
- AR visualization of models
- Advanced mesh editing tools
- Support for more video sources

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

## Acknowledgments

- OpenMVG for Structure from Motion
- OpenCV for computer vision utilities
- Eigen for linear algebra
- CGAL for mesh processing algorithms

## Support

For issues, questions, or suggestions, please open an issue on GitHub.

## References

- [OpenMVG Documentation](https://github.com/openMVG/openMVG)
- [Multi-View Stereo](https://en.wikipedia.org/wiki/Multiview_stereo)
- [Poisson Surface Reconstruction](https://www.cs.jhu.edu/~misha/MyPapers/ToG13.pdf)
- [Bundle Adjustment](https://en.wikipedia.org/wiki/Bundle_adjustment)