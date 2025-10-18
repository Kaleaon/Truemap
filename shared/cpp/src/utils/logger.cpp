#include "truemap/logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace truemap {

LogLevel Logger::current_level_ = LogLevel::INFO;

void Logger::log(LogLevel level, const std::string& message) {
    if (level < current_level_) {
        return;
    }
    
    // Get timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    // Level string
    const char* level_str;
    switch (level) {
        case LogLevel::DEBUG:   level_str = "DEBUG"; break;
        case LogLevel::INFO:    level_str = "INFO "; break;
        case LogLevel::WARNING: level_str = "WARN "; break;
        case LogLevel::ERROR:   level_str = "ERROR"; break;
        default:                level_str = "?????"; break;
    }
    
    std::string log_message = "[" + oss.str() + "] [" + level_str + "] " + message;
    
#ifdef __ANDROID__
    // Use Android logging
    android_LogPriority priority;
    switch (level) {
        case LogLevel::DEBUG:   priority = ANDROID_LOG_DEBUG; break;
        case LogLevel::INFO:    priority = ANDROID_LOG_INFO; break;
        case LogLevel::WARNING: priority = ANDROID_LOG_WARN; break;
        case LogLevel::ERROR:   priority = ANDROID_LOG_ERROR; break;
        default:                priority = ANDROID_LOG_INFO; break;
    }
    __android_log_print(priority, "Truemap", "%s", message.c_str());
#else
    // Standard output
    if (level == LogLevel::ERROR) {
        std::cerr << log_message << std::endl;
    } else {
        std::cout << log_message << std::endl;
    }
#endif
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::setLogLevel(LogLevel level) {
    current_level_ = level;
}

LogLevel Logger::getLogLevel() {
    return current_level_;
}

} // namespace truemap
