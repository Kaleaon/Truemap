# Truemap API Reference

## C++ Core API

### TruemapCore

Main reconstruction engine class.

```cpp
namespace truemap {
  class TruemapCore {
  public:
    TruemapCore();
    ~TruemapCore();
    
    // Configuration
    void setParameters(const ReconstructionParams& params);
    void setProgressCallback(ProgressCallback callback);
    
    // Video management
    Result addVideoURL(const std::string& url);
    
    // Reconstruction
    Result startReconstruction();
    void cancelReconstruction();
    bool isProcessing() const;
    ProcessingStatus getStatus() const;
    
    // Output
    std::shared_ptr<Mesh> getMesh() const;
    Result exportMesh(const std::string& filepath);
    
    // Cleanup
    void reset();
  };
}
```

### Types

#### ReconstructionParams

Configuration parameters for reconstruction.

```cpp
struct ReconstructionParams {
    float frame_rate;          // Frames per second to extract (default: 1.0)
    int max_features;          // Maximum features per image (default: 2000)
    bool use_sift;             // true = SIFT, false = ORB (default: true)
    float match_ratio;         // Feature match ratio test (default: 0.8)
    int min_matches;           // Minimum matches required (default: 50)
    int ransac_iterations;     // RANSAC iterations (default: 1000)
    float ransac_threshold;    // RANSAC inlier threshold (default: 4.0)
    int icp_iterations;        // ICP max iterations (default: 50)
    float icp_tolerance;       // ICP convergence tolerance (default: 1e-6)
    int ba_iterations;         // Bundle adjustment iterations (default: 50)
    int depth_map_resolution;  // MVS depth map size (default: 512)
    int poisson_depth;         // Poisson octree depth (default: 8)
    float mesh_simplification; // Keep ratio for simplification (default: 0.9)
};
```

#### ProcessingStatus

Current processing state.

```cpp
enum class ProcessingStatus {
    IDLE,
    DOWNLOADING_VIDEO,
    EXTRACTING_FRAMES,
    EXTRACTING_FEATURES,
    MATCHING_FEATURES,
    COMPUTING_SFM,
    ALIGNING_MODELS,
    BUNDLE_ADJUSTMENT,
    DENSE_RECONSTRUCTION,
    GENERATING_MESH,
    EXPORTING,
    COMPLETED,
    ERROR
};
```

#### ProgressCallback

Callback for progress updates.

```cpp
using ProgressCallback = std::function<void(
    ProcessingStatus status,
    float progress,        // 0.0 to 1.0
    const std::string& message
)>;
```

#### Point3D

3D point with color.

```cpp
struct Point3D {
    double x, y, z;
    uint8_t r, g, b;
};
```

#### PointCloud

Collection of 3D points.

```cpp
struct PointCloud {
    std::vector<Point3D> points;
    void clear();
    size_t size() const;
    bool empty() const;
};
```

#### Mesh

Triangle mesh with optional texture.

```cpp
struct Mesh {
    std::vector<Point3D> vertices;
    std::vector<Point3D> normals;
    std::vector<TexCoord> texcoords;
    std::vector<Triangle> triangles;
    std::string texture_path;
    
    void clear();
    size_t vertex_count() const;
    size_t triangle_count() const;
    bool empty() const;
};
```

#### Result

Operation result with error handling.

```cpp
struct Result {
    bool success;
    std::string error_message;
    
    static Result Success();
    static Result Error(const std::string& msg);
};
```

## iOS Swift API

### ReconstructionViewModel

Main view model for iOS app.

```swift
class ReconstructionViewModel: ObservableObject {
    @Published var videoURLs: [String]
    @Published var isProcessing: Bool
    @Published var progress: Double
    @Published var statusMessage: String
    @Published var hasCompletedReconstruction: Bool
    @Published var errorMessage: String?
    
    func addVideoURL(_ url: String)
    func deleteURL(at offsets: IndexSet)
    func startReconstruction()
    func cancelReconstruction()
    func exportMesh()
}
```

### ReconstructionService

Service layer for C++ integration.

```swift
class ReconstructionService {
    var progressPublisher: AnyPublisher<ProgressUpdate, Never>
    var statusPublisher: AnyPublisher<ProcessingStatus, Never>
    
    func startReconstruction(urls: [String], 
                           completion: @escaping (Result<Void, Error>) -> Void)
    func cancelReconstruction()
    func exportMesh(completion: @escaping (Result<URL, Error>) -> Void)
}
```

## Android Kotlin API

### ReconstructionViewModel

Main view model for Android app.

```kotlin
class ReconstructionViewModel : ViewModel() {
    var videoUrls: List<String> by mutableStateOf(emptyList())
    var isProcessing: Boolean by mutableStateOf(false)
    var progress: Double by mutableStateOf(0.0)
    var statusMessage: String by mutableStateOf("")
    var hasCompletedReconstruction: Boolean by mutableStateOf(false)
    var errorMessage: String? by mutableStateOf(null)
    
    fun addVideoUrl(url: String)
    fun removeUrl(url: String)
    fun startReconstruction()
    fun cancelReconstruction()
    fun exportMesh()
    fun clearError()
}
```

### ReconstructionService

Service layer with JNI bridge.

```kotlin
class ReconstructionService {
    fun setProgressCallback(callback: (ProcessingStatus, Double, String) -> Unit)
    suspend fun startReconstruction(urls: List<String>)
    fun cancelReconstruction()
    suspend fun exportMesh(): String
}
```

### ProcessingStatus

```kotlin
enum class ProcessingStatus {
    IDLE,
    DOWNLOADING_VIDEO,
    EXTRACTING_FRAMES,
    EXTRACTING_FEATURES,
    MATCHING_FEATURES,
    COMPUTING_SFM,
    ALIGNING_MODELS,
    BUNDLE_ADJUSTMENT,
    DENSE_RECONSTRUCTION,
    GENERATING_MESH,
    EXPORTING,
    COMPLETED,
    ERROR
}
```

## Usage Examples

### C++

```cpp
#include "truemap/truemap_core.h"

using namespace truemap;

// Create core instance
TruemapCore core;

// Configure parameters
ReconstructionParams params;
params.frame_rate = 1.0f;
params.max_features = 2000;
params.use_sift = true;
core.setParameters(params);

// Set progress callback
core.setProgressCallback([](ProcessingStatus status, float progress, const std::string& msg) {
    std::cout << "Status: " << static_cast<int>(status) 
              << " Progress: " << progress 
              << " Message: " << msg << std::endl;
});

// Add videos
core.addVideoURL("https://youtube.com/watch?v=...");
core.addVideoURL("https://youtube.com/watch?v=...");

// Start reconstruction
auto result = core.startReconstruction();
if (!result.success) {
    std::cerr << "Error: " << result.error_message << std::endl;
}

// Wait for completion
while (core.isProcessing()) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

// Export mesh
if (core.getStatus() == ProcessingStatus::COMPLETED) {
    core.exportMesh("/path/to/output.obj");
}
```

### iOS (Swift)

```swift
import SwiftUI

struct ContentView: View {
    @StateObject private var viewModel = ReconstructionViewModel()
    
    var body: some View {
        VStack {
            // Add video URLs
            Button("Add Video") {
                viewModel.addVideoURL("https://youtube.com/watch?v=...")
            }
            
            // Start reconstruction
            Button("Start") {
                viewModel.startReconstruction()
            }
            .disabled(viewModel.videoURLs.isEmpty)
            
            // Progress
            if viewModel.isProcessing {
                ProgressView(value: viewModel.progress)
                Text(viewModel.statusMessage)
            }
            
            // Export
            if viewModel.hasCompletedReconstruction {
                Button("Export") {
                    viewModel.exportMesh()
                }
            }
        }
    }
}
```

### Android (Kotlin)

```kotlin
@Composable
fun ReconstructionScreen(viewModel: ReconstructionViewModel = viewModel()) {
    Column {
        // Add video URLs
        Button(onClick = { viewModel.addVideoUrl("https://youtube.com/watch?v=...") }) {
            Text("Add Video")
        }
        
        // Start reconstruction
        Button(
            onClick = { viewModel.startReconstruction() },
            enabled = viewModel.videoUrls.isNotEmpty()
        ) {
            Text("Start Reconstruction")
        }
        
        // Progress
        if (viewModel.isProcessing) {
            LinearProgressIndicator(progress = viewModel.progress.toFloat())
            Text(viewModel.statusMessage)
        }
        
        // Export
        if (viewModel.hasCompletedReconstruction) {
            Button(onClick = { viewModel.exportMesh() }) {
                Text("Export Mesh")
            }
        }
    }
}
```

## Error Handling

All operations return a `Result` type that contains success status and error message:

```cpp
auto result = core.addVideoURL("invalid-url");
if (!result.success) {
    // Handle error
    std::cerr << "Error: " << result.error_message << std::endl;
}
```

## Thread Safety

- The C++ core is thread-safe for concurrent calls
- Progress callbacks are called from worker threads
- iOS: Use `@MainActor` or `DispatchQueue.main` for UI updates
- Android: Use `withContext(Dispatchers.Main)` for UI updates

## Memory Management

- C++: Use smart pointers (`std::shared_ptr`, `std::unique_ptr`)
- iOS: ARC handles memory automatically
- Android: JVM garbage collector handles memory
- Large meshes may require manual cleanup with `reset()`
