//By Ryan Corbin and  Michael Deatley

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
 
void insertEnd(struct node **head, Task *newTask);

struct node *H[10] = {NULL};
struct node *Done = NULL;
int n=0;

// add a task to the job queue
void add(char *name, int priority, int burst){
    Task *in = malloc(sizeof(Task));
    in->name = name;
    in->priority = priority;
    in->burst = burst;
    in->tid = n;
    n++;
    
    //add task to end of queue with priority i
    for(int i = 1; i < 11; i++){
        if(i == priority){
            insertEnd(&H[i-1], in);
        }
    }
}

void schedule(){
    int time = 0;//clock
    int *ta = malloc (n * sizeof *ta);//holds turn around times for tasks
    int *wt = malloc (n * sizeof *wt);//holds waiting times for tasks
    int *bt = malloc (n * sizeof *bt);//holds burst times for tasks
    
    //set waiting times & turn around times to 0
    for(int i = 0; i < n; i++){
        *(ta + i) = 0;
        *(wt + i) = 0;
    }
    
    //for each priority level
    for(int i = 9; i >= 0; i--){
        struct node *Temp = H[i];
        while(Temp != NULL){
            *(bt + (Temp->task->tid)) = Temp->task->burst;
            Temp = Temp->next;
        }
        //while queue is not empty
        while(H[i] != NULL){              
            //tick clock and do task
            for(int j = 0; j < 10; j++){
                time++;//increment clock
                H[i]->task->burst--;//decrement remaining burst time
                if(H[i]->task->burst == 0){
                    break;
                }
            }
            //if time remaining is 0 remove from job queue
            if(H[i]->task->burst == 0){
                *(ta + (H[i]->task->tid)) += time;//set turnaround time
                *(wt + (H[i]->task->tid)) += time - *(bt + (H[i]->task->tid));//set wait time
                insertEnd(&Done, H[i]->task);
                delete(&H[i], H[i]->task);
            //else move to end of job queue
            }else{
                Task *Temp0 = H[i]->task;
                delete(&H[i], Temp0);
                insertEnd(&H[i], Temp0);
            }
        }
    }
    //calculate avgs
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

