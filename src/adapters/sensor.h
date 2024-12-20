#ifndef TTT_SENSOR_ADAPTERS_SENSOR_H
#define TTT_SENSOR_ADAPTERS_SENSOR_H

#include <ports/sensor.h>
#include <zephyr/drivers/i2c.h>

namespace TTT::Sensor::Adapters {

class Sensor : public Ports::Sensor {
  public:
    Sensor();

  public:
    /**
     * @brief Measure sensor data one
     */
    virtual std::optional<Data::SensorData> measure() const;

  private:
    /**
     * @brief Read status byte from device
     */
    std::optional<uint8_t> get_status() const;

    /**
     * @brief Initialize device
     */
    bool init_device() const;

    /**
     * @brief Reset register @p reg
     */
    bool reset_register(uint8_t reg) const;

    /**
     * @brief Triggering device to make a new measurement
     */
    bool trigger_measurement() const;

  private:
    i2c_dt_spec m_dt;
};

}  // End namespace TTT::Sensor::Adapters

#endif  // TTT_SENSOR_ADAPTERS_SENSOR_H
