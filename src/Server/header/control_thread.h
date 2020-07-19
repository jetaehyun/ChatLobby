#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>

enum state {
    safe,
    cast,
    kick,
    mute
};

void *ctrl_thread(void *ptr);
void create_ctrl_thread();
enum state setState(char *com);

#endif