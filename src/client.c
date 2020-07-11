#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdbool.h>

int main(int argc, const char *argv[]) {
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 

    printf("Making socket...\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(8800); 
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 


    printf("Connected...\n"); 
    char things[1024] = {0};

    while(true) {
        scanf("%s", things);
        send(sock, things, 1024, 0);
    }

    return 0; 
    
}