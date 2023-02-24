//
// Created by 20550 on 2022/12/5.
//

#include <stdlib.h>
#include "list.h"


static ListNode * alloc_listnode(uint32_t target, uint32_t pos);

List * init_list()
{
    List * pl = (List *)malloc(sizeof(List));
    pl->head = pl->tail = NULL;
    pl->RESET = false;

    return pl;
}

void insert_lst(List * pl, uint32_t target, uint32_t pos)
{
    if (pl->RESET)
    {
        pl->RESET = false;
        pl->tail = pl->head;
        if (!pl->head)
            pl->head = pl->tail = alloc_listnode(target, pos);
        else
        {
            pl->tail->target = target;
            pl->tail->pos = pos;
        }
    } else
    {
        if (!pl->head)
            pl->head = pl->tail = alloc_listnode(target, pos);
        else if (!pl->tail->next)
        {
            pl->tail->next = alloc_listnode(target, pos);
            pl->tail = pl->tail->next;
        } else
        {
            pl->tail = pl->tail->next;
            pl->tail->target = target;
            pl->tail->pos = pos;
        }
    }
}

void reset_list(List * pl)
{
    pl->RESET = true;
}

void destroy_list(List * pl)
{
    ListNode * tmp;
    while (pl->head)
    {
        tmp = pl->head->next;
        free(pl->head);
        pl->head = tmp;
    }

    free(pl);
}


static ListNode * alloc_listnode(uint32_t target, uint32_t pos)
{
    ListNode * pn  = (ListNode *) malloc(sizeof(ListNode));
    pn->target = target;
    pn->pos = pos;
    pn->next  = NULL;

    return pn;
}
