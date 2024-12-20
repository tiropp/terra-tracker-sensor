#include "wifi.h"

#include <logger.h>
#include <zephyr/net/wifi_mgmt.h>

namespace TTT::Sensor::Adapters {

bool Wifi::connect(const std::string& ssid, const std::string& password) {
    auto iface = net_if_get_wifi_sta();
    if (iface == nullptr) {
        TTT_LOG_ERROR << "Cannot get STA WIFI interface";
        return false;
    }

    struct wifi_connect_req_params params = {0};
    params.ssid = reinterpret_cast<const uint8_t*>(ssid.c_str());
    params.ssid_length = ssid.length();
    params.psk = reinterpret_cast<const uint8_t*>(password.c_str());
    params.psk_length = password.length();
    params.security = WIFI_SECURITY_TYPE_WPA_AUTO_PERSONAL;
    params.channel = WIFI_CHANNEL_ANY;
    params.band = WIFI_FREQ_BAND_2_4_GHZ;

    int ret =
        net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
    if (ret) {
        TTT_LOG_ERROR << "Cannot connect to " << ssid << "(" << ret << ")";
        return false;
    }

    return true;
}

}  // End namespace TTT::Sensor::Adapters
