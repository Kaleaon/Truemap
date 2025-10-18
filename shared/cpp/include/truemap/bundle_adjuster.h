#ifndef TRUEMAP_BUNDLE_ADJUSTER_H
#define TRUEMAP_BUNDLE_ADJUSTER_H

#include "types.h"
#include <vector>
#include <memory>

namespace truemap {

/**
 * Global bundle adjustment for optimizing camera poses and 3D points
 */
class BundleAdjuster {
public:
    BundleAdjuster();
    ~BundleAdjuster();
    
    /**
     * Set maximum iterations
     */
    void setMaxIterations(int max_iter);
    
    /**
     * Perform global bundle adjustment
     * @param point_cloud Point cloud to optimize
     * @param camera_poses Camera poses to optimize
     * @param callback Progress callback
     * @return Result indicating success or error
     */
    Result optimize(PointCloud& point_cloud, std::vector<CameraPose>& camera_poses, ProgressCallback callback = nullptr);
    
    /**
     * Get final reprojection error
     */
    double getFinalError() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_BUNDLE_ADJUSTER_H
