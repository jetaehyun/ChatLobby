#include "linked_list.h"

/*
    Linked List implementation for server messages only.
    Could implement void type for modular use but not necessary
    for this project.
*/


bool enqueue(node_t** list, int connection, char *username) {

    if(doesExist(list, username)) return false;

    node_t *node = malloc(sizeof(node_t));
    node->username = username;
    node->connection = connection;

    if(list == NULL) node->next = NULL;
    else             node->next = *list;

    *list = node;
    return true;
}

node_t *dequeue(node_t** list, char *username) {
    if(*list == NULL) return NULL;

    node_t *data = NULL;

    while(*(list) != NULL) {
        if(strcmp((*list)->username, username) == 0) {
            data = (*list);
            (*list) = data->next;
            return data;
        }
        *(list) = (*list)->next;
    }

    return NULL;

}

bool doesExist(node_t **list, char *username) {
    if(list == NULL) return false;

    node_t *node = *list;

    while(node != NULL) {
        if(strcmp(node->username, username) == 0) return true;
        node = node->next;
    }

    return false;
}

void clear(node_t** list) {
    if(*list == NULL) return;

    node_t *node = *list;

    while(node != NULL) {
        *list = (*list)->next;
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