#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "LinkedList.h"

void print_list(Node * head){
    Node * currentNode = head;
    
    while(currentNode != NULL){
        printf("%d ",currentNode->value);
        currentNode = currentNode->next;
    }
}

void push_end(Node** head, int value){
    Node * newNode;
    newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL; 
    
    Node * currentNode = *head;
    
    if(*head == NULL){
        newNode->prev = NULL;
        *head = newNode;
        return;
    }
    
    while(currentNode->next != NULL){
        currentNode = currentNode->next;
    }
    
    newNode->prev = currentNode;
    currentNode->next = newNode;
}

void push_front(Node ** head, int value){
    Node * newNode = (Node *) malloc(sizeof(Node));
    newNode->prev = NULL;
    newNode->next = *head;
    newNode->value = value;
    
    if((*head) != NULL){
        (*head)->prev = newNode;
    }
    
    *head = newNode;
}

void insert(Node * node, int value){
    
    if(node == NULL){
        printf("NULL element!\n");
        return;
    }
    
    Node* newNode =  (Node *) malloc(sizeof(Node));
    newNode->next = node->next;
    newNode->prev = node;
    newNode->value = value;
    
    node->next = newNode;
    
    if(newNode->next != NULL){
        newNode->next->prev = newNode;
    }
}

void pop(Node ** head){
    if(*head == NULL){
        printf("List is empty!\n");
        return;
    }
    
    Node * newHead = (*head)->next;
    newHead->prev = NULL;
    free(*head);
    *head = newHead;
    
}

void remove_last(Node * head){
    if(head == NULL){
        printf("List is empty!\n");
        return;
    }
    
    Node * currentNode = head;
    while(currentNode->next->next != NULL){
        currentNode = currentNode->next;
    }
    
    free(currentNode->next);
    currentNode->next = NULL;
    
}

void remove_by_index(Node ** head, int n){
    if(n == 0){
        return pop(head);
    }
    
    Node * currentNode = *head;
    Node * temporaryNode = NULL;
    
    for(int i = 0; i < n-1; i++){
        /** If there in no such element. */
        if(currentNode->next == NULL){
            return;
        }
        
        currentNode = currentNode->next;
    }
    
    temporaryNode = currentNode->next;
    currentNode->next = temporaryNode->next;
    temporaryNode->next->prev = currentNode;
    free(temporaryNode);
}

