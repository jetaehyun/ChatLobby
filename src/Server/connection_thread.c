#include "header/connection_thread.h"
#define SIZE 1024

int currentUsers = 0;
pthread_mutex_t exitLock; 
pthread_mutex_t nodeLock; 
pthread_mutex_t commLock; 

void *newConnection(void *ptr) {
    int data = 0;
    bool wasKicked = false;
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
        
        // Check if user was kicked
        if(isKicked(userData.node, userData.username)) {
            wasKicked = true;
            break;
        }

        data = recv(userData.socket, buffer, SIZE, 0);

        // No longer receiving data from client
        if(data <= 0) break;

        // General msg
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
            
            // check to unblock
            current_time = getTime();

            if(current_time - timeRef >= 5) {

                block = false;
                timeRef = current_time;
                messagesSent = 0;

            }

        }

        usleep(500000);         
    }
    
    closeIO(userData, wasKicked);
    pthread_exit(NULL);
}

/**
 * @brief close/free 
 * 
 * @param userData user that has exited
 */
void closeIO(struct user_t userData, bool wasKicked) {
    // adjust stats
    pthread_mutex_lock(&exitLock); 
    currentUsers--;

    if(wasKicked) {

        alertStatus(false, true, userData);
        printf("Kicked %s...\n", userData.username);

    } else {

        alertStatus(false, false, userData);  
        free(dequeue(userData.node, userData.username));
        printf("%s disconnected\n", userData.username);

    }

    pthread_mutex_unlock(&exitLock);

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
    alertStatus(true, false, newUser);
    currentUsers++;
    pthread_mutex_unlock(&nodeLock);

    // Make new thread
    pthread_create(newUser.thread, NULL, newConnection, &newUser);
    printf("\nUsers: %d\n\n", currentUsers);

}

/**
 * @brief 
 * 
 * @param node 
 * @param username 
 * @return true 
 * @return false 
 */
bool isKicked(node_t **node, char *username) {

    if(!doesExist(node, username)) {
        return true;
    }

    return false;
}

/**
 * @brief malloc new pthread_t
 * 
 * @return pthread_t* 
 */
pthread_t *allocateThread() {
    return malloc(sizeof(pthread_t));
}
