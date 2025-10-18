#ifndef TRUEMAP_MVS_PROCESSOR_H
#define TRUEMAP_MVS_PROCESSOR_H

#include "types.h"
#include <string>
#include <vector>
#include <memory>

namespace truemap {

/**
 * Multi-View Stereo (MVS) processor for dense reconstruction
 */
class MVSProcessor {
public:
    MVSProcessor();
    ~MVSProcessor();
    
    /**
     * Set depth map resolution
     */
    void setDepthMapResolution(int resolution);
    
    /**
     * Generate dense point cloud from sparse reconstruction
     * @param sparse_cloud Sparse point cloud from SfM
     * @param camera_poses Camera poses
     * @param image_paths Original image paths
     * @param callback Progress callback
     * @return Dense point cloud
     */
    std::shared_ptr<PointCloud> generateDenseCloud(const PointCloud& sparse_cloud, const std::vector<CameraPose>& camera_poses, const std::vector<std::string>& image_paths, ProgressCallback callback = nullptr);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_MVS_PROCESSOR_H
