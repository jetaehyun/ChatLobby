#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * When we pass pointer as a parameter in a function and 
 * want update in the same pointer we use double pointer. 
 * On the other hand if we pass pointer as a parameter in a function and 
 * catch it in single pointer then will have to return the result to calling function back
 * in order to use the result.
 */

typedef struct _node_t {
    struct _node_t *next;
    int connection;
    char* username;

} node_t;

bool doesExist(node_t** list, char *username);
bool enqueue(node_t** list, int connection, char *username);
node_t *dequeue(node_t** list, char *username);
void clear(node_t** list);
void printList(node_t** list); // test
int size(node_t** list);


#endif