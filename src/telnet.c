#include "telnet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "esp_log.h"


#define TELNET_PORT 23
#define LISTEN_QUEUE 2

static const char* TAG = "TelnetServer";
static volatile bool keepRunning = true;  // Global flag to control server operation
static Motor *motor_left;
static Motor *motor_right;

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
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }

    err = listen(listen_sock, LISTEN_QUEUE);
    if (err != 0) {
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }

    while (keepRunning) {
        struct sockaddr_in source_addr;
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            continue; // Continue trying to accept connections even if one fails
        }

        while (1) {
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len > 6) {
                rx_buffer[len] = 0;  // Null-terminate whatever we received and treat like a string.

                // Process received data
                char* receivedData = rx_buffer;
                char* response = malloc(strlen("OK: ") + strlen(receivedData) + strlen("\n") + 1);
                /**>
                 * First Digit is type of command
                 * 0 - Motor Control
                 * 1 - LED Control
                 * 2 - Sensor Data
                 * 3 - Other
                 *
                 * Second Digit is the specific LED/MOTOR/SENSOR/other this is specific except these
                 * 0 - All
                 * 1 - Left
                 * 2 - Right
                 *
                 * Third, Fourth & Fifth Digit is the VALUE
                 * Like % degree or other
                 * */
                if(receivedData[0] == '0'){
                    // Motor Control
                    switch (receivedData[2]) {
                        case '0':
                            motor_set_speed(motor_left, atoi(strndup(receivedData + 2, 3)));
                            motor_set_speed(motor_right, atoi(strndup(receivedData + 2, 3)));
                            break;
                    }
                }else if (receivedData[0] == '1'){
                    // LED Control
                }else if (receivedData[0] == '2'){
                    // Sensor Data
                }else if (receivedData[0] == '3'){
                    // Other
                }

                send(sock, response, len, 0);//
                free(response);
            } else {
                break;  // Break if the connection is closed or an error occurs
            }
        }

        shutdown(sock, SHUT_RDWR);  // Disable further sends and receives on the socket
        close(sock);
    }

    close(listen_sock);
    vTaskDelete(NULL);
}

bool start_telnet_server(void) {
    if (xTaskCreate(telnet_server_task, "telnet_server_task", 2048, NULL, 5, NULL) != pdPASS) {
        return true;  // Return true to indicate error in task creation
    }
    return false;  // Return false to indicate successful task creation
}
void stop_telnet_server() {
    keepRunning = false;  // Signal the server task to exit
}
void register_motor(Motor *motorLeft, Motor *motorRight){
    motor_left = motorLeft;
    motor_right = motorRight;
}