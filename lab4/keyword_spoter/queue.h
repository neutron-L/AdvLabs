//
// Created by 20550 on 2022/11/6.
//

#ifndef AC_AUTOMATON_QUEUE_H
#define AC_AUTOMATON_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct QNode
{
    void * val;
    struct QNode * next;
} QNode;

typedef struct Queue
{
    QNode * front;
    QNode * rear;
}Queue;

Queue * init_queue();

void push(Queue * q, void * val);
void * pop(Queue * q);
bool empty_queue(Queue *q);
void destroy_queue(Queue *q);

#endif //AC_AUTOMATON_QUEUE_H
