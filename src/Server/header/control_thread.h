#ifndef CONTROL_THREAD_H
#define CONTROL_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../../linked_list.h"
#include "../header/communication.h"
#include "../header/log.h"
#include "locks.h"

enum state {
    safe,
    cast,
    kick,
    list
};

void *ctrl_thread(void *ptr);
void create_ctrl_thread(node_t **nodeT);
enum state setState(char *com);

#endif