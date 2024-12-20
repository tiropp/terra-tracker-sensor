#ifndef TTT_SENSOR_DATA_HTTP_RESPONSE_H
#define TTT_SENSOR_DATA_HTTP_RESPONSE_H

#include <cstdint>
#include <string>
#include <vector>

namespace TTT::Sensor::Data {

/**
 * @brief Data struct contains the response from a HTTP request.
 */
struct HttpResponse {
    HttpResponse();

    /**
     * @brief The HTTP status code
     */
    uint16_t status_code;

    /**
     * @brief The full response data
     *
     * This contains header and body information.
     */
    std::vector<uint8_t> data;

    /**
     * @brief Index within @p data where the body starts.
     */
    size_t body_begin;

    /**
     * @brief The length of body within data.
     */
    size_t body_len;

    /**
     * @brief Return the body of the response
     */
    std::string get_body() const;
};

}  // End namespace TTT::Sensor::Data

#endif  // TTT_SENSOR_DATA_HTTP_RESPONSE_H
