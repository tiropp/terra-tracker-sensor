#ifndef TTT_SENSOR_DATA_SENSOR_DATA_H
#define TTT_SENSOR_DATA_SENSOR_DATA_H

namespace TTT::Sensor::Data {

struct SensorData {
    /**
     * @brief The measured temperature in degree Celsius.
     */
    float temperature;

    /**
     * @brief The measured relative humidity [0-100%]
     */
    float humidity;
};

}  // End namespace TTT::Sensor::Data

#endif  // TTT_SENSOR_DATA_SENSOR_DATA_H
