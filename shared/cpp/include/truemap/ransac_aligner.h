#ifndef TRUEMAP_RANSAC_ALIGNER_H
#define TRUEMAP_RANSAC_ALIGNER_H

#include "types.h"
#include <memory>

namespace truemap {

/**
 * RANSAC-based initial alignment for point clouds
 */
class RANSACAligner {
public:
    RANSACAligner();
    ~RANSACAligner();
    
    /**
     * Set RANSAC parameters
     */
    void setMaxIterations(int max_iter);
    void setThreshold(float threshold);
    
    /**
     * Compute initial alignment between two point clouds
     * @param source Source point cloud
     * @param target Target point cloud
     * @param output_transform Output transformation matrix (4x4, row-major)
     * @return Result indicating success or error
     */
    Result computeInitialAlignment(const PointCloud& source, const PointCloud& target, double* output_transform);
    
    /**
     * Get number of inliers from last alignment
     */
    int getInlierCount() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_RANSAC_ALIGNER_H
