#ifndef TTT_SENSOR_ADAPTERS_HTTP_CLIENT_H
#define TTT_SENSOR_ADAPTERS_HTTP_CLIENT_H

#include <adapters/socket.h>
#include <ports/http_client.h>
#include <zephyr/net/http/client.h>

#include <string>

namespace TTT::Sensor::Adapters {

class HttpClient : public Ports::HttpClient {
  public:
    HttpClient(size_t recv_buf_size = 512);

  public:
    virtual bool open(const std::string& address, uint16_t port);
    virtual bool close();

    virtual std::optional<Data::HttpResponse> get(
        const std::string& path, std::chrono::milliseconds timout,
        const std::vector<std::string>& headers = {});
    virtual std::optional<Data::HttpResponse> post(
        const std::string& path, const std::string& data,
        std::chrono::milliseconds timout,
        const std::vector<std::string>& headers = {});

  private:
    std::optional<Data::HttpResponse> request(
        http_method method, const std::string& path,
        std::chrono::milliseconds timeout,
        const std::vector<std::string>& headers,
        const std::string& data = std::string{});

    static void rsp_callback(struct http_response* response,
                             enum http_final_call final_data, void* user_data);

  private:
    size_t m_recv_buf_size;
    std::string m_address;
    uint16_t m_port;
    Socket m_socket;
};

}  // End namespace TTT::Sensor::Adapters

#endif  // TTT_SENSOR_ADAPTERS_HTTP_CLIENT_H
