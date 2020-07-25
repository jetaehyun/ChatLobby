#include "header/communication.h"
#define SIZE 1024

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
        
        // only send if it's not the sender
        if(strcmp(node->username, userData.username) != 0) 
            send(node->connection, message, SIZE, 0);
        
        node = node->next;
    }

}

/**
 * @brief send a online/offline message to all other users
 * 
 * @param isOnline what message to send
 * @param wasKicked was the user kicked
 * @param userData user that has exited/entered
 */
void alertStatus(bool isOnline, bool wasKicked, struct user_t userData) {
    char msg[SIZE] = {'\0'};
    char *statusMsg = NULL;

    // determine status message
    if     (isOnline)  statusMsg = " has entered the chat...";
    else if(wasKicked) statusMsg = " has been kicked...";
    else               statusMsg = " has exited the chat...";
    
    // form message
    strncat(msg, userData.username, strlen(userData.username));
    strncat(msg, statusMsg, strlen(statusMsg));

    // broadcast
    broadcast(userData, msg);

}

