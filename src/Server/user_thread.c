#include "header/user_thread.h"

void *newConnection(void *ptr) {
    int valread;
    int socket = *(int *)ptr;
    // printf("Connection: %d", socket);
    char buffer[1024] = {0};

    while(true) {
        valread = read(socket, buffer, 1024);
        if(valread > 0) printf("%s\n", buffer);
    }
}

void newThread(int socket) {
    int *socketT = malloc(sizeof(*socketT));
    *socketT = socket;
    pthread_create(&user_thread, NULL, newConnection, socketT);
}
