//
// Created by 20550 on 2022/12/5.
//

#ifndef KEYWORD_SPOTER_LIST_H
#define KEYWORD_SPOTER_LIST_H

#include <stdint.h>
#include <stdbool.h>

/* 包含了出现的关键字字符串对应的序号和位置 */
typedef struct ListNode
{
    uint32_t target;
    uint32_t pos;
    struct ListNode * next;
} ListNode;

typedef struct
{
    ListNode * head;
    ListNode * tail;
    bool RESET;
} List;


/* 操作：初始化一个链表 */
/* 前提条件： */
/* 后置条件：返回指向一个已初始化的list的指针 */
List * init_list();


/* 操作：将<target, pos>按尾插法插入到pl指定的list中 */
/* 前提条件：pl指向已经被初始化的list */
/* 后置条件：<target, pos>被插入到链表，list中新增相应的节点 */
void insert_lst(List * pl, uint32_t target, uint32_t pos);


/* 操作：重置pl指向的list，使其尾巴节点的下一个节点指向head */
/* 前提条件：pl指向已经被初始化的list */
/* 后置条件： */
void reset_list(List * pl);


/* 操作：销毁pa指向的ac自动机 */
/* 前提条件：pa指向已经被初始化的ac自动机 */
/* 后置条件：ac自动机被成功销毁无内存泄露 */
void destroy_list(List * pl);


#endif //KEYWORD_SPOTER_LIST_H
