#include "socket.h"

#include <optional>

namespace TTT::Sensor::Adapters {

Socket::Socket() : m_socket(-1) {}

bool Socket::open(const std::string& address, uint16_t port) {
    // Make sure we close connection first
    close();

    // Check if address is either IPv4 or IPv6 format, if so create a socket.
    if (auto addr = ipv4addr(address, port)) {
        TTT_LOG_DEBUG << "Have found IPv4 address";
        return create_and_connect_socket(
            AF_INET, reinterpret_cast<const struct sockaddr*>(&addr),
            sizeof(addr));
    } else if (auto addr = ipv6addr(address, port)) {
        TTT_LOG_DEBUG << "Have found IPv6 address";
        return create_and_connect_socket(
            AF_INET6, reinterpret_cast<const struct sockaddr*>(&addr),
            sizeof(addr));
    } else {
        // Address is neither a IPv4 nor an IPv6
        // Name resolution isn't supported here yet.
        TTT_LOG_ERROR << "Address '" << address << "' isn't IPv4 or IPv6.";
        return false;
    }
}

bool Socket::close() {
    if (m_socket < 0) return true;

    ::close(m_socket);
    m_socket = -1;
    return true;
}

std::optional<struct sockaddr_in> Socket::ipv4addr(const std::string& address,
                                                   uint16_t port) const {
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &addr.sin_addr) != 1)
        return std::nullopt;

    return addr;
}

std::optional<struct sockaddr_in6> Socket::ipv6addr(const std::string& address,
                                                    uint16_t port) const {
    struct sockaddr_in6 addr = {0};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(port);
    if (inet_pton(AF_INET6, address.c_str(), &addr.sin6_addr) != 1)
        return std::nullopt;

    return addr;
}

bool Socket::create_and_connect_socket(sa_family_t family,
                                       const struct sockaddr* addr,
                                       socklen_t addr_len) {
    m_socket = socket(family, SOCK_STREAM, IPPROTO_TCP);

    // Check socket was actually properly created
    if (m_socket < 0) {
        TTT_LOG_ERROR << "Failed to create socket (" << errno << ")";
        return false;
    }

    auto ret = connect(m_socket, addr, addr_len);
    if (ret < 0) {
        TTT_LOG_ERROR << "Cannot connect socket " << m_socket << " (" << errno
                      << ")";
        close();
        return false;
    }

    return true;
}

}  // End namespace TTT::Sensor::Adapters
