#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h>
#include <signal.h>
#include "Client/header/read_thread.h"

/**
 * @brief shorturl.at/dsN48
 * 
 */
void sigint_handler(int signum) { //Handler for SIGINT
   //Reset handler to catch SIGINT next time.
   signal(SIGINT, sigint_handler);
   printf("Cannot be stopped using Ctrl+C \n");
   fflush(stdout);
}

char* username() {
    char* name = malloc(sizeof(char) * 50);
    size_t s = sizeof(name);
    printf("Enter your name: ");
    getline(&name, &s, stdin);
    printf("Entering lobby as: %s\n", name);
    return name;
}

int main(int argc, const char *argv[]) {
    signal(SIGINT, sigint_handler);
    int sock = 0; 
    struct sockaddr_in serv_addr; 
    bool auth = true;

    sock = socket(AF_INET, SOCK_STREAM, 0);
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(8800);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.0.156"); 

    char *name = username();

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("Connection Failed \n"); 
        return -1; 
    } 

    send(sock, name, sizeof(name), 0);

    char *message = malloc(sizeof(char) * 1024);
    size_t msgSize = sizeof(message);
    pthread_t *thr = NULL;

    while(true) {
        recv(sock, message, msgSize, 0);
        if(strcmp(message, "deny") == 0) {
            printf("User already exists, try again...\n");
            auth = false;
            break;
        } else if(strcmp(message, "accept") == 0) {
            thr = allocateThread();
            create_thread(sock, thr);
            break;
        }

    }
    
    fflush(stdout);
    
    while(auth) {
        // for now
        if(!feof(stdin)) {
            getline(&message, &msgSize, stdin);
            if(strcmp(message, "exit()\n") == 0) break;
            send(sock, message, 1024, 0);
        }

    }

    printf("\nEXITED CHAT\n");
    free(name);
    free(thr);
    free(message);
    close(sock);

    return 0; 
    
}