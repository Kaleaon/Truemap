#ifndef TRUEMAP_VIDEO_PROCESSOR_H
#define TRUEMAP_VIDEO_PROCESSOR_H

#include "types.h"
#include <string>
#include <vector>

namespace truemap {

/**
 * Handles video downloading and frame extraction
 */
class VideoProcessor {
public:
    VideoProcessor();
    ~VideoProcessor();
    
    /**
     * Download video from URL
     * @param url Video URL (YouTube)
     * @param output_path Local file path to save video
     * @param callback Progress callback
     * @return Result indicating success or error
     */
    Result downloadVideo(const std::string& url, const std::string& output_path, ProgressCallback callback = nullptr);
    
    /**
     * Extract frames from video file
     * @param video_path Path to video file
     * @param output_dir Directory to save extracted frames
     * @param frame_rate Frames per second to extract
     * @param callback Progress callback
     * @return Vector of extracted frame paths
     */
    std::vector<std::string> extractFrames(const std::string& video_path, const std::string& output_dir, float frame_rate, ProgressCallback callback = nullptr);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace truemap

#endif // TRUEMAP_VIDEO_PROCESSOR_H
