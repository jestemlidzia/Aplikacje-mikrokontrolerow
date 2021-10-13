#include <stdio.h>
#include "LinkedList.h"

int main()
{
    Node * head = NULL;
    head = (Node *) malloc(sizeof(Node));
    if (head == NULL) {
        return 1;
    }
    
    head->value = 1;
    //head->next = NULL;

    head->next = (Node *) malloc(sizeof(Node));
    head->next->value = 2;
    head->next->next = NULL;
    
   //printf("First element: %d\n", head->value);
    //printf("Second element: %d", head->next->value);

    print_list(head);
    printf("\n");
    
    push_end(head, 3);
    push_front(&head, 0);
    print_list(head);
    printf("\n");

    pop(&head);
    remove_last(head);
    print_list(head);
    printf("\n");
    
    push_end(head, 3);
    push_end(head, 4);
    push_end(head, 5);
    push_end(head, 6);
    push_end(head, 7);
    print_list(head);
    printf("\n");


    remove_by_index(&head, 5);
    print_list(head);
    printf("\n");
    return 0;
}
