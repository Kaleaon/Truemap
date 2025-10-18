#include "truemap/truemap_core.h"
#include "truemap/video_processor.h"
#include "truemap/sfm_pipeline.h"
#include "truemap/ransac_aligner.h"
#include "truemap/icp_aligner.h"
#include "truemap/bundle_adjuster.h"
#include "truemap/mvs_processor.h"
#include "truemap/poisson_reconstructor.h"
#include "truemap/mesh_exporter.h"
#include "truemap/logger.h"
#include <thread>
#include <atomic>
#include <mutex>

namespace truemap {

class TruemapCore::Impl {
public:
    Impl() : status_(ProcessingStatus::IDLE), is_processing_(false) {}
    
    ReconstructionParams params_;
    ProgressCallback callback_;
    std::vector<std::string> video_urls_;
    std::shared_ptr<Mesh> final_mesh_;
    
    std::atomic<ProcessingStatus> status_;
    std::atomic<bool> is_processing_;
    std::atomic<bool> should_cancel_;
    
    std::mutex mutex_;
    
    void updateStatus(ProcessingStatus status, float progress, const std::string& message) {
        status_ = status;
        if (callback_) {
            callback_(status, progress, message);
        }
    }
    
    Result runReconstruction() {
        is_processing_ = true;
        should_cancel_ = false;
        
        try {
            // Step 1: Download and process videos
            std::vector<std::vector<std::string>> all_frames;
            for (size_t i = 0; i < video_urls_.size(); ++i) {
                if (should_cancel_) return Result::Error("Cancelled");
                
                updateStatus(ProcessingStatus::DOWNLOADING_VIDEO, 
                           static_cast<float>(i) / video_urls_.size(),
                           "Downloading video " + std::to_string(i + 1));
                
                VideoProcessor video_proc;
                std::string video_path = "/tmp/truemap/video_" + std::to_string(i) + ".mp4";
                auto result = video_proc.downloadVideo(video_urls_[i], video_path);
                if (!result.success) return result;
                
                updateStatus(ProcessingStatus::EXTRACTING_FRAMES,
                           static_cast<float>(i) / video_urls_.size(),
                           "Extracting frames from video " + std::to_string(i + 1));
                
                std::string frames_dir = "/tmp/truemap/frames_" + std::to_string(i);
                auto frames = video_proc.extractFrames(video_path, frames_dir, params_.frame_rate);
                all_frames.push_back(frames);
            }
            
            // Step 2: Run SfM on each video
            std::vector<std::shared_ptr<PointCloud>> sub_clouds;
            std::vector<std::vector<CameraPose>> sub_poses;
            
            for (size_t i = 0; i < all_frames.size(); ++i) {
                if (should_cancel_) return Result::Error("Cancelled");
                
                updateStatus(ProcessingStatus::COMPUTING_SFM,
                           static_cast<float>(i) / all_frames.size(),
                           "Computing SfM for video " + std::to_string(i + 1));
                
                SfMPipeline sfm;
                sfm.setParameters(params_);
                auto result = sfm.process(all_frames[i]);
                if (!result.success) return result;
                
                sub_clouds.push_back(sfm.getPointCloud());
                sub_poses.push_back(sfm.getCameraPoses());
            }
            
            // Step 3: Align all sub-models
            if (sub_clouds.size() > 1) {
                updateStatus(ProcessingStatus::ALIGNING_MODELS, 0.0f, "Aligning sub-models");
                
                // Align all clouds to the first one
                std::shared_ptr<PointCloud> merged_cloud = sub_clouds[0];
                std::vector<CameraPose> merged_poses = sub_poses[0];
                
                for (size_t i = 1; i < sub_clouds.size(); ++i) {
                    if (should_cancel_) return Result::Error("Cancelled");
                    
                    // RANSAC initial alignment
                    RANSACAligner ransac;
                    ransac.setMaxIterations(params_.ransac_iterations);
                    ransac.setThreshold(params_.ransac_threshold);
                    
                    double transform[16];
                    auto result = ransac.computeInitialAlignment(*sub_clouds[i], *merged_cloud, transform);
                    if (!result.success) return result;
                    
                    // ICP refinement
                    ICPAligner icp;
                    icp.setMaxIterations(params_.icp_iterations);
                    icp.setTolerance(params_.icp_tolerance);
                    
                    double refined_transform[16];
                    result = icp.align(*sub_clouds[i], *merged_cloud, transform, refined_transform);
                    if (!result.success) return result;
                    
                    // Transform and merge (simplified - actual implementation would transform points)
                    for (const auto& pt : sub_clouds[i]->points) {
                        merged_cloud->points.push_back(pt);
                    }
                    
                    // Merge camera poses
                    for (const auto& pose : sub_poses[i]) {
                        merged_poses.push_back(pose);
                    }
                }
                
                // Step 4: Global bundle adjustment
                updateStatus(ProcessingStatus::BUNDLE_ADJUSTMENT, 0.0f, "Running global optimization");
                
                BundleAdjuster ba;
                ba.setMaxIterations(params_.ba_iterations);
                auto result = ba.optimize(*merged_cloud, merged_poses);
                if (!result.success) return result;
                
                // Step 5: Dense reconstruction
                updateStatus(ProcessingStatus::DENSE_RECONSTRUCTION, 0.0f, "Generating dense point cloud");
                
                // Collect all frames
                std::vector<std::string> all_image_paths;
                for (const auto& frames : all_frames) {
                    all_image_paths.insert(all_image_paths.end(), frames.begin(), frames.end());
                }
                
                MVSProcessor mvs;
                mvs.setDepthMapResolution(params_.depth_map_resolution);
                auto dense_cloud = mvs.generateDenseCloud(*merged_cloud, merged_poses, all_image_paths);
                
                // Step 6: Mesh generation
                updateStatus(ProcessingStatus::GENERATING_MESH, 0.0f, "Generating mesh");
                
                PoissonReconstructor poisson;
                poisson.setDepth(params_.poisson_depth);
                final_mesh_ = poisson.reconstruct(*dense_cloud);
                
                updateStatus(ProcessingStatus::COMPLETED, 1.0f, "Reconstruction completed");
            } else if (sub_clouds.size() == 1) {
                // Single video - skip alignment
                updateStatus(ProcessingStatus::DENSE_RECONSTRUCTION, 0.5f, "Generating dense point cloud");
                
                MVSProcessor mvs;
                mvs.setDepthMapResolution(params_.depth_map_resolution);
                auto dense_cloud = mvs.generateDenseCloud(*sub_clouds[0], sub_poses[0], all_frames[0]);
                
                updateStatus(ProcessingStatus::GENERATING_MESH, 0.75f, "Generating mesh");
                
                PoissonReconstructor poisson;
                poisson.setDepth(params_.poisson_depth);
                final_mesh_ = poisson.reconstruct(*dense_cloud);
                
                updateStatus(ProcessingStatus::COMPLETED, 1.0f, "Reconstruction completed");
            }
            
            is_processing_ = false;
            return Result::Success();
            
        } catch (const std::exception& e) {
            is_processing_ = false;
            status_ = ProcessingStatus::ERROR;
            return Result::Error(std::string("Exception: ") + e.what());
        }
    }
};

TruemapCore::TruemapCore() : impl_(std::make_unique<Impl>()) {}

TruemapCore::~TruemapCore() = default;

void TruemapCore::setParameters(const ReconstructionParams& params) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    impl_->params_ = params;
}

void TruemapCore::setProgressCallback(ProgressCallback callback) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    impl_->callback_ = callback;
}

Result TruemapCore::addVideoURL(const std::string& url) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    if (impl_->is_processing_) {
        return Result::Error("Cannot add video while processing");
    }
    impl_->video_urls_.push_back(url);
    Logger::info("Added video URL: " + url);
    return Result::Success();
}

Result TruemapCore::startReconstruction() {
    {
        std::lock_guard<std::mutex> lock(impl_->mutex_);
        if (impl_->is_processing_) {
            return Result::Error("Reconstruction already in progress");
        }
        if (impl_->video_urls_.empty()) {
            return Result::Error("No video URLs added");
        }
    }
    
    // Run in separate thread
    std::thread([this]() {
        impl_->runReconstruction();
    }).detach();
    
    return Result::Success();
}

void TruemapCore::cancelReconstruction() {
    impl_->should_cancel_ = true;
    Logger::info("Cancellation requested");
}

bool TruemapCore::isProcessing() const {
    return impl_->is_processing_;
}

ProcessingStatus TruemapCore::getStatus() const {
    return impl_->status_;
}

std::shared_ptr<Mesh> TruemapCore::getMesh() const {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    return impl_->final_mesh_;
}

Result TruemapCore::exportMesh(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    
    if (!impl_->final_mesh_ || impl_->final_mesh_->empty()) {
        return Result::Error("No mesh available to export");
    }
    
    MeshExporter exporter;
    
    // Determine format from file extension
    if (filepath.ends_with(".obj")) {
        return exporter.exportOBJ(*impl_->final_mesh_, filepath);
    } else if (filepath.ends_with(".ply")) {
        return exporter.exportPLY(*impl_->final_mesh_, filepath, true);
    } else {
        return Result::Error("Unsupported file format. Use .obj or .ply");
    }
}

void TruemapCore::reset() {
    std::lock_guard<std::mutex> lock(impl_->mutex_);
    impl_->video_urls_.clear();
    impl_->final_mesh_.reset();
    impl_->status_ = ProcessingStatus::IDLE;
    Logger::info("Core reset");
}

} // namespace truemap
