/*
 * Header file for shared variables
 */

#ifndef HEADER_H
#define HEADER_H

// Library instancing
#include <deque>

using namespace std;
#define MAX_PROCESSES 10000

// Initializion variables
int lambda;
float serviceTime;
float timeSlice;
int scheduler;

// Variables for metrics
float turnaroundTime;
float waitingTime;
float completeTime;
float busyTime;
float quantum;
float qClock;
float avgServeTime = 0.0;
float readyQCount;

// Structs
struct process
{
    int pid;
    float arrival;
    float start;
    float reStart;
    float finish;
    float burst;
    float remaining;

    struct process *nextProc;
};
struct event
{
    float time;
    int type;
    // add more fields
    struct event *next;
    struct process *pLink;
};

struct CPU
{
    float clock;
    bool busy;
    struct process *pLinkCpu;
};

struct readyProc
{
    struct process *pLink;
    struct readyProc *next;
};
event *eve;
CPU *cpu;
process *processHead;
process *processTail;
// Deques to serve as queues for a ready queue and an event queue
deque<readyProc> readyQ;
deque<event> eventQ;

// Function Definitions:
void init();
void generate_report();
void FCFS();
void SRTF();
void HRRN();

int run_sim();

float urand();
float genexp(float);
float expectedEndTime();
float getResponseRatio(float, float);

int getHRRNProcess();

#endif

/*

int main()
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
}

*/

/*
switch (eve->type)
    {
    // Arrival
    case 1:
        process_event1(eve);

        break;
    case EVENT2:
        // process_event2(eve);
        break;
    // add more events
    default:
        break;
    }

*/

/*
for (int i = 0; i < eventQ.size(); i++)
        {
            cout << eventQ[i].type << ", ";
        }
        cout << endl;
        for (int i = 0; i < readyQ.size(); i++)
        {
            cout << readyQ[i].pLink->pid << ", ";
        }
        cout << endl;
*/