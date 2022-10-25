#ifndef EVENT_H
#define EVENT_h
// Setup
enum EventType
{
    ARRIVE = 1,
    DISPATCH = 2,
    DEPART = 3,
    PREEMPT = 4
};

struct event
{
    float time;
    EventType type;
    struct event *eq_next;
    struct process *p_link;
};

class EventQueue
{
public:
    EventQueue() { eq_head = eq_tail = nullptr; }
    event *top();
    void pop();
    void push(event *);

private:
    event *eq_head, *eq_tail;
};

// Function Delclarations
event *EventQueue::top() { return eq_head; }
void EventQueue::pop()
{
    event *tempPtr = eq_head;
    eq_head = eq_head->eq_next;
    delete tempPtr;
}
void EventQueue::push(event *newEvent)
{
    // Empty Case
    if (eq_head == nullptr)
    {
        eq_head = newEvent;
    }
    else if (eq_head->time > newEvent->time)
    {
        newEvent->eq_next = eq_head;
        eq_head = newEvent;
    }
    else
    {
        event *eq_marker = eq_head;
        while (eq_marker != nullptr)
        {
            if ((eq_marker->time < newEvent->time) && (eq_marker->eq_next == nullptr))
            {
                eq_marker->eq_next = newEvent;
                break;
            }
            else if ((eq_marker->time < newEvent->time) && (eq_marker->eq_next->time > newEvent->time))
            {
                newEvent->eq_next = eq_marker->eq_next;
                eq_marker->eq_next = newEvent;
                break;
            }
            else
            {
                eq_marker = eq_marker->eq_next;
            }
        }
    }

    struct eventCompareFunc
    {
        bool operator()(const event *left, const event *right) const
        {
            return left->time > right->time;
        }
    };
#endif