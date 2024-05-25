//
// Created by arch on 24.05.24.
//

#include "motor_control.h"
#include "driver/ledc.h"


// Initialize the PWM timer (shared among all motors)
static void pwm_timer_init_once(uint32_t freq_hz, ledc_timer_bit_t duty_resolution) {
    static bool is_initialized = false;
    if (!is_initialized) {
        ledc_timer_config_t pwm_timer_config = {
                .duty_resolution = duty_resolution,
                .freq_hz = freq_hz,
                .speed_mode = LEDC_LOW_SPEED_MODE,
                .timer_num = LEDC_TIMER_0
        };

#if defined(SOC_LEDC_SUPPORT_HS_MODE) // Check if high-speed mode is supported
        pwm_timer_config.speed_mode = LEDC_HIGH_SPEED_MODE; // Set to HIGH speed mode if supported
#endif

        ledc_timer_config(&pwm_timer_config);
        is_initialized = true;
    }
}

// Initialize a motor with custom settings
Motor motor_init_c(uint8_t gpio_num, ledc_channel_t channel, uint32_t freq_hz, ledc_timer_bit_t duty_resolution){
    pwm_timer_init_once(freq_hz, duty_resolution);

    Motor motor = {
            .gpio_num = gpio_num,
            .channel = channel,
            .freq_hz = freq_hz,
            .duty_resolution = duty_resolution
    };

    ledc_channel_config_t channel_config = {
            .channel    = channel,
            .duty       = 0,
            .gpio_num   = gpio_num,
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_sel  = LEDC_TIMER_0
    };

#if defined(SOC_LEDC_SUPPORT_HS_MODE) // Check if high-speed mode is supported
    channel_config.speed_mode = LEDC_HIGH_SPEED_MODE;
#endif

    ledc_channel_config(&channel_config);

    return motor;
}

// Initialize a motor with default settings
Motor motor_init(uint8_t gpio_num, ledc_channel_t channel) {
    return motor_init_c(gpio_num, channel, DEFAULT_PWM_FREQ_HZ, DEFAULT_PWM_RESOLUTION);
}

// Set motor speed (duty cycle)
void motor_set_speed(Motor motor, uint8_t speed) {
#if defined(SOC_LEDC_SUPPORT_HS_MODE) // Check if high-speed mode is supported
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, motor.channel, speed);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, motor.channel);
#else
    ledc_set_duty(LEDC_LOW_SPEED_MODE, motor.channel, speed);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, motor.channel);
#endif
}

