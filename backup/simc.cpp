/*
 * File: sim.cpp
 * Author: Jonathan Humphrey
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>

#include "header.h"

using namespace std;
////////////////////////////////////////////////////////////////
// sample events
#define ARRIVE 1
#define EVENT2 2
// .. add more events
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// function definition
void init();
int run_sim();
void generate_report();
int schedule_event(struct event *);
int process_event1(struct event *eve);
int process_event2(struct event *eve);
////////////////////////////////////////////////////////////////
// Global variables
struct event *head; // head of event queue

////////////////////////////////////////////////////////////////
void init()
{
    std::cout << "Select Your Scheduler: " << std::endl;
    std::cout << "1: FCFS" << std::endl;
    std::cout << "2: SRTF" << std::endl;
    std::cout << "3: HRRN" << std::endl;
    std::cout << "4: RR" << std::endl;

    std::cin >> scheduler;

    std::cout << "Enter the average arrival rate of processes: ";
    std::cin >> lambda;

    std::cout << "Enter the average service time: ";
    std::cin >> serviceTime;

    if (scheduler == 4)
    {
        std::cout << "Enter the quantum interval for the Round Robin Scheduler: ";
        std::cin >> timeSlice;
    }

    cpu = new CPU;
    cpu->clock = 0.0;
    cpu->busy = false;

    process *p = new process;
    p->pid = 0;
    p->arrival = genexp((float)lambda);
    p->start = 0.0;
    p->reStart = 0.0;
    p->finish = 0.0;
    p->burst = genexp(avgServeTime);
    p->remaining = p->burst;
    // maybe need pointer to next process??

    event *e = new event;
    e->time = p->arrival;
    e->type = ARRIVE;
    e->next = nullptr;
    e->pLink = p;

    eventQ.push_back(*e);
    head = e;
}
////////////////////////////////////////////////////////////////
void generate_report()
{
    // output statistics
}
////////////////////////////////////////////////////////////////
// schedules an event in the future
/* int schedule_event(struct event *new)
{
    // insert event in the event queue in its order of time
} */
////////////////////////////////////////////////////////////////
// returns a random number between 0 and 1
float urand()
{
    return ((float)rand() / RAND_MAX);
}
/////////////////////////////////////////////////////////////
// returns a random number that follows an exp distribution
float genexp(float lambda)
{
    float u, x;
    x = 0;
    while (x == 0)
    {
        u = urand();
        x = (-1 / lambda) * log(u);
    }
    return (x);
}
////////////////////////////////////////////////////////////
int run_sim()
{
    int departureTotal = 0;

    while (departureTotal < MAX_PROCESSES)
    {

        eve = head;
        cout << eve;
        // cpu->clock = eve->time;
        // cout << cpu->clock;
        switch (eve->type)
        {
        case ARRIVE:
            // process_event1(eve);

            break;
        case EVENT2:
            // process_event2(eve);
            break;
        // add more events
        default:
            break;
        }
        head = eve->next;
        free(eve);
        eve = NULL;
    }
    return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    // parse arguments
    init();
    // cout << eventQ[0].pLink->pid;
    run_sim();
    // generate_report();
    return 0;
}