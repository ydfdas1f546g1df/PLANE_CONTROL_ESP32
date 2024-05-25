// telnet.h

#ifndef TELNET_H
#define TELNET_H

#include "freertos/FreeRTOS.h"
#include "../mototr/motor_control.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start the Telnet server task.
 */
bool start_telnet_server(void);
void stop_telnet_server();
void register_motor(Motor *motorLeft, Motor *motorRight);
#ifdef __cplusplus
}
#endif

#endif // TELNET_H
