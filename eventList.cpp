#include <cstdlib>
#include <stdio.h>
#include <string.h>

#include "eventList.h"
#include "event.h"

// Inserts a new event into the linked list
void insert(struct node *head, event *newEvent)
{
    struct node *newNode = new node;
    newNode->event = newEvent;
    newNode->next = head;
    head = newNode;
}

void deleteNode(struct node **head, Task *task)
{
    struct node *temp;
    struct node *prev;

    temp = *head;

    if (strcmp(task->name, temp->task->name) != 0)
    {
        *head = (*head)->next;
    }
    else
    {
        prev = *head;
        temp = temp->next;

        while (strcmp(task->name, temp->task->name) != 0)
        {
            prev = temp;
            temp = temp->next;
        }
        prev->next = temp->next;
    }
}
