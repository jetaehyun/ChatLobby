#ifndef USER_THREAD_H
#define USER_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_t user_thread;
int socketConnect;

void *newConnection(void *ptr);
void newThread(int socket);


#endif