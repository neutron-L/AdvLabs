//
// Created by 20550 on 2022/12/11.
//

#ifndef KEYWORD_SPOTER_TUPLE_H
#define KEYWORD_SPOTER_TUPLE_H

#include <stdint.h>


/*
 * 存放一个或多个数据成员，目前存放两个，模拟c++中的pair
 * */
typedef struct
{
    uint32_t first;
    double second;
} Tuple;


/* 操作： 初始化一个tuple，返回指向新tuple的指针 */
/* 前置条件： */
/* 后置条件： 成功初始化并返回指向新tuple的指针 */
Tuple * init_tuple(uint32_t first, double second);

/* 操作： 销毁一个tuple */
/* 前置条件： pt指向已初始化的tuple */
/* 后置条件：成功释放pt指向的空间 */
void destroy_tuple(Tuple *pt);

#endif //KEYWORD_SPOTER_TUPLE_H
