#ifndef LINKED_H_
#define LINKED_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "LinkedList.h"

/** Structure describing the node of linked list. */
typedef struct node{
    int value;
    struct node * next;
    struct node * prev;
} Node;

/** Iterating over a list. */
void print_list(Node * head);

/** Adding an item to the end of the list. */
void push_end(Node ** head, int value);

/** Insert an item after a given node. */
void insert(Node * node, int value);

/** Adding an item to the beginning of the list (pushing to the list). */
void push_front(Node ** head, int value);

/** Removing the first item (popping from the list). */
void pop(Node ** head);

/** Removing the last item of the list. */
void remove_last(Node * head);

/** Removing a specific item. */
void remove_by_index(Node ** head, int n);

#endif /* LINKED_H_ */