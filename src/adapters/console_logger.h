#ifndef TTT_SENSOR_ADAPTERS_CONSOLE_LOGGER_H
#define TTT_SENSOR_ADAPTERS_CONSOLE_LOGGER_H

#include <ports/logger.h>

namespace TTT::Sensor::Adapters {

class ConsoleLogger : public Ports::Logger {
  public:
    ConsoleLogger();

  public:
    virtual void log(const std::string& msg) const;
};

}  // End namespace TTT::Sensor::Adapters

#endif  // TTT_SENSOR_ADAPTERS_CONSOLE_LOGGER_H
