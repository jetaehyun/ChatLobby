#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node_t {
    struct _node_t *next;
    int connection;
    char* username;

} node_t;

bool doesExist(node_t** list, char *username);
bool enqueue(node_t** list, int connection, char *username);
bool dequeue(node_t** list, char *username);
void clear(node_t** list);
void printList(node_t** list); // test
int size(node_t** list);


#endif