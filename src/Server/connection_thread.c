#include "header/connection_thread.h"

int currentUsers = 0;
pthread_mutex_t exitLock; 
pthread_mutex_t nodeLock; 
pthread_mutex_t commLock; 

void *newConnection(void *ptr) {
    int data;
    struct user_t userN = *(struct user_t *)ptr;
    // printList(userN.node);

    char buffer[1024] = {0};
    while(true) {
        data = read(userN.sock, buffer, 1024);
        if(data > 0) {
            pthread_mutex_lock(&commLock);
            sendData(userN, buffer);
            pthread_mutex_unlock(&commLock);
        } else break;

        usleep(500000);         
    }

    closeIO(userN);
    pthread_exit(NULL);
}

void sendData(struct user_t userData, char *buffer) {
    node_t *node = *(userData.node);    
    char *message = malloc(sizeof(char) * 1024);

    int count = 0;
    while(*(userData.username + count) != '\0') count++;

    strncat(message, userData.username, count + 1);
    strncat(message, ": ", 3);
    strncat(message, buffer, 1021 - count);
    // printList((userData.node));
    while(node != NULL) {
        // printf("\nconnection: %d\n", node->connection);
        if(strcmp(node->username, userData.username) != 0) {
            printf("connection: %d\n", node->connection);

            send(node->connection, message, 1024, 0);
        }

        node = node->next;
    }

    free(message);
}

void closeIO(struct user_t userN) {
    pthread_mutex_lock(&exitLock); 
    currentUsers--;
    // dequeue(userN.node, userN.username); // cause seg issue
    pthread_mutex_unlock(&exitLock);
    printf("%s has left the chat...\n", userN.username);
    close(userN.sock);
    free(userN.thread);
}

void create_thread(int socket, char *name, node_t **nodeT) {
    pthread_mutex_lock(&nodeLock);     
    enqueue(nodeT, socket, name);
    pthread_mutex_unlock(&nodeLock);
    
    currentUsers++;
    struct user_t newUser;
    newUser.thread = allocateThread();
    newUser.sock = socket;
    newUser.username = name;
    newUser.node = nodeT;
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
