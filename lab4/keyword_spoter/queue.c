//
// Created by 20550 on 2022/11/6.
//
#include <assert.h>
#include "queue.h"


Queue * init_queue()
{
    Queue  * q = (Queue*) malloc(sizeof(Queue));
    q->front = q->rear = NULL;

    return q;
}

void push(Queue * q, void * val)
{
    QNode *node = (QNode*)malloc(sizeof(QNode));
    node->val = val;
    node->next = NULL;
    if (q->rear)
        q->rear->next = node;
    q->rear = node;
    if (q->front == NULL)
        q->front = q->rear;
}

void * pop(Queue * q) {
    assert(q->front != NULL);
    void *val = q->front->val;
    QNode *tmp = q->front;
    q->front = q->front->next;
    if (q->rear == tmp)
        q->rear = NULL;
    free(tmp);

    return val;
}

bool empty_queue(Queue *q)
{
    return q->front == NULL;
}

void destroy_queue(Queue *q)
{
    while (!empty_queue(q))
        pop(q);
    free(q);
}