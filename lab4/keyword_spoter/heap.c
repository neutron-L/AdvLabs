//
// Created by 20550 on 2022/12/5.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "heap.h"

#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i) ((i) * 2 + 1)
#define RIGHT(i) (LEFT(i) + 1)


#define INCREMENT 128

static void heapify(Heap * ph, uint32_t index);
static void adjust_up(Heap * ph, uint32_t index);
static void swap(Heap * ph, uint32_t i, uint32_t j);
static bool compare(Heap * ph, double vi, double vj);

Heap * init_heap(uint32_t capacity, bool less, bool is_fixed)
{
    Heap * ph = (Heap *) malloc(sizeof(Heap));
    ph->capacity = capacity;
    ph->arr_item = (Tuple **)calloc(capacity, sizeof(Tuple *) );
    ph->size = 0;
    ph->less = less;
    ph->is_fix = is_fixed;

    return ph;
}

void insert_heap_pointer(Heap * ph, Tuple * elem)
{
    if (ph->size == ph->capacity)
    {
        ph->capacity += INCREMENT;
        ph->arr_item = realloc(ph->arr_item, sizeof(Tuple *) * ph->capacity);
        memset(ph->arr_item + ph->size, 0, sizeof(Tuple *) * INCREMENT);
    }
    // 如果指向一块不需要的内存 则先释放掉它
    if (ph->arr_item[ph->size] != NULL)
    {
        destroy_tuple(ph->arr_item[ph->size]);
        ph->arr_item[ph->size] = NULL;
    }
    ph->arr_item[ph->size] = elem;
    uint32_t index = ph->size;
    ++ph->size;
    adjust_up(ph, index);
}

void insert_heap_data(Heap * ph, uint32_t first, double second)
{
    if (ph->size == ph->capacity)
    {
        if (!ph->is_fix)
        {
            ph->capacity += INCREMENT;
            ph->arr_item = realloc(ph->arr_item, sizeof(Tuple *) * ph->capacity);
            memset(ph->arr_item + ph->size, 0, sizeof(Tuple *) * INCREMENT);
        }
        else
        {
            Tuple * pt = ph->arr_item[0];
            if (compare(ph, pt->second, second))
            {
                extract_top(ph);
                insert_heap_data(ph, first, second);
            }
            return;
        }
    }
    // 如果已经有空间，则复用
    if (ph->arr_item[ph->size] != NULL)
    {
        ph->arr_item[ph->size]->first = first;
        ph->arr_item[ph->size]->second = second;
    } else
        ph->arr_item[ph->size] = init_tuple(first, second);
    uint32_t index = ph->size;
    ++ph->size;
    adjust_up(ph, index);
}

Tuple * extract_top(Heap * ph)
{
    Tuple * res = ph->arr_item[0];
    swap(ph, 0, --ph->size);
    heapify(ph, 0);

    return res;
}

Tuple * peek(Heap * ph)
{
    return  ph->arr_item[0];
}


bool empty_heap(Heap * ph)
{
    return ph->size == 0;
}

void set_less(Heap * ph, bool LESS)
{
    ph->less = LESS;
}

void set_fixed(Heap * ph, bool fixed)
{
    ph->is_fix = fixed;
}

void set_capacity(Heap * ph, uint32_t capacity)
{
    if (ph->capacity < capacity)
        ph->arr_item = realloc(ph->arr_item, sizeof(Tuple *) * capacity);
    ph->capacity = capacity;
}


void heap_clear(Heap * ph)
{
    ph->size = 0;
}


void destroy_heap(Heap * ph)
{
    for (int i = 0; i < ph->capacity; ++i)
    {
        if(ph->arr_item[i])
        {
            destroy_tuple(ph->arr_item[i]);
            ph->arr_item[i] = NULL;
        }
    }
    free(ph);
}

static void heapify(Heap * ph, uint32_t index)
{
    while (index < ph->size)
    {
        uint32_t l = LEFT(index);
        uint32_t r = RIGHT(index);
        uint32_t t = index;
        if (l < ph->size && compare(ph, ph->arr_item[l]->second, ph->arr_item[index]->second))
            t = l;
        if (r < ph->size && compare(ph, ph->arr_item[r]->second, ph->arr_item[t]->second))
            t = r;
        if (t != index)
        {
            swap(ph, index, t);
            index = t;
        }
        else
            break;
    }
}

static void adjust_up(Heap * ph, uint32_t index)
{
    uint32_t par;

    while (index)
    {
        par = PARENT(index);
        if (compare(ph, ph->arr_item[index]->second, ph->arr_item[par]->second))
        {
            swap(ph, index, par);
            index = par;
        }
        else
            break;
    }
}

static void swap(Heap * ph, uint32_t i, uint32_t j)
{
    Tuple * temp = ph->arr_item[i];
    ph->arr_item[i] = ph->arr_item[j];
    ph->arr_item[j] = temp;
}


static bool compare(Heap * ph, double vi, double vj)
{
    if (ph->less)
        return vi < vj;
    else
        return vi > vj;
}