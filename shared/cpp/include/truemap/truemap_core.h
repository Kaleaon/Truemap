#ifndef TRUEMAP_CORE_H
#define TRUEMAP_CORE_H

#include "types.h"
#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace truemap {

// Forward declarations
class VideoProcessor;
class SfMPipeline;
class RANSACAligner;
class ICPAligner;
class BundleAdjuster;
class MVSProcessor;
class PoissonReconstructor;
class MeshExporter;

/**
 * Main Truemap reconstruction engine
 * Orchestrates the entire 3D reconstruction pipeline
 */
class TruemapCore {
public:
    TruemapCore();
    ~TruemapCore();
    
    /**
     * Set reconstruction parameters
     */
    void setParameters(const ReconstructionParams& params);
    
    /**
     * Set progress callback for UI updates
     */
    void setProgressCallback(ProgressCallback callback);
    
    /**
     * Add a video URL for processing
     * @param url YouTube URL
     * @return Result indicating success or error
     */
    Result addVideoURL(const std::string& url);
    
    /**
     * Start the reconstruction process
     * This runs asynchronously and calls progress callback
     * @return Result indicating success or error
     */
    Result startReconstruction();
    
    /**
     * Cancel ongoing reconstruction
     */
    void cancelReconstruction();
    
    /**
     * Check if reconstruction is in progress
     */
    bool isProcessing() const;
    
    /**
     * Get current processing status
     */
    ProcessingStatus getStatus() const;
    
    /**
     * Get the final reconstructed mesh
     * Only valid after reconstruction completes successfully
     */
    std::shared_ptr<Mesh> getMesh() const;
    
    /**
     * Export mesh to file
     * @param filepath Output file path (.obj or .ply)
     * @return Result indicating success or error
     */
    Result exportMesh(const std::string& filepath);
    
    /**
     * Clear all data and reset
     */
    void reset();
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_CORE_H
