#ifndef READ_THREAD_H
#define READ_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>


void *newConnection(void *ptr);
void create_thread(int  *socket, pthread_t *thread);
pthread_t *allocateThread();

#endif