#ifndef TTT_SENSOR_PORTS_HTTP_CLIENT_H
#define TTT_SENSOR_PORTS_HTTP_CLIENT_H

#include <data/http_response.h>

#include <chrono>
#include <string>

namespace TTT::Sensor::Ports {

class HttpClient {
  public:
    virtual ~HttpClient() {}

  public:
    virtual bool open(const std::string& address, uint16_t port) = 0;
    virtual bool close() = 0;

    virtual std::optional<Data::HttpResponse> get(
        const std::string& path, std::chrono::milliseconds timeout,
        const std::vector<std::string>& headers = {}) = 0;
    virtual std::optional<Data::HttpResponse> post(
        const std::string& path, const std::string& data,
        std::chrono::milliseconds timeout,
        const std::vector<std::string>& headers = {}) = 0;
};

}  // End namespace TTT::Sensor::Ports

#endif  // TTT_SENSOR_PORTS_HTTP_CLIENT_H
