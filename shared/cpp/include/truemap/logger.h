#ifndef TRUEMAP_LOGGER_H
#define TRUEMAP_LOGGER_H

#include <string>

namespace truemap {

/**
 * Logging levels
 */
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

/**
 * Simple logger for Truemap library
 */
class Logger {
public:
    static void log(LogLevel level, const std::string& message);
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    
    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();
    
private:
    static LogLevel current_level_;
};

} // namespace truemap

#endif // TRUEMAP_LOGGER_H
