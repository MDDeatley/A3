//By Ryan Corbin and  Michael Deatley

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"

void insertEnd(struct node **head, Task *newTask);

struct node *H = NULL;
struct node *Done = NULL;
int n = 0;

void add(char *name, int priority, int burst){
    Task *in = malloc(sizeof(Task));
    in->name = name;
    in->burst = burst;
    in->priority = priority;
    in->tid = n;
    n++;
    insertEnd(&H, in);
}

// invoke the scheduler
void schedule(){
    int time = 0;//clock
    int *ta = malloc (n * sizeof *ta);//holds turn around times for tasks
    int *wt = malloc (n * sizeof *wt);//holds waiting times for tasks
    int *bt = malloc (n * sizeof *bt);//holds burst times for tasks
    
    int *WT = malloc (n * sizeof *WT);//holds burst times for tasks
    
    //set waiting times & turn around times to 0
    for(int i = 0; i < n; i++){
        *(ta + i) = 0;
        *(wt + i) = 0;
        *(WT + i) = 0;
    }
    
    struct node *Temp = H;
    while(Temp != NULL){
        *(bt + (Temp->task->tid)) = Temp->task->burst;
        Temp = Temp->next;
    }
    
    //while queue is not empty
    while(H != NULL){       
        
        *(wt + (H->task->tid)) += time - (*(WT + (H->task->tid)));
        *(WT + (H->task->tid)) = time +10;
        
        //tick clock and do task
        for(int i = 0; i < 10; i++){
            time++;//increment clock
            H->task->burst--;//decrement remaining burst time
            if(H->task->burst == 0){
                break;
            }
        }
        //if time remaining is 0 remove from job queue
        if(H->task->burst == 0){
            *(ta + (H->task->tid)) += time; //set turnaround time
            insertEnd(&Done, H->task);
            delete(&H, H->task);
        //else move to end of job queue
        }else{
            Task *Temp0 = H->task;
            delete(&H, Temp0);
            insertEnd(&H, Temp0);
        }
    }
    
    float avwt = 0, avta = 0;
    for(int i = 0; i < n; i ++){
        avwt +=  *(wt + i);
    }
    avwt /= n;
    for(int i = 0; i < n; i ++){
        avta +=  *(ta + i);
    }
    avta /= n;
    //print results
    printf("Process\t    Burst Time    \tWaiting Time\tTurnaround Time\n");
    while(Done != NULL){
        printf("%s\t\t  %d\t\t    %d\t\t\t%d\n",Done->task->name , *(bt + (Done->task->tid)), *(wt + (Done->task->tid)), *(ta + (Done->task->tid)));
        delete(&Done, Done->task);
    }
    printf("\nAverage Waiting Time = %.2f\n",avwt);
    printf("Average Turnaround Time = %.2f\n",avta);
}

//Inserts Task at End of list

void insertEnd(struct node **head, Task *newTask) {
    struct node *newNode = malloc(sizeof(struct node));
    newNode->task = newTask;  
    newNode->next = NULL;
    if(*head == NULL){
        *head = newNode;
        return;
    }
    struct node *Temp = *head;
    while(Temp->next != NULL){
        Temp = Temp->next;
    }
    Temp->next = newNode;
}
