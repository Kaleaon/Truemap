#ifndef TRUEMAP_MESH_EXPORTER_H
#define TRUEMAP_MESH_EXPORTER_H

#include "types.h"
#include <string>

namespace truemap {

/**
 * Mesh exporter for OBJ and PLY formats
 */
class MeshExporter {
public:
    MeshExporter();
    ~MeshExporter();
    
    /**
     * Export mesh to OBJ format
     * @param mesh Mesh to export
     * @param filepath Output file path
     * @return Result indicating success or error
     */
    Result exportOBJ(const Mesh& mesh, const std::string& filepath);
    
    /**
     * Export mesh to PLY format
     * @param mesh Mesh to export
     * @param filepath Output file path
     * @param binary Use binary format (true) or ASCII (false)
     * @return Result indicating success or error
     */
    Result exportPLY(const Mesh& mesh, const std::string& filepath, bool binary = true);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_MESH_EXPORTER_H
