#ifndef TRUEMAP_TYPES_H
#define TRUEMAP_TYPES_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

namespace truemap {

// Forward declarations
struct Point3D;
struct CameraPose;
struct Feature;
struct Match;
struct PointCloud;
struct Mesh;

// 3D Point representation
struct Point3D {
    double x, y, z;
    uint8_t r, g, b;  // Color
    
    Point3D() : x(0), y(0), z(0), r(0), g(0), b(0) {}
    Point3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_), r(0), g(0), b(0) {}
    Point3D(double x_, double y_, double z_, uint8_t r_, uint8_t g_, uint8_t b_)
        : x(x_), y(y_), z(z_), r(r_), g(g_), b(b_) {}
};

// 2D Point representation
struct Point2D {
    double x, y;
    
    Point2D() : x(0), y(0) {}
    Point2D(double x_, double y_) : x(x_), y(y_) {}
};

// Image feature
struct Feature {
    Point2D position;
    std::vector<float> descriptor;
    float scale;
    float orientation;
    
    Feature() : scale(1.0f), orientation(0.0f) {}
};

// Feature match between two images
struct Match {
    size_t feature_idx1;
    size_t feature_idx2;
    float distance;
    
    Match() : feature_idx1(0), feature_idx2(0), distance(0.0f) {}
    Match(size_t idx1, size_t idx2, float dist)
        : feature_idx1(idx1), feature_idx2(idx2), distance(dist) {}
};

// Camera pose (position and orientation)
struct CameraPose {
    double position[3];      // Camera position in world coordinates
    double rotation[9];      // 3x3 rotation matrix (row-major)
    double intrinsics[9];    // 3x3 intrinsic matrix (row-major)
    
    CameraPose() {
        // Initialize to identity
        for (int i = 0; i < 3; ++i) {
            position[i] = 0.0;
            for (int j = 0; j < 3; ++j) {
                rotation[i * 3 + j] = (i == j) ? 1.0 : 0.0;
                intrinsics[i * 3 + j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }
};

// Point cloud
struct PointCloud {
    std::vector<Point3D> points;
    
    PointCloud() = default;
    
    void clear() {
        points.clear();
    }
    
    size_t size() const {
        return points.size();
    }
    
    bool empty() const {
        return points.empty();
    }
};

// Triangle mesh
struct Triangle {
    size_t v0, v1, v2;  // Vertex indices
    
    Triangle() : v0(0), v1(0), v2(0) {}
    Triangle(size_t a, size_t b, size_t c) : v0(a), v1(b), v2(c) {}
};

// Texture coordinate
struct TexCoord {
    float u, v;
    
    TexCoord() : u(0.0f), v(0.0f) {}
    TexCoord(float u_, float v_) : u(u_), v(v_) {}
};

// 3D Mesh
struct Mesh {
    std::vector<Point3D> vertices;
    std::vector<Point3D> normals;
    std::vector<TexCoord> texcoords;
    std::vector<Triangle> triangles;
    std::string texture_path;
    
    Mesh() = default;
    
    void clear() {
        vertices.clear();
        normals.clear();
        texcoords.clear();
        triangles.clear();
        texture_path.clear();
    }
    
    size_t vertex_count() const {
        return vertices.size();
    }
    
    size_t triangle_count() const {
        return triangles.size();
    }
    
    bool empty() const {
        return vertices.empty() || triangles.empty();
    }
};

// Reconstruction parameters
struct ReconstructionParams {
    // Frame extraction
    float frame_rate = 1.0f;  // Frames per second to extract
    
    // Feature extraction
    int max_features = 2000;
    bool use_sift = true;  // true = SIFT, false = ORB
    
    // Matching
    float match_ratio = 0.8f;
    int min_matches = 50;
    
    // RANSAC
    int ransac_iterations = 1000;
    float ransac_threshold = 4.0;
    
    // ICP
    int icp_iterations = 50;
    float icp_tolerance = 1e-6;
    
    // Bundle adjustment
    int ba_iterations = 50;
    
    // MVS
    int depth_map_resolution = 512;
    
    // Mesh
    int poisson_depth = 8;
    float mesh_simplification = 0.9f;  // Keep 90% of triangles
};

// Processing status
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

// Progress callback type
using ProgressCallback = std::function<void(ProcessingStatus status, float progress, const std::string& message)>;

// Error result
struct Result {
    bool success;
    std::string error_message;
    
    Result() : success(true) {}
    Result(bool s, const std::string& msg = "") : success(s), error_message(msg) {}
    
    static Result Success() {
        return Result(true);
    }
    
    static Result Error(const std::string& msg) {
        return Result(false, msg);
    }
};

} // namespace truemap

#endif // TRUEMAP_TYPES_H
