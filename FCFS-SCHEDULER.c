#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum State
{
    READY,
    RUNNING,
    WAITING,
    TERMINATED
} State;

typedef struct Process
{
    int processId;
    char processName[50];
    int burstTime;
    int remainingTime;
    int ioStartTime;
    int ioDuration;
    int ioRemainingTime;
    int arrivalTime;
    int executedTime;
    int waitingTime;
    int turnAroundTime;
    int completionTime;
    State currState;
} Process;

typedef struct KillEvent
{
    int processId;
    int time;
} KillEvent;

typedef struct Node
{
    Process* processPtr;
    struct Node* next;
} Node;

typedef struct Queue
{
    Node* front;
    Node* rear;
} Queue;

static int numberOfProcesses;
static int numberOfKillEvent;
static int time = 0, terminatedCount = 0;
Process* runningProcess = NULL;

Queue readyQueue;
Queue waitingQueue;
Queue terminatedQueue;

Process hashTable[100];
KillEvent killTable[100];

bool isEmpty (Queue* queue)
{
    return (queue->front == NULL || queue->front->processPtr == NULL);
}

void insertProcess (Queue* queue, Process* currProcess)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->processPtr = currProcess;
    newNode->next = NULL;
    if (isEmpty (queue))
    {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next= newNode;
    queue->rear = newNode;
}

Process* getProcess (Queue* queue)
{
    if (queue->front == NULL)
    {
        return NULL;
    }
    Process* currProcess = queue->front->processPtr;
    queue->front = queue->front->next;
    return currProcess;
}

void getProcessData ()
{
    printf("Enter Number of Processes: ");
    scanf("%d", &numberOfProcesses);
    for (int index = 0; index < numberOfProcesses; index++)
    {
        char pName[50];
        int pId, burstTime, ioStartTime, ioDuration;
        scanf("%s", pName);
        scanf("%d", &pId);
        scanf("%d", &burstTime);
        scanf("%d", &ioStartTime);
        scanf("%d", &ioDuration);
        int hashIndex = pId % 100;
        strcpy(hashTable[hashIndex].processName, pName);
        hashTable[hashIndex].processId = pId;
        hashTable[hashIndex].burstTime = burstTime;
        hashTable[hashIndex].ioStartTime = ioStartTime;
        hashTable[hashIndex].remainingTime = burstTime;
        hashTable[hashIndex].ioDuration = ioDuration;
        hashTable[hashIndex].ioRemainingTime = ioDuration;
        hashTable[hashIndex].arrivalTime = 0;
        hashTable[hashIndex].waitingTime = 0;
        hashTable[hashIndex].completionTime = 0;
        hashTable[hashIndex].executedTime = 0;
        hashTable[hashIndex].turnAroundTime = 0;
        hashTable[hashIndex].currState = READY;
        insertProcess (&readyQueue, &hashTable[hashIndex]);
        getchar();
    }
}

void getKillEvent ()
{
    printf("Enter Number of Kill Events: ");
    scanf("%d", &numberOfKillEvent);
    for (int index = 0; index < numberOfKillEvent; index++)
    {
        char tempString[10];
        scanf("%s", tempString);
        int pId, killTime;
        scanf("%d %d", &pId, &killTime);
        killTable[index].processId = pId;
        killTable[index].time = killTime;
    }
}

void removeProcessFromQueue (Queue* currQueue, int pId)
{
    Node* current = currQueue->front;
    Node* previous = NULL;
    Node* temp = NULL;
    while (current != NULL)
    {
        if (current->processPtr->processId == pId)
        {
            if (previous == NULL)
            {
                temp = currQueue->front;
                currQueue->front = current->next;
            }
            else
            {
                temp = current;
                previous->next = current->next;
            }

            if (current == currQueue->rear)
            {
                temp = currQueue->rear;
                currQueue->rear = previous;
            }
            break;
        }
        previous = current;
        current = current->next;
        free(temp);
    }
}

Process* getProcessFromTable (int pId)
{
    return &hashTable[pId%100];
}

void applyKillEvent ()
{
    for (int index = 0; index < numberOfKillEvent; index++)
    {
        if (killTable[index].time == time)
        {
            Process* target = getProcessFromTable (killTable[index].processId);

            if (target == runningProcess)
            {
                runningProcess->currState = TERMINATED;
                insertProcess (&terminatedQueue, runningProcess);
                runningProcess->completionTime = time;
                terminatedCount++;
                runningProcess = NULL;
                continue;
            }
            else if (target->currState == READY)
            {
                target->completionTime = time;
                target->currState = TERMINATED;
                removeProcessFromQueue (&readyQueue, target->processId);
                insertProcess (&terminatedQueue, target);
                terminatedCount++;
                continue;
            }
            else if (target->currState == WAITING)
            {
                target->completionTime = time;
                target->currState = TERMINATED;
                removeProcessFromQueue (&waitingQueue, target->processId);
                insertProcess (&terminatedQueue, target);
                terminatedCount++;
                continue;
            }
        }
    }
}

void runCpuTime ()
{
    if (runningProcess == NULL)
    {
        if (!isEmpty (&readyQueue))
        {
            runningProcess = getProcess (&readyQueue);
            runningProcess->currState = RUNNING;
        }
    }
    if (runningProcess != NULL)
    {
        runningProcess->executedTime += 1;
        if (runningProcess->executedTime == runningProcess->ioStartTime && runningProcess->ioRemainingTime > 0)
        {
            runningProcess->currState = WAITING;
            insertProcess (&waitingQueue, runningProcess);
            runningProcess = NULL;
            return;
        }
        runningProcess->remainingTime -= 1;
        if (runningProcess->remainingTime == 0)
        {
            runningProcess->currState = TERMINATED;
            runningProcess->completionTime = time;
            insertProcess (&terminatedQueue, runningProcess);
            runningProcess = NULL;
            terminatedCount++;
            return;
        }
    }
}

void runIoTime ()
{
    Node* current = waitingQueue.front;
    while (current != NULL)
    {
        current->processPtr->ioRemainingTime -= 1;
        if (current->processPtr->ioRemainingTime == 0)
        {
            removeProcessFromQueue (&waitingQueue, current->processPtr->processId);
            current->processPtr->currState = READY;
            insertProcess (&readyQueue, current->processPtr);
        }
        current = current->next;
    }
}

void runWaitingTime ()
{
    Node* current = readyQueue.front;
    while (current != NULL)
    {
        current->processPtr->waitingTime += 1;
        current = current->next;
    }
}

void displayResults ()
{
    Node* current = terminatedQueue.front;
    printf("PID |  Name  |  CPU  |  IO  |  Turnaround  |  Waiting\n");
    while (current != NULL)
    {
        current->processPtr->turnAroundTime = current->processPtr->completionTime - current->processPtr->arrivalTime;
        Process* currProcess = current->processPtr;
        printf("%-5d %-10s %-6d %-9d %-11d %-7d\n", currProcess->processId, currProcess->processName, currProcess->burstTime
                                        , currProcess->ioDuration, currProcess->turnAroundTime, currProcess->waitingTime);
        current = current->next;
    }
}

void freeUsedMemory ()
{
    Node* current = terminatedQueue.front;
    Node* temp = NULL;
    while (current != NULL)
    {
        temp = current;
        current = current->next;
        free(temp);
    }
}

int main ()
{
    readyQueue.front = readyQueue.rear = NULL;
    terminatedQueue.front = terminatedQueue.rear = NULL;
    waitingQueue.front = waitingQueue.rear = NULL;
    getProcessData ();
    getKillEvent ();
    while (terminatedCount != numberOfProcesses)
    {
        applyKillEvent ();

        runCpuTime ();

        runIoTime ();

        runWaitingTime ();

        time += 1;
    }
    displayResults ();
    freeUsedMemory ();
  return 0;
}
