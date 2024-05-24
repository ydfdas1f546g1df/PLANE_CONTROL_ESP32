#include "driver/gpio.h"
#include "motor_control.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"

// IO pins
#define BUTTON_LEFT 5
#define BUTTON_RIGHT 6
#define MOTOR_LEFT 8
#define MOTOR_RIGHT 9
#define LED_DEBUG 48
#define LED_DEBUG_RED 46
#define LED_DEBUG_GREEN 0
#define LED_DEBUG_BLUE 45

// UART
#define EX_UART_NUM UART_NUM_0
#define BUF_SIZE (1024)
static const char *TAG = "UART";


void gpio_reset() {
    gpio_reset_pin(BUTTON_LEFT);
    gpio_reset_pin(BUTTON_RIGHT);
    gpio_reset_pin(MOTOR_LEFT);
    gpio_reset_pin(MOTOR_RIGHT);
    gpio_reset_pin(LED_DEBUG);
    gpio_reset_pin(LED_DEBUG_RED);
    gpio_reset_pin(LED_DEBUG_GREEN);
    gpio_reset_pin(LED_DEBUG_BLUE);

}

void gpio_set_direction_init() {
    gpio_set_direction(MOTOR_LEFT, GPIO_MODE_OUTPUT);
    gpio_set_direction(MOTOR_RIGHT, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG_BLUE, GPIO_MODE_OUTPUT);

    gpio_set_direction(BUTTON_LEFT, GPIO_MODE_INPUT);
    gpio_set_direction(BUTTON_RIGHT, GPIO_MODE_INPUT);
}

void uart_init() {
    uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .source_clk = UART_SCLK_APB,
    };
    // Configure UART parameters
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(EX_UART_NUM, &uart_config);
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void setup() {
    gpio_set_direction_init();
    gpio_reset();

    gpio_set_level(LED_DEBUG_RED, 1);

    uart_init();

    Motor motor_left = motor_init(MOTOR_LEFT, LEDC_CHANNEL_0);
    Motor motor_right = motor_init(MOTOR_RIGHT, LEDC_CHANNEL_1);

    motor_set_speed(motor_left, 0);
    motor_set_speed(motor_right, 0);

    gpio_set_level(LED_DEBUG_RED, 0);

    gpio_set_level(LED_DEBUG_GREEN, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(LED_DEBUG_GREEN, 0);
}

void app_main() {
    setup();

    char *data = "Hello world!";
    while (1) {
        uart_write_bytes(EX_UART_NUM, data, strlen(data));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}