# Terra Tracker - Sensor
[![Build Container](https://github.com/tiropp/terra-tracker-sensor/actions/workflows/create-build-container.yaml/badge.svg)](https://github.com/tiropp/terra-tracker-sensor/actions/workflows/create-build-container.yaml)
[![Build](https://github.com/tiropp/terra-tracker-sensor/actions/workflows/build.yaml/badge.svg)](https://github.com/tiropp/terra-tracker-sensor/actions/workflows/build.yaml)

Terra Tracker Sensor is the sensor part of the TTT (Tiropp's Terra Tracker) system. It will make the environment measurements and send the information to the data collector API. 

The sensor is based on a [Raspberry Pico W](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#picow-technical-specification) device using [Zephyr](https://www.zephyrproject.org) RTOS.


## Building the Code
The building done within a dedicated build container which contains already the Zephyr RTOS and SDK.

### Create Build Container
To create or update the build container run the following (or similar) command:
```
docker build -t zephyr -f container/build.Dockerfile .
```

### Building
Run build container with
```
docker run -it --rm --mount type=bind,src=$PWD,dst=/host zephyr
```

#### Debug Build
Inside the container, make the build with following command for Raspberry Pi Pico W board
```
cd /host
cmake -B _build/debug -G Ninja -D CMAKE_BUILD_TYPE=Debug -D BOARD=rpi_pico/rp2040/w -D FILE_SUFFIX=debug
cmake -B _build/debug -G Ninja -D CMAKE_BUILD_TYPE=Debug -D BOARD=rpi_pico/rp2040/w -D FILE_SUFFIX=debug
cmake --build _build/debug --config Debug
```

**Note**
For whatever reason the cmake configuration step needs to run twice, otherwise the building fill fail.

The device image can be found at `_build/debug/build/zephyr/zephyr.uf2`.

#### Release Build
Inside the container, make the build with following command for Raspberry Pi Pico W board
```
cd /host
cmake -B _build/release -G Ninja -D CMAKE_BUILD_TYPE=Release -D BOARD=rpi_pico/rp2040/w -D FILE_SUFFIX=release
cmake -B _build/release -G Ninja -D CMAKE_BUILD_TYPE=Release -D BOARD=rpi_pico/rp2040/w -D FILE_SUFFIX=release
cmake --build _build/release --config Release
```

**Note**
For whatever reason the cmake configuration step needs to run twice, otherwise the building fill fail.

The device image can be found at `_build/release/build/zephyr/zephyr.uf2`.


### Copy Image to Device
1. Boot the device as USB mass storage by pressing the button on the Pico device before plugging in the USB connector. 
2. Mount the USB mass storage device 
3. Copy the `zephyr.uf2` image to the mass storage device

After copying the file, the Pico will boot right afterwards using the new image. No manual action is needed.

### Console
In debug configuration the image will output the log information as a USB CBC ACM device. I.e. when device is attached to the dev machine, the console log information can be viewed with a serial terminal. E.g. with `minicom` simply use
```
minicom -D /dev/ttyACM0
```
Assuming the device is known within the system as `/dev/ttyACM0`.

