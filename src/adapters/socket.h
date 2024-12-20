#ifndef TTT_SENSOR_ADAPTERS_SOCKET_H
#define TTT_SENSOR_ADAPTERS_SOCKET_H

#include <logger.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>

#include <optional>

namespace TTT::Sensor::Adapters {

class Socket {
  public:
    Socket();

  public:
    /**
     * @brief Create and connect socket to given address and port
     *
     * @return True on success, false on error.
     */
    bool open(const std::string& address, uint16_t port);

    /**
     * @brief Close socket
     *
     * @return True on success, false on error.
     */
    bool close();

  public:
    /**
     * @brief Return the platform native socket id
     */
    int native() const { return m_socket; }

  private:
    std::optional<struct sockaddr_in> ipv4addr(const std::string& address,
                                               uint16_t port) const;
    std::optional<struct sockaddr_in6> ipv6addr(const std::string& address,
                                                uint16_t port) const;

    bool create_and_connect_socket(sa_family_t family,
                                   const struct sockaddr* addr,
                                   socklen_t addr_len);

  private:
    int m_socket;
};

}  // End namespace TTT::Sensor::Adapters

#endif  // TTT_SENSOR_ADAPTERS_SOCKET_H
