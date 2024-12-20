#ifndef TTT_SENSOR_LOGGER_H
#define TTT_SENSOR_LOGGER_H

#include <ports/logger.h>

#include <memory>
#include <sstream>

namespace TTT::Sensor {

/**
 * @brief Singleton logger instance
 */
class Logger {
  public:
    static const Ports::Logger& instance();

  public:
    static void set(std::unique_ptr<Ports::Logger> logger) {
        m_instance = std::move(logger);
    }

  public:
    void log(const std::string& msg) const;

  private:
    static std::unique_ptr<Ports::Logger> m_instance;
};

namespace details {

enum class LogLevel { ERROR, WARNING, INFO, DEBUG };

/**
 * @brief The LoggerWriter will write a log message on instance destruction.
 *
 * The class offers operator<< which will store the added messages internally
 * until the instance is destroyed. At that time all content added is sent to
 * logger.
 *
 * This class is not intended to be used directly. Instead, the macros
 * TTT_LOG_XXX() should be used, for simplicity.
 */
class LoggerWriter {
  public:
    LoggerWriter(LogLevel level);
    ~LoggerWriter();

  public:
    template <class T>
    std::ostream& operator<<(const T& val) {
        m_stream << val;
        return m_stream;
    }

  private:
    std::string toString(LogLevel);

  private:
    std::stringstream m_stream;
};

}  // End namespace details

}  // End namespace TTT::Sensor

#define TTT_LOG_ERROR \
    TTT::Sensor::details::LoggerWriter(TTT::Sensor::details::LogLevel::ERROR)

#define TTT_LOG_WARNING \
    TTT::Sensor::details::LoggerWriter(TTT::Sensor::details::LogLevel::WARNING)

#define TTT_LOG_INFO \
    TTT::Sensor::details::LoggerWriter(TTT::Sensor::details::LogLevel::INFO)

#define TTT_LOG_DEBUG \
    TTT::Sensor::details::LoggerWriter(TTT::Sensor::details::LogLevel::DEBUG)

#endif  // TTT_SENSOR_LOGGER_H
