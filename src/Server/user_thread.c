#include "header/user_thread.h"

int currentUsers = 0;
pthread_mutex_t incrementLock; 

void *newConnection(void *ptr) {
    int valread;
    struct user_t userN = *(struct user_t *)ptr;
    char buffer[1024] = {0};
    while(true) {
        valread = read(userN.sock, buffer, 1024);
        if(valread > 0) printf("%s: %s\n", userN.username, buffer);
        else break;
    }
    closeIO(userN);
    pthread_exit(NULL);
}

void closeIO(struct user_t userN) {
    pthread_mutex_lock(&incrementLock); 
    currentUsers--;
    pthread_mutex_unlock(&incrementLock);
    printf("%s has exited...\n", userN.username);
    close(userN.sock);
    free(userN.thread);

}

void create_thread(int socket, char *name) {
    currentUsers++;
    struct user_t newUser;
    newUser.thread = allocateThread();
    newUser.sock = socket;
    newUser.username = name;
    pthread_create(newUser.thread, NULL, newConnection, &newUser);
    printf("\nUsers: %d\n", currentUsers);

}

pthread_t *allocateThread() {
    return malloc(sizeof(pthread_t));
}

long long retrieveTime() {
    time_t t = time(NULL);
    return t;
}

