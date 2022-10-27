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
#include <string>
#include <algorithm>

#include "header.h"

using namespace std;
////////////////////////////////////////////////////////////////
// sample events
#define ARRIVE 1
#define DISPATCH 2
#define DEPART 3
#define PREEMPT 4
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
    cout << "lambda: " << lambda << endl;
    qClock = 0.0;
    avgServeTime = (float)1.0 / avgServeTime;

    cpu = new CPU;
    cpu->clock = 0.0;
    cpu->busy = false;
    cpu->pLinkCpu = nullptr;

    process *p = new process;
    p->pid = 0;
    p->arrival = genexp((float)lambda);
    p->start = 0.0;
    p->reStart = 0.0;
    p->finish = 0.0;
    p->burst = genexp(avgServeTime);
    p->remaining = p->burst;
    p->nextProc = nullptr;

    processTail = p;

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
    float avgTurnAround = turnaroundTime / (float)MAX_PROCESSES;
    float totalThroughput = (float)MAX_PROCESSES / completeTime;
    float cpuUtil = busyTime / completeTime;
    float avgWait = waitingTime / (float)MAX_PROCESSES;
    float avgReadyQLength = readyQCount / (float)MAX_PROCESSES;
    string _scheduler;

    switch (scheduler)
    {
    case 1:
        _scheduler = "FCFS";
        break;
    case 2:
        _scheduler = "SRTF";
        break;
    case 3:
        _scheduler = "HRRN";
        break;
    case 4:
        _scheduler = "RR";
        break;
    }

    ofstream data("sim.data", ios::out | ios::app);
    ofstream xcel("sim.csv", ios::out | ios::app);

    if (data.is_open() && xcel.is_open())
    {
        int width = 20;
        if (lambda == 1)
        {
            data << setfill('-') << setw(width * 5) << "\n"
                 << setfill(' ')
                 << setw(width / 2) << "Scheduler"
                 << setw(width / 2) << "lambda"
                 << setw(width) << "AvgTurnaround"
                 << setw(width) << "Throughput"
                 << setw(width) << "CPU Utilization"
                 << setw(width) << "AvgReadyQ\n"
                 << setfill('-') << setw(width * 5) << "\n";

            cout << setfill('-') << setw(width * 5) << "\n"
                 << setfill(' ')
                 << setw(width / 2) << "Scheduler"
                 << setw(width / 2) << "lambda"
                 << setw(width) << "AvgTurnaround"
                 << setw(width) << "Throughput"
                 << setw(width) << "CPU Utilization"
                 << setw(width) << "AvgReadyQ\n"
                 << setfill('-') << setw(width * 5) << "\n";

            xcel << "Scheduler,lambda,AvgTurnaround,Throughput,CPU Utilization,AvgReadyQ\n";
        }
        data << setfill(' ')
             << setw(width / 2) << _scheduler
             << setw(width / 2) << lambda
             << setw(width) << avgTurnAround
             << setw(width) << totalThroughput
             << setw(width) << cpuUtil
             << setw(width) << avgReadyQLength << endl;

        xcel << _scheduler << ","
             << lambda << ","
             << avgTurnAround << ","
             << totalThroughput << ","
             << cpuUtil << ","
             << avgReadyQLength << ","
             << endl;

        data.close();
    }
    else
        cout << "Unable to open file";
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

    eve = head;
    switch (scheduler)
    {
    case 1:
        cout << "FCFS\n";
        FCFS();
        break;
    case 2:
        cout << "SRTF\n";
        SRTF();
        break;
    case 3:
        cout << "HRRN\n";
        HRRN();
        break;
    }

    head = eve->next;
    free(eve);
    eve = NULL;

    return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    // parse arguments
    scheduler = stoi(argv[1]);
    lambda = atoi(argv[2]);
    avgServeTime = (float)atof(argv[3]);

    if (argc == 5)
    {
        quantum = (float)atof(argv[4]);
    }
    init();
    run_sim();
    generate_report();
    return 0;
}
///////////////////////////////////////////////////////////////
// Scheduling Functions

/*
    Creates new process and schedules the arrival in the event queue
*/
void scheduleArrival()
{
    process *cursor = processTail;
    cursor->nextProc = new process;
    cursor->nextProc->pid = cursor->pid + 1;
    cursor->nextProc->arrival = cursor->arrival + genexp((float)lambda);
    cursor->nextProc->start = 0.0;
    cursor->nextProc->reStart = 0.0;
    cursor->nextProc->finish = 0.0;
    cursor->nextProc->burst = genexp(avgServeTime);
    cursor->nextProc->remaining = cursor->nextProc->burst;
    cursor->nextProc->nextProc = nullptr;

    processTail = processTail->nextProc;

    event *arrival = new event;
    arrival->type = ARRIVE;
    arrival->time = cursor->nextProc->arrival;
    arrival->pLink = cursor->nextProc;
    arrival->next = nullptr;

    eventQ.push_back(*arrival);

    // cout << "Arrival Scheduled for: " << eventQ.back().pLink->pid << endl;
}
void arrival()
{
    readyProc ready;
    ready.pLink = eventQ.front().pLink;
    ready.next = nullptr;

    readyQ.push_back(ready);
    eventQ.pop_front();

    // cout << "Arrival For: " << readyQ.back().pLink->pid << endl;
}

void scheduleDispatch()
{
    event *dispatch = new event;
    process *nextProc;
    bool exists = false;
    float shortest;

    // FCFS
    if (scheduler == 1)
    {
        nextProc = readyQ.front().pLink;
    }
    else if (scheduler == 2)
    {
        if (cpu->clock > readyQ.front().pLink->arrival)
        {
            // Checking to see what process has the least amount of time left
            for (int i = 0; i < readyQ.size(); i++)
            {
                shortest = readyQ.front().pLink->remaining;
                cout << shortest << endl;
                if (readyQ[i].pLink->remaining < shortest)
                {
                    nextProc = readyQ[i].pLink;
                    cout << "Shortest Process: " << nextProc->pid << " with time: " << nextProc->remaining << endl
                         << endl;
                }
            }
        }
        else
        {
            nextProc = readyQ.front().pLink;
        }
    }
    else if (scheduler = 3)
    {
        int nextProcID = getHRRNProcess();
        for (int i = 0; i < readyQ.size(); i++)
        {
            if (readyQ[i].pLink->pid == nextProcID)
            {
                nextProc = readyQ[i].pLink;
            }
        }
    }

    if (cpu->clock < nextProc->arrival)
    {
        dispatch->time = nextProc->arrival;
    }
    else
    {
        dispatch->time = cpu->clock;
    }

    dispatch->type = DISPATCH;
    dispatch->next = nullptr;
    dispatch->pLink = nextProc;
    for (int i = 0; i < eventQ.size(); i++)
    {
        if (dispatch->pLink->pid == eventQ[i].pLink->pid)
        {
            exists = true;
        }
    }
    if (!exists)
    {

        eventQ.push_back(*dispatch);
        // cout << "Dispatch Scheduled for: " << dispatch->pLink->pid << endl;
    }
}

void dispatch()
{
    // cout << "Dispatching: " << eventQ.front().pLink->pid << endl;

    cpu->pLinkCpu = eventQ.front().pLink;

    // SRTF = 2 RR = 4
    if (scheduler == 2 || scheduler == 4)
    {
        readyProc second = readyQ[1];
        readyProc first = readyQ[0];
        if (first.pLink->arrival != eventQ.front().pLink->arrival)
        {
            for (int i = 0; i < readyQ.size(); i++)
            {
                if (second.pLink->arrival == eventQ.front().pLink->arrival)
                {
                    readyQ[i + 1] = readyQ[i + 2];
                    readyQ[i + 1] = readyQ.front();
                    readyQ.front() = readyQ[i + 1];
                    break;
                }
            }
        }
    }

    readyQ.pop_front();
    eventQ.pop_front();

    cpu->busy = true;

    if (cpu->clock < cpu->pLinkCpu->arrival)
    {
        cpu->clock = cpu->pLinkCpu->arrival;
    }

    if (cpu->pLinkCpu->start == 0)
    {
        cpu->pLinkCpu->start = cpu->clock;
    }
    else
    {
        cpu->pLinkCpu->reStart = cpu->clock;
    }
}

void scheduleDeparture()
{
    bool exists = false;

    event *departure = new event;
    departure->type = DEPART;
    departure->next = nullptr;
    departure->pLink = cpu->pLinkCpu;

    // FCFS = 1 & RR = 4
    if (scheduler == 1 || scheduler == 4)
    {
        departure->time = cpu->pLinkCpu->start + cpu->pLinkCpu->remaining;
    }
    // SRTF
    else if (scheduler == 2)
    {
        if (cpu->pLinkCpu->reStart == 0)
        {
            departure->time = cpu->pLinkCpu->start + cpu->pLinkCpu->remaining;
        }
        else
        {
            departure->time = cpu->pLinkCpu->reStart + cpu->pLinkCpu->remaining;
        }
    }

    for (int i = 0; i < eventQ.size(); i++)
    {
        if (departure->pLink->pid == eventQ[i].pLink->pid)
        {
            exists = true;
        }
    }
    if (!exists)
    {

        eventQ.push_back(*departure);
    }
}

void departure()
{

    cpu->clock = eventQ.front().time;

    cpu->pLinkCpu->finish = cpu->clock;
    cpu->pLinkCpu->remaining = 0.0;

    // Tracking metrics
    busyTime += cpu->pLinkCpu->burst;
    turnaroundTime += (cpu->pLinkCpu->finish - cpu->pLinkCpu->arrival);
    completeTime = cpu->pLinkCpu->finish;
    waitingTime += (cpu->pLinkCpu->finish - cpu->pLinkCpu->arrival - cpu->pLinkCpu->burst);

    cpu->pLinkCpu = nullptr;
    cpu->busy = false;

    eventQ.pop_front();
}
///////////////////////////////////////////////////////////////
// Preemption Functions to decide when and where to preempt a process.

bool willPreempt()
{
    cout << "preempt bool" << endl;
    float cpuFinish = expectedEndTime();
    float cpuRemaining = cpuFinish - eventQ.front().time;

    if (eventQ.front().time < cpuFinish && eventQ.front().pLink->remaining < cpuRemaining)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void schedulePreemption()
{
    cout << "testing" << endl;
    bool exists = false;

    event *preempt = new event;
    preempt->time = eventQ.front().time;
    preempt->type = PREEMPT;
    preempt->next = nullptr;
    preempt->pLink = eventQ.front().pLink;

    // eventQ.pop_front();
    if (!exists)
    {
        eventQ.push_back(*preempt);
        cout << "Preemption Scheduled for: " << eventQ.back().pLink->pid << endl;
        for (int i = 0; i < eventQ.size(); i++)
        {
            cout << eventQ[i].type << ", ";
        }
        cout << endl;
    }
}
void preemption()
{
    cout << "preempting process" << endl;
    bool exists = false;

    process *preemptProc = cpu->pLinkCpu;
    cpu->pLinkCpu->remaining = expectedEndTime() - eventQ.front().time;

    cpu->pLinkCpu = eventQ.front().pLink;
    cpu->clock = eventQ.front().time;

    if (cpu->pLinkCpu->reStart == 0.0)
    {
        cpu->pLinkCpu->start = eventQ.front().time;
    }
    else
    {
        cpu->pLinkCpu->reStart = eventQ.front().time;
    }

    event *pArrival = new event;
    pArrival->time = eventQ.front().time;
    pArrival->type = ARRIVE;
    pArrival->next = nullptr;
    pArrival->pLink = preemptProc;

    // eventQ.pop_front();
    /* for (int i = 0; i < eventQ.size(); i++)
    {
        if (pArrival->pLink->pid == eventQ[i].pLink->pid)
        {
            exists = true;
        }
    }
    if (!exists)
    {
        eventQ.push_back(*pArrival);
    } */
    eventQ.push_back(*pArrival);
}
float expectedEndTime()
{
    float estimate;
    float begin = cpu->pLinkCpu->start;
    float reStart = cpu->pLinkCpu->reStart;
    float remaining = cpu->pLinkCpu->remaining;

    estimate = (reStart == 0 ? begin : reStart) + remaining;

    return estimate;
}
///////////////////////////////////////////////////////////////
// Scheduler Logic
void FCFS()
{
    int arrivals = 0;
    int departures = 0;
    int dispatches = 0;
    while (departures < MAX_PROCESSES)
    {

        if (!cpu->busy)
        {
            scheduleArrival();
            if (!readyQ.empty())
            {
                scheduleDispatch();
            }
        }
        else
        {
            scheduleDeparture();
        }
        switch (eventQ.front().type)
        {
        case 1:
            arrival();
            arrivals++;
            break;
        case 2:
            dispatch();
            dispatches++;
            break;
        case 3:
            departure();
            departures++;
            break;
        default:
            cerr << "failed event";
        }
    }
}

// Shortest Remaining Time First
// NOT WORKTING: Preemption event seems to break the queue, or it's not scheduling enough things?
void SRTF()
{
    int arrivals = 0;
    int departures = 0;
    int dispatches = 0;
    /* while (departures < 15)
    {
    } */
    // for (int i = 0; i < 100; i++)
    while (departures < MAX_PROCESSES)
    {
        if (arrivals < (MAX_PROCESSES * 1.20))
        {
            scheduleArrival();
            arrivals++;
        }

        if (!cpu->busy)
        {
            // scheduleArrival();
            if (!readyQ.empty())
            {
                scheduleDispatch();
            }
        }
        else
        {
            // Arrival Event
            if (eventQ.front().type == 1)
            {
                if (eventQ.front().time > expectedEndTime())
                {
                    scheduleDeparture();
                }
                else if (willPreempt())
                {
                    cout << eventQ.front().pLink->pid << " will preempt" << endl;
                    schedulePreemption();
                }
            }
        }
        switch (eventQ.front().type)
        {
        case 1:
            arrival();
            // arrivals++;
            break;
        case 2:
            dispatch();
            dispatches++;
            break;
        case 3:
            departure();
            departures++;
            break;
        case 4:
            preemption();
        default:
            cerr << "failed event: " + eventQ.front().type << endl;
        }
    }
}

int getHRRNProcess()
{

    process *hrrProc = readyQ.front().pLink;

    float arrival = hrrProc->arrival;
    float remaining = hrrProc->remaining;

    float HRR = getResponseRatio(arrival, remaining);
    for (int i = 0; i < readyQ.size(); i++)
    {
        if (getResponseRatio(arrival, remaining) > HRR)
        {
            HRR = getResponseRatio(arrival, remaining);
            hrrProc = readyQ[i].pLink;
            cout << hrrProc->pid << ": " << HRR << endl;
        }
    }

    return hrrProc->pid;
}
float getResponseRatio(float arrival, float burst)
{
    float HRR = ((cpu->clock - arrival) + burst) / burst;
    // cout << "HRR: " << HRR << endl;
    return HRR;
}

void HRRN()
{
    {
        int arrivals = 0;
        int departures = 0;
        int dispatches = 0;
        while (departures < MAX_PROCESSES)
        {

            if (!cpu->busy)
            {
                scheduleArrival();
                if (!readyQ.empty())
                {
                    scheduleDispatch();
                }
            }
            else
            {
                scheduleDeparture();
            }
            switch (eventQ.front().type)
            {
            case 1:
                arrival();
                arrivals++;
                break;
            case 2:
                dispatch();
                dispatches++;
                break;
            case 3:
                departure();
                departures++;
                break;
            default:
                cerr << "failed event";
            }
        }
    }
}
void RR()
{
}
