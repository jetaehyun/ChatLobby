#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h>
#include <signal.h>

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
    printf("\nEntering lobby as: %s\n", name);
    return name;
}

int main(int argc, const char *argv[]) {
    signal(SIGINT, sigint_handler);
    int sock = 0; 
    struct sockaddr_in serv_addr; 

    printf("Making socket...\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(8800); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 

   char *name = username();

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("Connection Failed \n"); 
        return -1; 
    } 

    printf("Connected...\n"); 
    char things[1024] = {0};

    while(true) {
        // for now
        if(!feof(stdin)) {
            scanf("%s", things);
            if(strcmp(things, "exit()") == 0) break;
            send(sock, things, 1024, 0);
        }

    }

    printf("\nEXITED CHAT\n");
    free(name);
    close(sock);

    return 0; 
    
}