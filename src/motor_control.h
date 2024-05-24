#ifndef LEARNESP32_MOTOR_CONTROL_H
#define LEARNESP32_MOTOR_CONTROL_H

#include "driver/ledc.h"

#define DEFAULT_PWM_FREQ_HZ 5000       // Default frequency of PWM signal
#define DEFAULT_PWM_RESOLUTION LEDC_TIMER_10_BIT // Default resolution of PWM duty

typedef struct{
    uint8_t gpio_num;
    ledc_channel_t channel;
    ledc_timer_t timer;
    uint8_t freq_hz;
    ledc_timer_bit_t duty_resolution;
} Motor;

Motor motor_init_c(uint8_t gpio_num, ledc_channel_t channel, uint8_t freq_hz, ledc_timer_bit_t duty_resolution);
Motor motor_init(uint8_t gpio_num, ledc_channel_t channel);

void motor_set_speed(Motor motor, uint8_t speed);

#endif //LEARNESP32_MOTOR_CONTROL_H