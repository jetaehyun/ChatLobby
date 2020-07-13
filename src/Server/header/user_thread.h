#ifndef USER_THREAD_H
#define USER_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include "../../linked_list.h"

struct user_t {
    pthread_t *thread;
    char* username;
    int sock;
    node_t **node;
};

void closeIO(struct user_t userN);
void *newConnection(void *ptr);
void create_thread(int socket, char *name, node_t **nodeT);
long long retrieveTime();
pthread_t *allocateThread();

#endif