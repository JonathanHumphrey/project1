// Replacing list.h from a previous project and adapting it for this

#include "event.h"

#ifndef EVENTLIST_H
#define EVENTLIST_H

struct node
{
    event *event;
    struct node *next;
};

void insert(struct node **head, event *event);
void deleteNode(struct node **head, event *event);
void traverse(struct node *head);

#endif
