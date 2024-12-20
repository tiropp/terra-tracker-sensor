# Terra Tracker - Sensor

Terra Tracker Sensor is the sensor part of the TTT (Tiropp's Terra Tracker) system. It will make the environment measurements and send the information to the data collector API. 

The sensor is based on a Raspberry Pico W device using Zephyr RTOS.


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

Inside the container, make the build with following command for Raspberry Pi Pico W board
```
cd /host
cmake -B _build -G Ninja -D BOARD=rpi_pico/rp2040/w -D FILE_SUFFIX=debug
cmake --build _build
```

The device image can be found at `_build/zephyr/zephyr.uf2`.

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

