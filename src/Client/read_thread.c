#include "header/read_thread.h"

pthread_t threadT;

void *newConnection(void *ptr) {
    int socket = *(int *)ptr;
    int data = 0;

    char buffer[1024] = {'\0'};
    while(true) {
        data = recv(socket, buffer, 1024, 0);
        // printf("%d\n", data);
        if(data > 0) {

            buffer[strlen(buffer)] = '\0'; 
            printf("%s\n", buffer);

        } else if(data <= 0) break;

        usleep(500000);
    }

    printf("Server has disconnected...\n");
    pthread_exit(NULL);

}

/**
 * @brief Create a thread object
 * 
 * @param socket socket file descriptor 
 * @param thread pointer to pthread_t obj
 */
void create_thread(int *socket, pthread_t *thread) {
    pthread_create(&threadT, NULL, newConnection, socket);
}

/**
 * @brief malloc new pthread_t
 * 
 * @return pthread_t* 
 */
pthread_t *allocateThread() {
    return malloc(sizeof(pthread_t));
}