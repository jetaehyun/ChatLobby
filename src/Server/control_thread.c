#include "header/control_thread.h"
#define SIZE 1024

pthread_t control_thread_ptr;
pthread_mutex_t commLock; 

void *ctrl_thread(void *ptr) {
    struct user_t userData = *(struct user_t *)ptr;
    enum state s = safe;

    // msg stuff
    char *buffer = malloc(sizeof(char) * SIZE);
    size_t bufSize = sizeof(buffer);
    char message[SIZE] = {'\0'};
    char t[12] = {'\0'};

    while(1) {
        
        getline(&buffer, &bufSize, stdin);
        strtok(buffer, "\n");
        
        // check to see if user wants access to admin controls
        if(strncmp(buffer, "/change", 7) == 0) {

            s = setState(&buffer[8]);

            if(s == -1) printf("Requested control does not exist: cast, kick, mute, and safe\n");
            else        printf("----Entering %s state----\n", &buffer[8]);
            
            continue;

        }

        pthread_mutex_lock(&commLock);
        // execute power
        switch(s) {
            case cast:
                timestamp(t);
                sprintf(message, "%sSERVER : %s", t, buffer);
                broadcast(userData, message);
                break;
            case kick:
                printf("2");
                s = -1;
                break;
            case mute:
                printf("3");
                s = -1;
                break;
            case safe:
                // simply observe
                break;
            default:
                printf("Server is entering safe state...\n");
                s = safe;
                break;
        }
        pthread_mutex_unlock(&commLock);
    }
}

/**
 * @brief Set the State object
 * 
 * @param com string of req state
 * @return enum state cats, kick, mute, safe, or -1
 */
enum state setState(char *com) {

    if     (strncmp("cast", com, 5) == 0) return cast;
    else if(strncmp("kick", com, 5) == 0) return kick;
    else if(strncmp("mute", com, 5) == 0) return mute;
    else if(strncmp("safe", com, 5) == 0) return safe;

    return -1;
}


void create_ctrl_thread(node_t **nodeT) {

    // create new user_t obj
    struct user_t newUser;
    newUser.thread = &control_thread_ptr;
    newUser.socket = -1;
    newUser.username = "SERVER";
    newUser.node = nodeT; 
    
    // create thread and pass obj
    pthread_create(&control_thread_ptr, NULL, ctrl_thread, &newUser);
}
