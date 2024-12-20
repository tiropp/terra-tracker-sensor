#ifndef TTT_SENSOR_PORTS_LOGGER_H
#define TTT_SENSOR_PORTS_LOGGER_H

#include <string>

namespace TTT::Sensor::Ports {

/**
 * @brief Logger interface class
 */
class Logger {
  public:
    virtual ~Logger() {}

  public:
    /**
     * @brief Log a message
     */
    virtual void log(const std::string& message) const = 0;
};

}  // End namespace TTT::Sensor::Ports

#endif  // TTT_SENSOR_PORTS_LOGGER_H
