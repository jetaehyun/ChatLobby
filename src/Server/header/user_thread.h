#ifndef USER_THREAD_H
#define USER_THREAD_H

#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>

struct user_t {
    pthread_t *thread;
    // const char* username;
    int sock;
};

void closeIO(struct user_t userN);
void *newConnection(void *ptr);
void newThread(int socket);
long long retrieveTime();
pthread_t *allocateThread();

#endif