#ifndef TTT_SENSOR_PORTS_WIFI_H
#define TTT_SENSOR_PORTS_WIFI_H

#include <string>

namespace TTT::Sensor::Ports {

class Wifi {
  public:
    virtual ~Wifi() {}

  public:
    virtual bool connect(const std::string& ssid,
                         const std::string& password) = 0;
};
}  // End namespace TTT::Sensor::Ports

#endif  // TTT_SENSOR_PORTS_WIFI_H
