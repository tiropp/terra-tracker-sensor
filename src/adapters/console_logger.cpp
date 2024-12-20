#include "console_logger.h"

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/drivers/uart.h>

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
             "Console device is not ACM CDC UART device");

namespace TTT::Sensor::Adapters {

ConsoleLogger::ConsoleLogger() {
    const struct device* const dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));

    if (usb_enable(NULL)) {
        return;
    }

    // Poll if the DTR flag was set
    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(dev, UART_LINE_CTRL_DTR, &dtr);
        // Give CPU resources to low priority threads.
        k_sleep(K_MSEC(100));
    }
}

void ConsoleLogger::log(const std::string& msg) const {
    printk("%s\n", msg.c_str());
}

}  // End namespace TTT::Sensor::Adapters
