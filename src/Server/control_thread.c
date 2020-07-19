#include "header/control_thread.h"
#define SIZE 1024

pthread_t control_thread_ptr;


void *ctrl_thread(void *ptr) {
    // int data = 0;
    char *buffer = malloc(sizeof(char) * SIZE);
    size_t bufSize = sizeof(buffer);
    enum state s = safe;

    while(1) {
        getline(&buffer, &bufSize, stdin);
        strtok(buffer, "\n");
        
        if(strncmp(buffer, "/change", 7) == 0) {
            s = setState(&buffer[8]);

            if(s == -1) {

                printf("State does not exist\n");
                printf("States: safe, cast, kick, mute\n");

            } else printf("----Entering %s state----\n", &buffer[8]);
            
            continue;
        }

        switch(s) {
            case cast:
                printf("1");
                s = -1;
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
                printf("4");
                s = -1;
                break;
            default:
                printf("Server is entering safe state...\n");
                s = safe;
                break;
        }
    }
}

enum state setState(char *com) {
    if(strncmp("cast", com, 5) == 0) {
        return cast;
    } else if(strncmp("kick", com, 5) == 0) {
        return kick;
    } else if(strncmp("mute", com, 5) == 0) {
        return mute;
    } else if(strncmp("safe", com, 5) == 0) {
        return safe;
    }

    return -1;
}


void create_ctrl_thread() {
    pthread_create(&control_thread_ptr, NULL, ctrl_thread, NULL);
}
