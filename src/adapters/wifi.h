#ifndef TTT_SENSOR_ADAPTERS_WIFI_H
#define TTT_SENSOR_ADAPTERS_WIFI_H

#include <ports/wifi.h>

namespace TTT::Sensor::Adapters {

class Wifi : public Ports::Wifi {
  public:
    virtual bool connect(const std::string& ssid, const std::string& password);
};

}  // End namespace TTT::Sensor::Adapters

#endif  // TTT_SENSOR_ADAPTERS_WIFI_H
