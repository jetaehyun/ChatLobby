#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdbool.h>
#include "Server/header/user_thread.h"

int main(int argc, const char *argv[]) {

    int server_fd, opt;
    struct sockaddr_in address;
    int addrlen = sizeof(address);     

    printf("Creating socket...\n");
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Setting socket options..\n");
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8800);

    printf("Binding socket to port 8800\n");
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    printf("Awaiting connections...\n");
    listen(server_fd, 10);

    while(true) {
        int connection = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        newThread(connection);    
    }

    return 0;    
} 