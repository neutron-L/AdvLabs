//
// Created by 20550 on 2022/11/5.
//

#ifndef AC_AUTOMATON_AC_H
#define AC_AUTOMATON_AC_H

#include <stdint.h>
#include <stdbool.h>


typedef struct AcNode
{
    // layer
    uint32_t  level;
    struct AcNode ** children;
    struct AcNode * fail;
    bool is_key;
    bool has_key;
}AcNode;

typedef struct
{
    AcNode * root;
    uint32_t order;
    uint32_t mask_len;
    uint32_t size;

    unsigned char * dp; // 分词数组
    uint32_t len; //分词数组长度
}Automaton;

/* 操作：初始化一个ac自动机 */
/* 前提条件：ac自动机”阶数“，根据几个位进行状态转移，最好是1 2 4 8 */
/* 后置条件：返回指向一个已初始化的ac自动机的指针 */
Automaton * init_automaton(uint32_t order);


/* 操作：将str字符串插入到pa指定的ac自动机中 */
/* 前提条件：pa指向已经被初始化的ac自动机，str指向非空字符串 */
/* 后置条件：str被插入，ac自动机中新增相应的状态节点 */
void insert(Automaton * pa, const char * str);


/* 操作：为pa指定的ac自动机中的节点设置fail指针 */
/* 前提条件：pa指向已经被初始化的ac自动机 */
/* 后置条件：pa中所有状态节点的fail指针均设置成功且正确 */
void set_fail_pointer(Automaton * pa);


/* 操作：在pa指向的ac自动机中匹配str字符串 */
/* 前提条件：pa指向已经被初始化的ac自动机，str指向非空字符串，goto_times是一个有效合法的内存地址 */
/* 后置条件：返回str中匹配的模式串总次数，goto_times中保存状态转移的次数 */
uint32_t match(Automaton * pa, const char * str, uint32_t * goto_times);


/* 操作：销毁pa指向的ac自动机 */
/* 前提条件：pa指向已经被初始化的ac自动机 */
/* 后置条件：ac自动机被成功销毁无内存泄露 */
void destroy_automaton(Automaton *pa);

#endif //AC_AUTOMATON_AC_H
