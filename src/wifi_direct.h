#ifndef LEARNESP32_WIFI_H
#define LEARNESP32_WIFI_H


#include "esp_wifi.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "wifi_direct.h"

bool wifi_init_p2p();
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif //LEARNESP32_WIFI_H
