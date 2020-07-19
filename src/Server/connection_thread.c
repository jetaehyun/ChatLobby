#include "header/connection_thread.h"

int currentUsers = 0;
pthread_mutex_t exitLock; 
pthread_mutex_t nodeLock; 
pthread_mutex_t commLock; 

void *newConnection(void *ptr) {
    int data = 0;
    struct user_t userData = *(struct user_t *)ptr;

    // buffers for message transmission
    char buffer[1024] = {'\0'};
    char message[1024] = {'\0'};
    char t[12] = {'\0'};

    while(true) {

        data = recv(userData.socket, buffer, 1024, 0);
        if(data > 0 && currentUsers > 1) {
            // Form Message
            timestamp(t);
            strncat(message, t, strlen(t));
            strncat(message, userData.username, strlen(userData.username));
            strncat(message, ": ", 3);
            strncat(message, buffer, 1024 - strlen(message));  

            // send message to open sockets
            pthread_mutex_lock(&commLock);
            broadcast(userData, message);
            pthread_mutex_unlock(&commLock);

            memset(message, 0, sizeof(message));
            memset(t, 0, sizeof(t));

        } else break;

        usleep(500000);         
    }

    closeIO(userData);
    pthread_exit(NULL);
}

/**
 * @brief send msg to all open sockets
 * 
 * @param userData active user trying to send a msg
 * @param message string to send 
 */
void broadcast(struct user_t userData, char *message) {
    node_t *node = *(userData.node);    
        
    // Send to all users
    while(node != NULL) {
        // printf("\nconnection: %d\n", node->connection);
        if(strcmp(node->username, userData.username) != 0) 
            send(node->connection, message, 1024, 0);
        
        node = node->next;
    }

}

/**
 * @brief close/free 
 * 
 * @param userData user that has exited
 */
void closeIO(struct user_t userData) {
    // adjust stats
    pthread_mutex_lock(&exitLock); 
    currentUsers--;
    alertStatus(false, userData);
    free(dequeue(userData.node, userData.username));
    pthread_mutex_unlock(&exitLock);

    printf("%s disconnected\n", userData.username);
    close(userData.socket);
    free(userData.thread);
    free(userData.username);
}

/**
 * @brief Create a thread object
 * 
 * @param socket socket file descriptor
 * @param username name of user
 * @param nodeT pointer to linked list
 */
void create_thread(int socket, char *username, node_t **nodeT) {
    
    // create new user_t obj
    struct user_t newUser;
    newUser.thread = allocateThread();
    newUser.socket = socket;
    newUser.username = username;
    newUser.node = nodeT;    
    
    // Add new user to list
    pthread_mutex_lock(&nodeLock);     
    enqueue(nodeT, socket, username);
    alertStatus(true, newUser);
    currentUsers++;
    pthread_mutex_unlock(&nodeLock);

    // Make new thread
    pthread_create(newUser.thread, NULL, newConnection, &newUser);
    printf("\nUsers: %d\n\n", currentUsers);

}

/**
 * @brief send a online/offline message to all other users
 * 
 * @param isOnline what message to send
 * @param userData user that has exited/entered
 */
void alertStatus(bool isOnline, struct user_t userData) {
    char msg[1024] = {'\0'};
    char *statusMsg = NULL;

    // determine status message
    if(isOnline)  statusMsg = " has entered the chat...";
    else          statusMsg = " has exited the chat...";
    
    // form message
    strncat(msg, userData.username, strlen(userData.username));
    strncat(msg, statusMsg, strlen(statusMsg));

    // broadcast
    broadcast(userData, msg);

}

/**
 * @brief malloc new pthread_t
 * 
 * @return pthread_t* 
 */
pthread_t *allocateThread() {
    return malloc(sizeof(pthread_t));
}
