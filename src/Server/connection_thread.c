#include "header/connection_thread.h"
#define SIZE 1024

int currentUsers = 0;
pthread_mutex_t exitLock; 
pthread_mutex_t nodeLock; 
pthread_mutex_t commLock; 

void *newConnection(void *ptr) {
    int data = 0;
    struct user_t userData = *(struct user_t *)ptr;

    // Spam prevention
    long long timeRef = getTime();
    long long current_time = 0;
    int messagesSent = 0;
    bool block = false;

    // buffers for message transmission
    char buffer[SIZE] = {'\0'};
    char message[SIZE] = {'\0'};
    char t[12] = {'\0'};

    while(true) {

        data = recv(userData.socket, buffer, SIZE, 0);

        if(data < 0) break;

        if(data > 0 && currentUsers > 1 && !block) {

            // Keeps track of the number of messages received in the last 2 seconds...
            current_time = getTime();
            if(current_time - timeRef < 2 && messagesSent > 10) {

                block = true;
                timeRef = current_time;
                
                printf("%s has been blocked...\n", userData.username);
                send(userData.socket, "Stop spamming the server...You've been block for 5 seconds", 1024, 0);
                continue;

            } else if(current_time - timeRef >= 2) {
                timeRef = current_time;
                messagesSent = 0;
            }
            messagesSent++;

            // Form Message - [xx:xx:xx] username: msg
            timestamp(t);
            strncat(message, t, strlen(t));
            strncat(message, userData.username, strlen(userData.username));
            strncat(message, ": ", 3);
            strncat(message, buffer, SIZE - strlen(message));  

            // send message to open sockets
            pthread_mutex_lock(&commLock);
            broadcast(userData, message);
            pthread_mutex_unlock(&commLock);

            memset(message, 0, sizeof(message));
            memset(t, 0, sizeof(t));

        } else if(block) {
            current_time = getTime();
            if(current_time - timeRef >= 5) {
                block = false;
                timeRef = current_time;
                messagesSent = 0;
            }
        }

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
            send(node->connection, message, SIZE, 0);
        
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
    char msg[SIZE] = {'\0'};
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

long long getTime() {
    time_t t;

    t = time(NULL);
    
    return t;
}