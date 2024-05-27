#include "telnet.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "esp_log.h"
#include "../LED/led.h"


static int client_socket = 0;

//static const char *TAG = "TelnetServer";
static volatile bool keepRunning = true;  // Global flag to control server operation
static Motor *motor_left;
static Motor *motor_right;


void telnet_server_message_proccessor(char *receivedData, int len, int sock, char *response)
{
    // Process received data
    if (response == NULL)
    {
        return;
    }
    {
        receivedData[len - 1] = 0;  // Remove the newline character
    }

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
    sprintf(response, "OK: %s\n", receivedData);

    switch (receivedData[0])
    {
        case '0':
            // Motor Control
            switch (receivedData[1])
            {
                case '0':
                    motor_set_speed(motor_left, atoi(strndup(receivedData + 2, 3)));
                    motor_set_speed(motor_right, atoi(strndup(receivedData + 2, 3)));
                    break;
                case '1':
                    motor_set_speed(motor_left, atoi(strndup(receivedData + 2, 3)));
                    break;
                case '2':
                    motor_set_speed(motor_right, atoi(strndup(receivedData + 2, 3)));
                    break;
            }
            break;
        case '1':
            // LED Control
            /**
             * LED`control
             * 0 - All
             * 1 - Red Internal
             * 2 - Green Internal
             * 3 - Blue Internal
             * 4 - Red External LEFT
             * 5 - Green External LEFT
             * 6 - RED External RIGTH
             * 7 - Green External RIGHT
             * 8 - Green External MIDDLE
             */
             int ledValue = atoi(&receivedData[2]);
            switch (receivedData[1])
            {
                case '0':
                    ledValue = ledValue > 0 ? 0 : 1;
                    gpio_set_level(LED_DEBUG_RED, ledValue);
                    gpio_set_level(LED_DEBUG_GREEN, ledValue);
                    gpio_set_level(LED_DEBUG_BLUE, ledValue);
                    ledValue = ledValue > 0 ? 0 : 1;
                    gpio_set_level(LED_RED_LEFT, ledValue);
                    gpio_set_level(LED_GREEN_LEFT, ledValue);
                    gpio_set_level(LED_RED_RIGTH, ledValue);
                    gpio_set_level(LED_GREEN_RIGTH, ledValue);
                    gpio_set_level(LED_GREEN_MIDDLE, ledValue);
                    break;
                case '1':
                    ledValue = ledValue > 0 ? 0 : 1;
                    gpio_set_level(LED_DEBUG_RED, ledValue);
                    break;
                case '2':
                    ledValue = ledValue > 0 ? 0 : 1;
                    gpio_set_level(LED_DEBUG_GREEN, ledValue);
                    break;
                case '3':
                    ledValue = ledValue > 0 ? 0 : 1;
                    gpio_set_level(LED_DEBUG_BLUE, ledValue);
                    break;
                case '4':
                    gpio_set_level(LED_RED_LEFT, ledValue);
                    break;
                case '5':
                    gpio_set_level(LED_GREEN_LEFT, ledValue);
                    break;
                case '6':
                    gpio_set_level(LED_RED_RIGTH, ledValue);
                    break;
                case '7':
                    gpio_set_level(LED_GREEN_RIGTH, ledValue);
                    break;
                case '8':
                    gpio_set_level(LED_GREEN_MIDDLE, ledValue);
                    break;

            }
            break;
            //case '2':
            // Sensor Data

            //case '3':
            // Other
        default:
            response[0] = 'E';
            response[1] = 'R';
            break;
    }
    send(sock, response, strlen(response), 0);

}


static void telnet_server_task(void *pvParameters)
{
    char rx_buffer[128];
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;


    // Set up the server address
    struct sockaddr_in dest_addr = {
            .sin_addr.s_addr = htonl(INADDR_ANY),
            .sin_family = AF_INET,
            .sin_port = htons(TELNET_PORT)
    };


    // Create a socket
    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0)
    {
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }


    // Bind the socket to the address
    int err = bind(listen_sock, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }


    // Start listening for incoming connections
    err = listen(listen_sock, LISTEN_QUEUE);
    if (err != 0)
    {
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }


    // Loop to accept incoming connections
    while (keepRunning)
    {
        struct sockaddr_in source_addr;
        socklen_t addr_len = sizeof(source_addr);
        // Accept a connection
        int sock = accept(listen_sock, (struct sockaddr*) &source_addr, &addr_len);
        if (sock < 0)
        {
            continue; // Continue trying to accept connections even if one fails
        }


        while (1)
        {
            // Receive data
            ssize_t len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len > 0)
            {
                rx_buffer[len] = 0;  // Null-terminate whatever we received and treat like a string.
                char *receivedData = rx_buffer;
                char *response = malloc(strlen("OK: ") + strlen(receivedData) + strlen("\n") + 1);
                if (len >= 5)
                {
                    telnet_server_message_proccessor(receivedData, len, sock, response);
                } else
                {
                    send(sock, "ER: FALSE LENGfhgH required 5digits,  2 identifier and 3 values first one treated as boolean with led\n", 100, 0);
                    char buffer[100];
                    sprintf(buffer, "given length: %d\n", len);
                    send(sock, buffer, 100, 0);
                }

                free(response);
            } else
            {
                break;  // Break if the connection is closed or an error occurs
            }

        }

        shutdown(sock, SHUT_RDWR);  // Disable further sends and receives on the socket
        close(sock);
    }

    close(listen_sock);
    vTaskDelete(NULL);
}


bool start_telnet_server(void)
{
    if (xTaskCreate(telnet_server_task, "telnet_server_task", 2048, NULL, 5, NULL) != pdPASS)
    {
        return true;  // Return true to indicate error in task creation
    }
    return false;  // Return false to indicate successful task creation
}


void stop_telnet_server()
{
    keepRunning = false;  // Signal the server task to exit
}


void register_motor(Motor *motorLeft, Motor *motorRight)
{
    motor_left = motorLeft;
    motor_right = motorRight;
}


void send_telnet_message(char *message)
{
    int message_len = strlen(message);

    send(client_socket, message, message_len, 0);

}
