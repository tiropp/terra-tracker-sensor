#include "http_client.h"

#include <chrono>
#include <optional>

namespace TTT::Sensor::Adapters {

bool HttpClient::open(const std::string& address, uint16_t port) {
    m_address = address;
    m_port = port;
    return true;
}

bool HttpClient::close() { return m_socket.close(); }

std::optional<Data::HttpResponse> HttpClient::get(
    const std::string& path, std::chrono::milliseconds timeout,
    const std::vector<std::string>& headers) {
    return request(HTTP_GET, path, timeout, headers);
}

std::optional<Data::HttpResponse> HttpClient::post(
    const std::string& path, const std::string& data,
    std::chrono::milliseconds timeout,
    const std::vector<std::string>& headers) {
    return request(HTTP_POST, path, timeout, headers, data);
}

std::optional<Data::HttpResponse> HttpClient::request(
    http_method method, const std::string& path,
    std::chrono::milliseconds timeout, const std::vector<std::string>& headers,
    const std::string& data) {
    // First connect socket
    // NB: Socket cannot be kept open, since server will terminate the
    // connection once the request is done.
    if (!m_socket.open(m_address, m_port)) {
        TTT_LOG_ERROR << "Failed to connect to " << m_address << ":" << m_port;
        return std::nullopt;
    }

    // Prepare headers
    // Need to append manually '\r\n' for each header
    std::vector<std::string> hdrs(headers);
    for (auto& hdr : hdrs) hdr.append("\r\n");
    // And repack header as a const char**
    std::vector<const char*> hdrs_;
    hdrs_.reserve(hdrs.size() + 1);
    for (const auto& hdr : hdrs) hdrs_.push_back(hdr.c_str());
    hdrs_.push_back(nullptr);

    // Prepare response structure
    Data::HttpResponse rsp;
    const auto data_size = 512;
    rsp.data.resize(data_size);

    // Make GET request
    struct http_request req = {0};
    req.protocol = "HTTP/1.1";
    req.method = method;
    req.host = m_address.c_str();
    req.url = path.c_str();
    req.payload = data.c_str();
    req.payload_len = data.length();
    req.optional_headers = hdrs_.data();
    req.response = &HttpClient::rsp_callback;
    req.recv_buf = &rsp.data[0];
    req.recv_buf_len = data_size;

    if (http_client_req(m_socket.native(), &req, timeout.count(),
                        reinterpret_cast<void*>(&rsp)) == 0)
        return std::nullopt;

    return rsp;
}

void HttpClient::rsp_callback(struct http_response* response,
                              enum http_final_call final_data,
                              void* user_data) {
    auto rsp = reinterpret_cast<Data::HttpResponse*>(user_data);
    if (final_data == HTTP_DATA_FINAL) {
        rsp->status_code = response->http_status_code;
        rsp->data.resize(response->data_len);
        if (response->body_frag_start >= response->recv_buf)
            rsp->body_begin = response->body_frag_start - response->recv_buf;
        else
            rsp->body_begin = 0;
        rsp->body_len = response->body_frag_len;
    }
}

}  // End namespace TTT::Sensor::Adapters
