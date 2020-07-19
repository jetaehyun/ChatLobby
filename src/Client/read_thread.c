#include "header/read_thread.h"

pthread_t threadT;

void *newConnection(void *ptr) {
    int socket = *(int *)ptr;
    int data;

    char buffer[1024] = {0};
    while(true) {
        data = recv(socket, buffer, 1024, 0);
        // printf("%d\n", data);
        if(data > 0) {
            buffer[strlen(buffer)] = '\0'; 
            printf("%s\n", buffer);
        }

        usleep(500000);
    }
    pthread_exit(NULL);

}

void create_thread(int *socket, pthread_t *thread) {
    pthread_create(&threadT, NULL, newConnection, socket);
}

pthread_t *allocateThread() {
    return malloc(sizeof(pthread_t));
}