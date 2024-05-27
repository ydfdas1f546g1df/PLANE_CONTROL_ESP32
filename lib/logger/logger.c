#include "logger.h"
#include "../telnet/telnet.h"
static int socket = -1;

void set_socket(int s) {
    socket = s;
}

/**
 *
 * @param message
 * @param level
 */
void log_message(char *message, log_level level) {
    if (socket == -1) {
        return;
    }
    char *level_str;
    switch (level) {
        case LOG_DEBUG:
            level_str = "DEBUG";
            break;
        case LOG_INFO:
            level_str = "INFO";
            break;
        case LOG_WARNING:
            level_str = "WARNING";
            break;
        case LOG_ERROR:
            level_str = "ERROR";
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }
    char buffer[256];
    snprintf(buffer, 256, "[%s]: %s\n", level_str, message);
    send(socket, buffer, strlen(buffer), 0);
}
