## Build container for TTT sensor
##
## The container contains the full toolchain including Zephyr RTOS and SDK.
##
from debian:bookworm-20241202-slim

RUN apt update \
    \
    ## Install Debian packages
    && apt install -y \
    git cmake ninja-build gperf ccache dfu-util device-tree-compiler wget \
    python3-dev python3-pip python3-setuptools python3-tk python3-wheel \
    python3-venv \
    xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1 \
    \
    ## Cleanup apt cache
    && rm -rf /var/lib/apt/lists/* \
    \
    ## Some package information
    && cmake --version \
    && python3 --version \
    && dtc --version

RUN \
    ## Install Zephyr sources
    mkdir /zephyr \
    && cd /zephyr \
    && python3 -m venv .venv \
    && . .venv/bin/activate \
    && pip install west \
    && west init . \
    \
    ## Install additional zephyr modules
    && west update \
    && west zephyr-export

RUN \
    ## Install Zephyr SDK
    cd /zephyr \
    && . .venv/bin/activate \
    && pip install patool requests semver \
    && west sdk install -t arm-zephyr-eabi

RUN \
    ## Install additional tools required during compilation
    cd /zephyr \
    && . .venv/bin/activate \
    && pip install pyelftools \
    \
    ## Install firwares for Invineon CYW43439 (Wi-Fi chip)
    ## See https://docs.zephyrproject.org/latest/boards/raspberrypi/rpi_pico/doc/index.html#prerequisites-for-the-pico-w
    && west blobs fetch hal_infineon

# Make sure zephyr venv is used when entering container
ENTRYPOINT [ "/bin/bash", "-c", ". /zephyr/.venv/bin/activate && exec bash"]
