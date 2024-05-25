#include "led.h"
#include "driver/gpio.h"

void led_reset(){
    gpio_reset_pin(LED_DEBUG);
    gpio_reset_pin(LED_DEBUG_RED);
    gpio_reset_pin(LED_DEBUG_GREEN);
    gpio_reset_pin(LED_DEBUG_BLUE);
    gpio_reset_pin(LED_GREEN_LEFT);
    gpio_reset_pin(LED_RED_LEFT);
    gpio_reset_pin(LED_GREEN_RIGTH);
    gpio_reset_pin(LED_RED_RIGTH);
    gpio_reset_pin(LED_GREEN_MIDDLE);

}
void led_set_direction_init(){
    gpio_set_direction(LED_DEBUG, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_DEBUG_BLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN_LEFT, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_RED_LEFT, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN_RIGTH, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_RED_RIGTH, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN_MIDDLE, GPIO_MODE_OUTPUT);
}