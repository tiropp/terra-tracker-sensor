#include "sensor.h"

#include <logger.h>

#include <chrono>
#include <thread>

#define I2C_CONTROLLER DT_ALIAS(i2c_controller)
static const struct device* i2c_controller = DEVICE_DT_GET(I2C_CONTROLLER);

static const uint8_t DHT20_I2C_ADDR = 0x38;
static const uint8_t DHT20_STATUS_MASK = 0x18;
static const uint8_t DHT20_RESET_REGISTERS[] = {0x1B, 0x1C, 0x1D};
static const uint8_t DHT20_RESET_REG_MASK = 0xB0;
static const uint8_t DHT20_TRIGGER_MEASUREMENT[] = {0xAC, 0x33, 0x00};

namespace TTT::Sensor::Adapters {

SensorAccessorImpl::SensorAccessorImpl() {
    TTT_LOG_INFO << "I2C device: " << i2c_controller->name;

    m_dt.bus = i2c_controller;
    m_dt.addr = DHT20_I2C_ADDR;
}

bool SensorAccessorImpl::is_bus_ready() const {
    if (!i2c_is_ready_dt(&m_dt)) {
        TTT_LOG_ERROR << "I2C isn't ready";
        return false;
    }

    return true;
}

std::optional<uint8_t> SensorAccessorImpl::get_status() const {
    uint8_t status[] = {0x00};
    if (i2c_read_dt(&m_dt, status, sizeof(status))) {
        TTT_LOG_ERROR << "Failed to get status";
        return std::nullopt;
    }

    return status[0];
}

bool SensorAccessorImpl::init_device() const {
    using namespace std::chrono_literals;
    for (auto reg : DHT20_RESET_REGISTERS) {
        if (!reset_register(reg)) {
            TTT_LOG_ERROR << "Failed to reset register " << std::showbase
                          << std::hex << static_cast<int>(reg);
            return false;
        }
    }

    return true;
}

bool SensorAccessorImpl::reset_register(uint8_t reg) const {
    using namespace std::chrono_literals;

    uint8_t data1[] = {reg, 0x00, 0x00};
    if (i2c_write_dt(&m_dt, data1, sizeof(data1))) {
        TTT_LOG_ERROR << "Failed to reset register " << std::showbase
                      << std::hex << static_cast<int>(reg) << ": step 1";
        return false;
    }
    std::this_thread::sleep_for(5ms);

    uint8_t data2[] = {0x00, 0x00, 0x00};
    if (i2c_read_dt(&m_dt, data2, sizeof(data2))) {
        TTT_LOG_ERROR << "Failed to reset register " << std::showbase
                      << std::hex << static_cast<int>(reg) << ": step 2";
        return false;
    }
    TTT_LOG_DEBUG << "READ bytes: " << std::showbase << std::hex
                  << static_cast<int>(data2[1]) << " "
                  << static_cast<int>(data2[2]);
    std::this_thread::sleep_for(10ms);

    uint8_t data4[] = {static_cast<uint8_t>(DHT20_RESET_REG_MASK | reg),
                       data2[1], data2[2]};
    if (i2c_write_dt(&m_dt, data4, sizeof(data4))) {
        TTT_LOG_ERROR << "Failed to reset register " << std::showbase
                      << std::hex << static_cast<int>(reg) << ": step 3";
        return false;
    }
    std::this_thread::sleep_for(10ms);

    return true;
}

bool SensorAccessorImpl::trigger_measurement() const {
    // Trigger measurement
    if (i2c_write_dt(&m_dt, DHT20_TRIGGER_MEASUREMENT,
                     sizeof(DHT20_TRIGGER_MEASUREMENT))) {
        TTT_LOG_ERROR << "Failed to trigger measurement";
        return false;
    }

    return false;
}

Sensor::Sensor(std::unique_ptr<SensorAccessor> accessor)
    : m_state(std::make_unique<details::SensorStateUninitialized>(
          this, std::move(accessor))) {}

std::optional<Data::SensorData> Sensor::measure() const {
    return m_state->measure();
}

void Sensor::change_state(std::unique_ptr<details::SensorState> state) {
    m_state = std::move(state);
}

namespace details {

SensorStateUninitialized::SensorStateUninitialized(
    Sensor* parent, std::unique_ptr<SensorAccessor> accessor)
    : m_parent(parent), m_accessor(std::move(accessor)) {}

std::optional<Data::SensorData> SensorStateUninitialized::measure() const {
    using namespace std::chrono_literals;

    // Check I2C bus is ready
    if (!m_accessor->is_bus_ready()) return std::nullopt;

    // Make sure status is indicating proper working state
    auto status = m_accessor->get_status();
    if (!status) return std::nullopt;
    TTT_LOG_DEBUG << "Sensor status: [" << std::showbase << std::hex
                  << static_cast<int>(*status) << "]";

    if ((*status & DHT20_STATUS_MASK) != DHT20_STATUS_MASK) {
        TTT_LOG_INFO << "Sensor needs to be initialized";
        if (!m_accessor->init_device()) return std::nullopt;
    }

    // Sensor is in proper working state hence, one can change state to
    // "Initialized"
    m_parent->change_state(std::make_unique<SensorStateInitialized>(
        m_parent, std::move(m_accessor)));

    std::this_thread::sleep_for(10ms);
    return m_parent->measure();
}

SensorStateInitialized::SensorStateInitialized(
    Sensor* parent, std::unique_ptr<SensorAccessor> accessor)
    : m_parent(parent), m_accessor(std::move(accessor)) {}

std::optional<Data::SensorData> SensorStateInitialized::measure() const {
    using namespace std::chrono_literals;
    TTT_LOG_DEBUG << "Start measurement";

    if (!m_accessor->trigger_measurement()) {
        TTT_LOG_ERROR << "Triggering measurement failed";
        return std::nullopt;
    }

    std::this_thread::sleep_for(80ms);

    return std::nullopt;
}

}  // End namespace details
}  // End namespace TTT::Sensor::Adapters
