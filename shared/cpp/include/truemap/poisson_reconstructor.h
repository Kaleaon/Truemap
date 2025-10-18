#ifndef TRUEMAP_POISSON_RECONSTRUCTOR_H
#define TRUEMAP_POISSON_RECONSTRUCTOR_H

#include "types.h"
#include <memory>

namespace truemap {

/**
 * Poisson surface reconstruction for mesh generation
 */
class PoissonReconstructor {
public:
    PoissonReconstructor();
    ~PoissonReconstructor();
    
    /**
     * Set Poisson reconstruction depth
     */
    void setDepth(int depth);
    
    /**
     * Generate mesh from dense point cloud
     * @param point_cloud Dense point cloud
     * @param callback Progress callback
     * @return Generated mesh
     */
    std::shared_ptr<Mesh> reconstruct(const PointCloud& point_cloud, ProgressCallback callback = nullptr);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_POISSON_RECONSTRUCTOR_H
