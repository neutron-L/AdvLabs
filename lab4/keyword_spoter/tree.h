//
// Created by 20550 on 2022/12/11.
//

#ifndef KEYWORD_SPOTER_TREE_H
#define KEYWORD_SPOTER_TREE_H

#include "tuple.h"

/*
 * tree的节点，包含一个指向tuple的指针item，相关数据存放在item中
 * 程序中item存储key: value键值对，pivot指示tuple中第几个元素为key
 *
 * */
typedef struct TNode
{
    Tuple * item;
    struct TNode * left;
    struct TNode * right;
} TNode;

typedef struct
{
    TNode * root;
    uint32_t size;
} Tree;

/* 操作：初始化一棵树 */
/* 前置条件：pivot为合理的值，指明tree中标识node的唯一关键字，即其不能超过tuple中的元素个数，程序中为2 */
/* 前置条件：成功初始化并返回指向tree的指针，其pivot为给定值 */
Tree * init_tree();


/* 操作：往树中添加一个关键字u，如果存在pivot位置为u的tuple，则递增tuple的值 */
/* 前置条件：pt指向已初始化的tree */
/* 后置条件： pt中添加了一个节点其tuple中的key为u，value为0，或递增了u对应的tuple的value*/
void insert_tree(Tree* pt, uint32_t u);


/* 操作： 前序遍历pt指向的tree，并使用visit为节点的tuple执行指定操作*/
/* 前置条件：pt指向已初始化的tree */
/* 后置条件：所有节点都有序被visit处理完 */
void traverse(Tree * pt, Tuple *** ptr);


/* 操作： 销毁pt指向的tree */
/* 前置条件：pt指向已初始化的tree */
/* 后置条件：成功释放pt指向的tree以及相关的空间 */
void destroy_tree(Tree * pt);


#endif //KEYWORD_SPOTER_TREE_H
