#include "header/connection_thread.h"
#define SIZE 1024

// FIXME: mutex locks break the code

int currentUsers = 0;
// pthread_mutex_t statsLock = PTHREAD_MUTEX_INITIALIZER; 
// pthread_mutex_t freeLock = PTHREAD_MUTEX_INITIALIZER;
// pthread_mutex_t commLock = PTHREAD_MUTEX_INITIALIZER; 

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

        // Check if client is still there
        if((data = read(userData.socket, buffer, SIZE)) <= 0) break;

        // Check if user was kicked
        if(isKicked(userData.node, userData.username)) {
            wasKicked = true;
            break;
        }

        // General msg
        if(!block) {

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

            printf("%s\n", message);

            // send message to open sockets
            // pthread_mutex_lock(&commLock);
            broadcast(userData, message);
            // pthread_mutex_unlock(&commLock);

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
}

/**
 * @brief close/free 
 * 
 * @param userData user that has exited
 */
void closeIO(struct user_t userData, bool wasKicked) {

    // adjust stats
    pthread_mutex_lock(&freeLock); 
    currentUsers--;

    if(wasKicked) {

        alertStatus(false, true, userData);
        printf("MESSAGE: Kicked user %s...\n", userData.username);

    } else {

        alertStatus(false, false, userData);  
        dequeue(userData.node, userData.username);
        printf("%s disconnected\n", userData.username);

    }

    pthread_mutex_unlock(&freeLock);
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

    // pthread_mutex_init(&statsLock, NULL);
    
    // create new user_t obj
    struct user_t *newUser = malloc(sizeof(struct user_t));
    newUser->thread = allocateThread();
    newUser->socket = socket;
    newUser->username = username;
    newUser->node = nodeT;    
    
    // Add new user to list
    // pthread_mutex_lock(&statsLock);     
    enqueue(nodeT, socket, username);
    alertStatus(true, false, *newUser);
    currentUsers++;
    // pthread_mutex_unlock(&statsLock);

    // Make new thread
    pthread_create(newUser->thread, NULL, newConnection, newUser);
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
    // pthread_mutex_lock(&statsLock);

    if(!doesExist(node, username)) {
        return true;
    }

    // pthread_mutex_unlock(&statsLock);     

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
