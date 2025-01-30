#ifndef TTT_SENSOR_ADAPTERS_SENSOR_H
#define TTT_SENSOR_ADAPTERS_SENSOR_H

#include <ports/sensor.h>
#include <zephyr/drivers/i2c.h>

#include <memory>

namespace TTT::Sensor::Adapters {

class SensorAccessor {
  public:
    virtual ~SensorAccessor() {}

  public:
    /**
     * @brief Returns true when I2C bus is ready
     */
    virtual bool is_bus_ready() const = 0;

    /**
     * @brief Read status byte from device
     */
    virtual std::optional<uint8_t> get_status() const = 0;

    /**
     * @brief Initialize device
     */
    virtual bool init_device() const = 0;

    /**
     * @brief Reset register @p reg
     */
    virtual bool reset_register(uint8_t reg) const = 0;

    /**
     * @brief Triggering device to make a new measurement
     */
    virtual bool trigger_measurement() const = 0;
};

class SensorAccessorImpl : public SensorAccessor {
  public:
    SensorAccessorImpl();

  public:
    virtual bool is_bus_ready() const;
    virtual std::optional<uint8_t> get_status() const;
    virtual bool init_device() const;
    virtual bool reset_register(uint8_t reg) const;
    virtual bool trigger_measurement() const;

  private:
    i2c_dt_spec m_dt;
};

// Fwd declarations
namespace details {
class SensorState;
}

class Sensor : public Ports::Sensor {
  public:
    Sensor(std::unique_ptr<SensorAccessor> accessor);

  public:
    /**
     * @brief Measure sensor data one
     */
    virtual std::optional<Data::SensorData> measure() const;

  public:
    /**
     * @brief Change state to @p state
     *
     * This operation is intended to be called only from within this class not
     * from the outside.
     */
    void change_state(std::unique_ptr<details::SensorState> state);

  private:
    std::unique_ptr<details::SensorState> m_state;
};

namespace details {

class SensorState {
  public:
    virtual ~SensorState() {}
    virtual std::optional<Data::SensorData> measure() const = 0;
};

class SensorStateUninitialized : public SensorState {
  public:
    SensorStateUninitialized(Sensor* parent,
                             std::unique_ptr<SensorAccessor> accessor);
    virtual std::optional<Data::SensorData> measure() const;

  private:
    Sensor* m_parent;
    mutable std::unique_ptr<SensorAccessor> m_accessor;
};

class SensorStateInitialized : public SensorState {
  public:
    SensorStateInitialized(Sensor* parent,
                           std::unique_ptr<SensorAccessor> accessor);
    virtual std::optional<Data::SensorData> measure() const;

  private:
    Sensor* m_parent;
    std::unique_ptr<SensorAccessor> m_accessor;
};

}  // End namespace details

}  // End namespace TTT::Sensor::Adapters

#endif  // TTT_SENSOR_ADAPTERS_SENSOR_H
