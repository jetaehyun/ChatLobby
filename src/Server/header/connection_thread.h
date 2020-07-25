#ifndef CONNECTION_THREAD_H
#define CONNECTION_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "../../linked_list.h"
#include "communication.h"
#include "log.h"

void closeIO(struct user_t userData, bool wasKicked);
void *newConnection(void *ptr);
void create_thread(int socket, char *username, node_t **nodeT);
pthread_t *allocateThread();
bool isKicked(node_t **node, char *username);

#endif