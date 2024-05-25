

#include "telnet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "esp_log.h"

#define TELNET_PORT 23
#define LISTEN_QUEUE 2

static const char* TAG = "TelnetServer";

static void telnet_server_task(void *pvParameters) {
    char rx_buffer[128];
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    struct sockaddr_in dest_addr = {
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_family = AF_INET,
            .sin_port = htons(TELNET_PORT)
    };

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", TELNET_PORT);

    err = listen(listen_sock, LISTEN_QUEUE);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    while (1) {
        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_in source_addr;
        uint addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Socket accepted");

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Echo the received data back
            if (len > 0) {
                send(sock, rx_buffer, len, 0);
            } else if (len < 0) {
                ESP_LOGE(TAG, "Recv failed: errno %d", errno);
                break;
            } else {
                ESP_LOGI(TAG, "Connection closed");
                break;
            }
        }

        shutdown(sock, 0);
        close(sock);
    }

    close(listen_sock);
    vTaskDelete(NULL);
}

void start_telnet_server(void) {
    xTaskCreate(telnet_server_task, "telnet_server_task", 2048, NULL, 5, NULL);
}