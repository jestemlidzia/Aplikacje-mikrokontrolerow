#include <stdio.h>
#include "LinkedList.h"

int main()
{
    Node* head = NULL;
    
    push_end(&head,1);
    push_end(&head,2);
    push_end(&head,8);
    print_list(head);
    printf("\n");
    
    push_front(&head,0);
    print_list(head);
    printf("\n");
     
    insert(head->next, 7);
    print_list(head);
    printf("\n");
    
    pop(&head);
    print_list(head);
    printf("\n");
    
    remove_last(head);
    print_list(head);
    printf("\n");
    
    push_end(&head,6);
    push_end(&head,8);
    print_list(head);
    printf("\n");

    remove_by_index(&head,2);
    print_list(head);
    printf("\n");
    return 0;
}
