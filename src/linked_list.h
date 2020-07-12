#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _node_t {
    struct _node_t *next;
    int connection;
    char* username;

} node_t;

bool doesExist(node_t *list, char *username);
bool *enqueue(node_t *list, int connection, char *username);
node_t *dequeue(node_t *list, int connection, char *username);
void clear(node_t *list);
int size(node_t *list);

#endif