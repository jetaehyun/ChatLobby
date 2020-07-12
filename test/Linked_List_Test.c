#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/linked_list.h"

int main(int argc, const char* argv[]) {
    puts("LINKED LIST TESTING...\n\n");

    node_t *node = NULL;
    enqueue(&node, 1, "Ted");
    enqueue(&node, 2, "Too");
    enqueue(&node, 3, "Two");

    puts("--------------ENQUEUE----------------------");
    printf("ENQUEUE 1: %d\n", node->connection);
    printf("ENQUEUE 2: %d\n", node->connection);
    printf("ENQUEUE 2: %d\n", node->connection);
    puts("-------------------------------------------\n");

    puts("--------------PRINTLIST----------------------");
    printList(&node);
    puts("---------------------------------------------\n");    

    puts("-----------------SIZE----------------------");
    printf("SIZE: %d\n", size(&node));
    puts("-------------------------------------------\n");

    puts("-----------------DEQUEUE-------------------");
    dequeue(&node, "Too");
    printList(&node);
    dequeue(&node, "Ted");
    puts("");
    printList(&node);
    puts("-------------------------------------------\n");    

    return 0;
}