#include "http_response.h"

namespace TTT::Sensor::Data {

HttpResponse::HttpResponse() : status_code(0), body_begin(0), body_len(0) {}

std::string HttpResponse::get_body() const {
    // Range check
    if (body_begin + body_len > data.size()) return std::string{};

    std::string result(data.begin() + body_begin,
                       data.begin() + body_begin + body_len);
    return result;
}

}  // End namespace TTT::Sensor::Data
