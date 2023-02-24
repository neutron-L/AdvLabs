//
// Created by 20550 on 2022/11/6.
//

#ifndef AC_AUTOMATON_QUEUE_H
#define AC_AUTOMATON_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
    void * val;
    struct Node * next;
} Node;

typedef struct Queue
{
    Node * front;
    Node * rear;
}Queue;

Queue * init_queue();

void push(Queue * q, void * val);
void * pop(Queue * q);
bool empty(Queue *q);

#endif //AC_AUTOMATON_QUEUE_H
