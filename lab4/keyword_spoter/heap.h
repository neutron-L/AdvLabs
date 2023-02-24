//
// Created by 20550 on 2022/12/5.
//

#ifndef KEYWORD_SPOTER_HEAP_H
#define KEYWORD_SPOTER_HEAP_H
#include <stdbool.h>
#include "tuple.h"

/*
 * arr_item为堆中的tuple指针类型的数组
 * capacity和size表明当前堆的容量和大小
 * pivot表明，堆的有序性根据tuple的第几个元素决定
 * bool类型成员表明堆的容量是否固定
 * less：true说明是小顶堆 false：说明是大顶堆
 * */
typedef struct
{
    Tuple ** arr_item;
    uint32_t capacity;
    uint32_t size;
    bool less;
    bool is_fix;
} Heap;


/* 操作：初始化一个容量为capacity */
/* 前置条件： */
/* 后置条件：返回指向已初始化的堆的指针 */
Heap * init_heap(uint32_t capacity, bool less, bool is_fix);



/* 操作： */
/* 前置条件：ph指向一个已初始化的heap */
/* 后置条件： */
void insert_heap_pointer(Heap * ph, Tuple * elem);

/* 操作： */
/* 前置条件：ph指向一个已初始化的heap */
/* 后置条件： */
void insert_heap_data(Heap * ph, uint32_t first, double second);


/* 操作：取出并删除heap的堆顶元素 */
/* 前置条件：ph指向一个已初始化的heap，且size不为零 */
/* 后置条件：成功取出堆顶元素并维护heap的结构 */
Tuple * extract_top(Heap * ph);


/* 操作：取出heap的堆顶元素 */
/* 前置条件：ph指向一个已初始化的heap，且size不为零 */
/* 后置条件：成功取出堆顶元素并维护heap的结构 */
Tuple * peek(Heap * ph);

/* 操作：判断heap是否为空 */
/* 前置条件：ph指向一个已初始化的heap */
/* 后置条件：成功返回heap是否为空 */
bool empty_heap(Heap * ph);



void set_capacity(Heap * ph, uint32_t capacity);

void set_less(Heap * ph, bool LESS);


void set_fixed(Heap * ph, bool fixed);


void heap_clear(Heap * ph);

/* 操作：销毁给定指针指向的堆 */
/* 前置条件：ph指向一个已初始化的堆 */
/* 后置条件：成功销毁堆 */
void destroy_heap(Heap * ph);

#endif //KEYWORD_SPOTER_HEAP_H
