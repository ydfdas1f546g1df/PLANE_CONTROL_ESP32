#include "driver/gpio.h"
#include "motor_control.h"
#include "bluetooth.h"


// IO pins
#define BUTTON_LEFT 5
#define BUTTON_RIGHT 6
#define MOTOR_LEFT 8
#define MOTOR_RIGHT 9
#define LED_DEBUG 48
#define LED_DEBUG_RED 46
#define LED_DEBUG_GREEN 0
#define LED_DEBUG_BLUE 45


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



void setup() {
    bool is_error = false;
    // Reset all GPIO pins
    gpio_reset();
    gpio_set_direction_init();

    gpio_set_level(LED_DEBUG_BLUE, 0);

    // Initialize motors
    Motor motor_left = motor_init(MOTOR_LEFT, LEDC_CHANNEL_0);
    Motor motor_right = motor_init(MOTOR_RIGHT, LEDC_CHANNEL_1);
    motor_set_speed(motor_left, 0);
    motor_set_speed(motor_right, 0);


    // Initialize Bluetooth
    is_error = bluetooth_init("ESP32_BT_CLASSIC_PLANE") || is_error;

    is_error = bluetooth_discoverable(true) || is_error;

    // Turn off all INTERNAL RGB LEDs level 1 because somehow it is inverted
    gpio_set_level(LED_DEBUG_RED, 1);
    gpio_set_level(LED_DEBUG_GREEN, 1);
    gpio_set_level(LED_DEBUG_BLUE, 1);

    //label errorinit
    if(is_error){
        gpio_set_level(LED_DEBUG_RED, 0);

    }else{
        gpio_set_level(LED_DEBUG_GREEN, 0);
    }

}

void app_main() {
    setup();


}