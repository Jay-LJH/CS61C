#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* TODO: Implement ll_has_cycle */
    node* q=head,*s=head;
    while(q){
        q=q->next;
        if(q){
            q=q->next;
        }
        s=s->next;
        if(q==s)
            return 1;
    }
    return 0;
}
