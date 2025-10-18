#ifndef TRUEMAP_SFM_PIPELINE_H
#define TRUEMAP_SFM_PIPELINE_H

#include "types.h"
#include <string>
#include <vector>
#include <memory>

namespace truemap {

/**
 * Structure from Motion pipeline
 * Processes image frames to generate sparse 3D reconstruction
 */
class SfMPipeline {
public:
    SfMPipeline();
    ~SfMPipeline();
    
    /**
     * Set reconstruction parameters
     */
    void setParameters(const ReconstructionParams& params);
    
    /**
     * Process a set of images to create 3D reconstruction
     * @param image_paths Paths to input images
     * @param callback Progress callback
     * @return Result indicating success or error
     */
    Result process(const std::vector<std::string>& image_paths, ProgressCallback callback = nullptr);
    
    /**
     * Get the reconstructed point cloud
     */
    std::shared_ptr<PointCloud> getPointCloud() const;
    
    /**
     * Get camera poses for each image
     */
    std::vector<CameraPose> getCameraPoses() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_SFM_PIPELINE_H
