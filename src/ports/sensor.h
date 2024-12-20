#ifndef TTT_SENSOR_PORTS_SENSOR_H
#define TTT_SENSOR_PORTS_SENSOR_H

#include <data/sensor_data.h>

#include <optional>

namespace TTT::Sensor::Ports {

class Sensor {
  public:
    virtual ~Sensor() {}

  public:
    /**
     * @brief Measure sensor data one
     */
    virtual std::optional<Data::SensorData> measure() const = 0;
};

}  // End namespace TTT::Sensor::Ports

#endif  // TTT_SENSOR_PORTS_SENSOR_H
