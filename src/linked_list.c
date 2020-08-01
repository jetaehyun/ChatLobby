#include "linked_list.h"

/*
    Linked List implementation for server messages only.
    Could implement void type for modular use but not necessary
    for this project.
*/


bool enqueue(node_t** list, int connection, char *username) {

    if(doesExist(list, username)) return false;

    // create node
    node_t *node = malloc(sizeof(node_t));
    node->username = malloc(strlen(username) + 1);
    strncpy(node->username, username, strlen(username) + 1);
    node->username[strlen(username)] = '\0';
    node->connection = connection;
    node->next = *list;

    // set node at head
    *list = node;

    return true;
}

bool dequeue(node_t** list, char *username) {

    if(*list == NULL) return false;

    node_t *data = *list;
    node_t *prev = *list;

    if(data != NULL && strncmp(data->username, username, strlen(username)) == 0) {
        *list = data->next;
        free(data->username);
        free(data);
        return true;
    }
    
    while(data != NULL) {

        if(strncmp(data->username, username, strlen(username)) == 0) {
            prev->next = data->next;
            free(data->username);        
            free(data);
            return true;
        }
        
        prev = data;
        data = data->next;
    }

    return false;

}

bool doesExist(node_t **list, char *username) {
    if(*list == NULL) return false;

    node_t *node = *list;

    while(node != NULL) {
        if(strncmp(node->username, username, strlen(username)) == 0) return true;
        node = node->next;
    }

    return false;
}

void clear(node_t** list) {
    if(*list == NULL) return;

    node_t *node = *list;

    while(node != NULL) {
        *list = (*list)->next;
        free(node->username);
        free(node);
        node = *list;
    }
}

int size(node_t** list) {
    if(*list == NULL) return 0;

    node_t *node = *list;
    int counter = 0;

    while(node != NULL) {
        node = node->next;
        counter++;
    }

    return counter;
}

void printList(node_t** list) {
    if(*list == NULL) {
        printf("Linked list is empty\n");
        return;
    }

    node_t *node = *list;

    while(node != NULL) {
        printf("username: %s, Connection: %d\n", node->username, node->connection);
        node = node->next;
    }
}