#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../../linked_list.h"

struct user_t {
    pthread_t *thread;
    char* username;
    int socket;
    node_t **node;
};

void broadcast(struct user_t userData, char *message);
void alertStatus(bool isOnline, struct user_t userData);


#endif