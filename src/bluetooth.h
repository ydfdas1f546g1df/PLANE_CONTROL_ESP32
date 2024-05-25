#ifndef LEARNESP32_BLUETOOTH_H
#define LEARNESP32_BLUETOOTH_H

#include "esp_bt.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "nvs_flash.h"
#include "esp_log.h"

bool bluetooth_init(char* device_name);
bool bluetooth_discoverable(bool discoverable);

#endif //LEARNESP32_BLUETOOTH_H
