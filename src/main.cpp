#include <adapters/console_logger.h>
#include <adapters/http_client.h>
#include <adapters/sensor.h>
#include <adapters/wifi.h>
#include <logger.h>

#include <chrono>
#include <memory>
#include <thread>

// @TODO: needs to be removed
// #include <zephyr/kernel.h>

int main() {
    using namespace std::chrono_literals;

    TTT::Sensor::Logger::set(
        std::make_unique<TTT::Sensor::Adapters::ConsoleLogger>());

    TTT_LOG_INFO << "Hello World " << CONFIG_ARCH;
    std::this_thread::sleep_for(5s);

    /*
    while (true) {
        TTT_LOG_INFO << "Try to connect wifi ... ";

        TTT::Sensor::Adapters::Wifi wifi;
        if (wifi.connect("XXX",
                         "XXX"))
            TTT_LOG_INFO << "Wifi connecte!";
        break;

        std::this_thread::sleep_for(1s);
    }

    TTT::Sensor::Adapters::HttpClient client;
    while (true) {
        TTT_LOG_INFO << "Try to connect to server ...";
        if (!client.open("192.168.10.209", 5000)) {
            TTT_LOG_ERROR << "Cannot open HTTP client";
        } else {
            TTT_LOG_INFO << "Connection established";
            break;
        }

        std::this_thread::sleep_for(1s);
    }

    while (true) {
        TTT_LOG_INFO << "Make GET request ... ";
        if (auto rsp = client.get("/api/call_count", 5000ms)) {
            std::string msg;
            for (auto c : rsp->data) msg += c;
            TTT_LOG_INFO << "Response[" << rsp->status_code << "]: " << msg
                         << "\nBODY[" << rsp->body_begin << "," << rsp->body_len
                         << "]:" << rsp->get_body();
        } else {
            TTT_LOG_ERROR << "GET request failed";
        }
        std::this_thread::sleep_for(1s);

        TTT_LOG_INFO << "Make POST request ...";
        std::vector<std::string> headers;
        headers.push_back("Content-Type: application/json");
        if (auto rsp = client.post("/api/measurements", "{\"temp\": 30.3}",
                                   5000ms, headers)) {
            std::string msg;
            for (auto c : rsp->data) msg += c;
            TTT_LOG_INFO << "Response[" << rsp->status_code << "]: " << msg
                         << "\nBODY:" << rsp->get_body();
        } else {
            TTT_LOG_ERROR << "POST request failed";
        }
        std::this_thread::sleep_for(1s);

        // k_sleep(K_SECONDS(1));
    }
    */

    TTT::Sensor::Adapters::Sensor sensor;
    while (true) {
        TTT_LOG_INFO << "Get sensor data ... ";
        auto m = sensor.measure();

        std::this_thread::sleep_for(1s);
    }
}
