#include "wifi_direct.h"



bool wifi_init_p2p() {
    esp_err_t ret;

    // Initialize NVS — required for WiFi
    ret = nvs_flash_init();
    if (ret != ESP_OK) {
        ESP_ERROR_CHECK(nvs_flash_erase());  // Erase if needed and reinit
        ret = nvs_flash_init();
        if (ret != ESP_OK) {
            return true;  // Return true to indicate error
        }
    }

    // Initialize the TCP/IP stack
    if (esp_netif_init() != ESP_OK) {
        return true;  // Return true to indicate error
    }

    // Create the default event loop necessary for the WiFi and Netif drivers
    if (esp_event_loop_create_default() != ESP_OK) {
        return true;  // Return true to indicate error
    }

    // Initialize WiFi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&cfg) != ESP_OK) {
        return true;  // Return true to indicate error
    }

    // Create a default WiFi AP_STA interface for P2P
    esp_netif_create_default_wifi_ap();
    esp_netif_t* wifi_netif_sta = esp_netif_create_default_wifi_sta();
    if (!wifi_netif_sta) {
        return true;  // Return true to indicate error
    }

    // Set WiFi to AP_STA mode which is required for P2P
    if (esp_wifi_set_mode(WIFI_MODE_APSTA) != ESP_OK) {
        return true;  // Return true to indicate error
    }

    // Configure the AP settings for P2P
    wifi_config_t ap_config = {
            .ap = {
                    .ssid = "ESP32_PLANE_CONTROLL",
                    .ssid_len = 0,  // 0 means use the full length of the SSID
                    .channel = 1,
                    .password = "",
                    .max_connection = 4,
                    .authmode = WIFI_AUTH_OPEN
            }
    };
    if (esp_wifi_set_config(ESP_IF_WIFI_AP, &ap_config) != ESP_OK) {
        return true;  // Return true to indicate error
    }

    // Start WiFi
    if (esp_wifi_start() != ESP_OK) {
        return true;  // Return true to indicate error
    }

    // Register handler for WiFi and IP events
    if (esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL) != ESP_OK) {
        return true;  // Return true to indicate error
    }

    return false;  // Return false to indicate success
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // Handle WiFi events here, such as starting P2P discovery when WiFi is started
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_ERROR_CHECK(wifi_init_p2p());
    }
}