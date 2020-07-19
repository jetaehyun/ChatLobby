#include "header/log.h"

/**
 * @brief get timestamp
 * 
 * @param buf string to cat
 */
void timestamp(char *buf) {
    time_t t;
    t = time(NULL);

    // format timestamp [xx:xx:xx]
    strncpy(buf, "[", 2);
    strncat(buf, &(ctime(&t))[11], 8);
    strncat(buf, "] ", 3);
    buf[strlen(buf)] = '\0';

    // printf("Time: %s\n", buf);
}