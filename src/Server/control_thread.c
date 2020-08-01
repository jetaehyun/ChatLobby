#include "header/control_thread.h"
#define SIZE 1024

pthread_t control_thread_ptr;
pthread_mutex_t ctrlLock; 

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
        if(*(buffer) == '/') {

            s = setState(buffer);

            if(s == -1 || s == cast) continue;
            
        }

        // pthread_mutex_lock(&ctrlLock);
        // execute power
        switch(s) {
            case cast:

                timestamp(t);
                sprintf(message, "%sSERVER : %s", t, buffer);
                broadcast(userData, message);
                break;

            case kick:
                if(*(userData.node) == NULL) {
                    printf("No users...\n");
                    s = safe; // fix duplicate
                    break;
                }

                if(!dequeue(userData.node, &buffer[6])) printf("WARNING: User does not exist...");
                
                s = safe;
                break;

            case list:

                printList(userData.node);
                s = safe;
                break;

            case safe:
                // simply observe
                break;

            default:
                printf("Server is entering safe state...\n");
                s = safe;
                break;
        }
        // pthread_mutex_unlock(&ctrlLock);

    }

    // TODO: add close state
}

/**
 * @brief Set the State object
 * 
 * @param com string of req state
 * @return enum state cats, kick, mute, safe, or -1
 */
enum state setState(char *com) {

    if     (strncmp("/cast", com, 5) == 0) return cast;
    else if(strncmp("/kick", com, 5) == 0) {

        if(strlen(com) < 7) {
            printf("Missing 1 argument --> /kick [username]\n");
            return -1;
        }

        return kick;
    }
    else if(strncmp("/list", com, 5) == 0) return list;
    else if(strncmp("/safe", com, 5) == 0) return safe;

    printf("Requested control does not exist: cast, kick, mute, and safe\n");

    return -1;
}


void create_ctrl_thread(node_t **nodeT) {

    pthread_mutex_init(&ctrlLock, NULL);

    // create new user_t obj
    struct user_t *server = malloc(sizeof(struct user_t));
    server->thread = &control_thread_ptr;
    server->socket = -1;
    server->username = "SERVER";
    server->node = nodeT; 
    
    // create thread and pass obj
    pthread_create(&control_thread_ptr, NULL, ctrl_thread, server);
}
