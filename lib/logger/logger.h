#ifndef LEARNESP32_LOGGER_H
#define LEARNESP32_LOGGER_H


enum log_level {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};
void set_socket(int socket);
void log_message(char *message, log_level level);

#endif //LEARNESP32_LOGGER_H
