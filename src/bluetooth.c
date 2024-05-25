#include "bluetooth.h"
bool bluetooth_init(char* device_name) {
    esp_err_t ret;

    esp_bluedroid_config_t bt_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();

    ret = esp_bluedroid_init_with_cfg(&bt_cfg);
    if (ret == ESP_FAIL) {
        ESP_LOGE("BD_INIT", "Bluedroid initialization failed: %s", esp_err_to_name(ret));
        return true;
    }

    ret = esp_bluedroid_enable();
    if (ret == ESP_FAIL) {
        ESP_LOGE("BD_ENABLE", "Bluedroid enable failed: %s", esp_err_to_name(ret));
        return true;
    }

    // Set the device name
    esp_bt_dev_set_device_name(device_name);


    // Return no Error
    return false;
}
bool bluetooth_discoverable(bool discoverable){
    esp_err_t err;
    if(discoverable){
        err = esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    }else{
        err = esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
    }

    if (err != ESP_OK){
        return true;
    }
    return false;
}
