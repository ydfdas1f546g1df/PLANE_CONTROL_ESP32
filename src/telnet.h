// telnet.h

#ifndef TELNET_H
#define TELNET_H

#include "freertos/FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start the Telnet server task.
 */
void start_telnet_server(void);

#ifdef __cplusplus
}
#endif

#endif // TELNET_H
