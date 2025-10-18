#ifndef TRUEMAP_ICP_ALIGNER_H
#define TRUEMAP_ICP_ALIGNER_H

#include "types.h"
#include <memory>

namespace truemap {

/**
 * Iterative Closest Point (ICP) algorithm for fine alignment
 */
class ICPAligner {
public:
    ICPAligner();
    ~ICPAligner();
    
    /**
     * Set ICP parameters
     */
    void setMaxIterations(int max_iter);
    void setTolerance(float tolerance);
    
    /**
     * Align two point clouds using ICP
     * @param source Source point cloud to be transformed
     * @param target Target point cloud (reference)
     * @param initial_transform Initial transformation guess (4x4 matrix, row-major)
     * @param output_transform Computed transformation matrix (4x4, row-major)
     * @return Result indicating success or error
     */
    Result align(const PointCloud& source, const PointCloud& target, const double* initial_transform, double* output_transform);
    
    /**
     * Get final alignment error
     */
    double getFinalError() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_ICP_ALIGNER_H
