cmake_minimum_required(VERSION 3.25.0)

find_package(Zephyr)

# NB: project() must be called after find_package(Zephyr) otherwise stuff gets
# messed up.
project(terra-tracker-sensor-pi-pico)

set_property(TARGET app PROPERTY CXX_STANDARD 20)
target_sources(app PRIVATE
  src/main.cpp
  src/logger.h
  src/logger.cpp

  src/data/http_response.h
  src/data/http_response.cpp

  src/ports/logger.h
  src/ports/http_client.h
  src/ports/wifi.h
  src/ports/sensor.h

  src/adapters/console_logger.h
  src/adapters/console_logger.cpp
  src/adapters/socket.h
  src/adapters/socket.cpp
  src/adapters/http_client.h
  src/adapters/http_client.cpp
  src/adapters/wifi.h
  src/adapters/wifi.cpp
  src/adapters/sensor.h
  src/adapters/sensor.cpp
)
target_include_directories(app PRIVATE src)

## Give some status information
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
