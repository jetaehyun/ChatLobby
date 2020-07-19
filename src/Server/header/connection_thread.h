#ifndef CONNECTION_THREAD_H
#define CONNECTION_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../../linked_list.h"
#include "log.h"

struct user_t {
    pthread_t *thread;
    char* username;
    int socket;
    node_t **node;
};

void broadcast(struct user_t userData, char *message);
void alertStatus(bool isOnline, struct user_t userData);
void closeIO(struct user_t userData);
void *newConnection(void *ptr);
void create_thread(int socket, char *username, node_t **nodeT);
pthread_t *allocateThread();

#endif