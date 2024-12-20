#include "sensor.h"

#include <logger.h>

#include <chrono>
#include <thread>

#define I2C_CONTROLLER DT_ALIAS(i2c_controller)
static const struct device* i2c_controller = DEVICE_DT_GET(I2C_CONTROLLER);

#define DHT20_I2C_ADDR 0x38
#define DHT20_STATUS_MASK 0x18
#define DHT20_RESET_REGISTERS \
    { 0x1B, 0x1C, 0x1D }
#define DHT20_RESET_REG_MASK 0xB0
#define DHT20_TRIGGER_MEASUREMENT \
    { 0xAC, 0x33, 0x00 }

namespace TTT::Sensor::Adapters {

Sensor::Sensor() {
    // auto dev = device_get_binding("i2c@40044000");
    // if (!dev) {
    //     TTT_LOG_ERROR << "Failed to get I2C device";
    //     return;
    // }
    // m_dt.bus = dev;
    TTT_LOG_INFO << "I2C device: " << i2c_controller->name;

    m_dt.bus = i2c_controller;
    m_dt.addr = DHT20_I2C_ADDR;
}

std::optional<Data::SensorData> Sensor::measure() const {
    using namespace std::chrono_literals;

    // i2c_dt_spec dt = I2C_DT_SPEC_INST_GET(0);

    // auto device = device_get_binding("I2C_0");
    // if (!device) {
    //     TTT_LOG_ERROR << "Cannot find device";
    //     return std::nullopt;
    // }

    // Check I2C bus is ready
    if (!i2c_is_ready_dt(&m_dt)) {
        TTT_LOG_ERROR << "I2C isn't ready";
        return std::nullopt;
    }

    /*
    // Request status
    uint8_t status_req[] = {0x71};
    uint8_t status_rsp[] = {0x00};

    struct i2c_msg msgs[2];
    msgs[0].buf = status_req;
    msgs[0].len = sizeof(status_req);
    msgs[0].flags = I2C_MSG_RESTART | I2C_MSG_WRITE;
    msgs[1].buf = status_rsp;
    msgs[1].len = sizeof(status_rsp);
    msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

    if (!i2c_transfer(i2c_controller, msgs, ARRAY_SIZE(msgs), DHT20_I2C_ADDR)) {
        TTT_LOG_ERROR << "Requesting status from sensor failed (" << errno
                      << ")";
        return std::nullopt;
    } else {
        TTT_LOG_DEBUG << "Sensor status: [" << std::showbase << std::hex
                      << static_cast<int>(status_rsp[0]) << "]";
        return std::nullopt;
    }
    */

    // uint8_t status_req[] = {0x71};
    // if (!i2c_write_dt(&dt, status_req, sizeof(status_req))) {
    //     TTT_LOG_ERROR << "Writing status request failed";
    //     return std::nullopt;
    // }

    // uint8_t status_rsp[] = {0x00};
    // if (!i2c_read_dt(&dt, status_rsp, sizeof(status_rsp))) {
    //     TTT_LOG_ERROR << "Reading status response failed";
    //     return std::nullopt;
    // }

    auto status = get_status();
    if (!status) return std::nullopt;
    TTT_LOG_DEBUG << "Sensor status: [" << std::showbase << std::hex
                  << static_cast<int>(*status) << "]";

    if ((*status & DHT20_STATUS_MASK) != DHT20_STATUS_MASK) {
        TTT_LOG_INFO << "Sensor needs to be initialized";
        init_device();
    }

    std::this_thread::sleep_for(10ms);
    if (!trigger_measurement()) {
        TTT_LOG_ERROR << "Triggering measurement failed";
        return std::nullopt;
    }

    return std::nullopt;
}

std::optional<uint8_t> Sensor::get_status() const {
    uint8_t status[] = {0x00};
    if (i2c_read_dt(&m_dt, status, sizeof(status))) {
        TTT_LOG_ERROR << "Failed to get status";
        return std::nullopt;
    }

    return status[0];
}

bool Sensor::init_device() const {
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

bool Sensor::reset_register(uint8_t reg) const {
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

bool Sensor::trigger_measurement() const {
    using namespace std::chrono_literals;

    // Trigger measurement
    if (i2c_write_dt(&m_dt, DHT20_TRIGGER_MEASUREMENT,
                     sizeof(DHT20_TRIGGER_MEASUREMENT))) {
        TTT_LOG_ERROR << "Failed to trigger measurement";
        return false;
    }

    std::this_thread::sleep_for(80ms);

    return false;
}

}  // End namespace TTT::Sensor::Adapters
