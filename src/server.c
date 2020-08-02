#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include "Server/header/connection_thread.h"
#include "Server/header/control_thread.h"
#include "Server/header/locks.h"
#include "linked_list.h"

pthread_mutex_t statsLock; 

void initializeLocks() {

    pthread_mutex_init(&statsLock, NULL);

}

int main(int argc, const char *argv[]) {

    node_t *node = NULL;
    initializeLocks();

    int server_fd, opt;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    printf("Creating socket...\n");
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Setting socket options..\n");

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    if(argv[1] == NULL) address.sin_addr.s_addr = inet_addr("192.168.0.156");
    else                address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(8800);

    printf("Binding socket to port 8800\n");
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    printf("Awaiting connections...\n");
    listen(server_fd, 10);

    create_ctrl_thread(&node);
    char con[1024] = {'\0'};

    while(true) {
        int connection = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if(read(connection, con, 1024) <= 0) continue;

        strtok(con, "\n");

        char *username = malloc(strlen(con) + 1);
        strncpy(username, con, strlen(con) + 1);
        username[strlen(con)] = '\0';

        // simple auth check (compare plaintext) to see if that username exists in the lobby
        pthread_mutex_lock(&statsLock);     
        if(doesExist(&node, username)) {

            send(connection, "deny", 5, 0);
            close(connection);

        } else {

            send(connection, "accept", 7, 0);
            printf("Authorizing: %s\n", username);
            create_thread(connection, username, &node);  

        }
        pthread_mutex_unlock(&statsLock);     

    }

    return 0;    
} 