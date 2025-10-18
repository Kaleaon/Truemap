#include "truemap/mesh_exporter.h"
#include "truemap/logger.h"
#include <fstream>
#include <iomanip>

namespace truemap {

class MeshExporter::Impl {
public:
    // Implementation details
};

MeshExporter::MeshExporter() : impl_(std::make_unique<Impl>()) {}

MeshExporter::~MeshExporter() = default;

Result MeshExporter::exportOBJ(const Mesh& mesh, const std::string& filepath) {
    Logger::info("Exporting mesh to OBJ: " + filepath);
    
    if (mesh.empty()) {
        return Result::Error("Empty mesh");
    }
    
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return Result::Error("Failed to open file: " + filepath);
    }
    
    // Write header
    file << "# Truemap OBJ Export\n";
    file << "# Vertices: " << mesh.vertex_count() << "\n";
    file << "# Triangles: " << mesh.triangle_count() << "\n\n";
    
    // Write vertices
    for (const auto& v : mesh.vertices) {
        file << "v " << std::fixed << std::setprecision(6) 
             << v.x << " " << v.y << " " << v.z << "\n";
    }
    
    // Write vertex colors (as comments, OBJ doesn't have standard color support)
    file << "\n# Vertex colors (r g b)\n";
    for (const auto& v : mesh.vertices) {
        file << "# " << static_cast<int>(v.r) << " " 
             << static_cast<int>(v.g) << " " 
             << static_cast<int>(v.b) << "\n";
    }
    
    // Write texture coordinates
    if (!mesh.texcoords.empty()) {
        file << "\n";
        for (const auto& tc : mesh.texcoords) {
            file << "vt " << tc.u << " " << tc.v << "\n";
        }
    }
    
    // Write normals
    if (!mesh.normals.empty()) {
        file << "\n";
        for (const auto& n : mesh.normals) {
            file << "vn " << n.x << " " << n.y << " " << n.z << "\n";
        }
    }
    
    // Write faces (1-indexed)
    file << "\n";
    for (const auto& tri : mesh.triangles) {
        file << "f " << (tri.v0 + 1) << " " 
             << (tri.v1 + 1) << " " 
             << (tri.v2 + 1) << "\n";
    }
    
    file.close();
    
    Logger::info("Successfully exported mesh to OBJ");
    return Result::Success();
}

Result MeshExporter::exportPLY(const Mesh& mesh, const std::string& filepath, bool binary) {
    Logger::info("Exporting mesh to PLY: " + filepath);
    Logger::info("Format: " + std::string(binary ? "binary" : "ASCII"));
    
    if (mesh.empty()) {
        return Result::Error("Empty mesh");
    }
    
    std::ofstream file(filepath, binary ? std::ios::binary : std::ios::out);
    if (!file.is_open()) {
        return Result::Error("Failed to open file: " + filepath);
    }
    
    // Write PLY header
    file << "ply\n";
    file << "format " << (binary ? "binary_little_endian" : "ascii") << " 1.0\n";
    file << "comment Truemap PLY Export\n";
    file << "element vertex " << mesh.vertex_count() << "\n";
    file << "property float x\n";
    file << "property float y\n";
    file << "property float z\n";
    file << "property uchar red\n";
    file << "property uchar green\n";
    file << "property uchar blue\n";
    file << "element face " << mesh.triangle_count() << "\n";
    file << "property list uchar int vertex_indices\n";
    file << "end_header\n";
    
    if (binary) {
        // Binary format
        for (const auto& v : mesh.vertices) {
            float pos[3] = {static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z)};
            file.write(reinterpret_cast<const char*>(pos), sizeof(pos));
            file.write(reinterpret_cast<const char*>(&v.r), 1);
            file.write(reinterpret_cast<const char*>(&v.g), 1);
            file.write(reinterpret_cast<const char*>(&v.b), 1);
        }
        
        for (const auto& tri : mesh.triangles) {
            uint8_t count = 3;
            file.write(reinterpret_cast<const char*>(&count), 1);
            int32_t indices[3] = {static_cast<int32_t>(tri.v0), 
                                  static_cast<int32_t>(tri.v1), 
                                  static_cast<int32_t>(tri.v2)};
            file.write(reinterpret_cast<const char*>(indices), sizeof(indices));
        }
    } else {
        // ASCII format
        for (const auto& v : mesh.vertices) {
            file << v.x << " " << v.y << " " << v.z << " "
                 << static_cast<int>(v.r) << " " 
                 << static_cast<int>(v.g) << " " 
                 << static_cast<int>(v.b) << "\n";
        }
        
        for (const auto& tri : mesh.triangles) {
            file << "3 " << tri.v0 << " " << tri.v1 << " " << tri.v2 << "\n";
        }
    }
    
    file.close();
    
    Logger::info("Successfully exported mesh to PLY");
    return Result::Success();
}

} // namespace truemap
