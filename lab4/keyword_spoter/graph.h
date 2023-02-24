//
// Created by 20550 on 2022/12/5.
//

#ifndef KEYWORD_SPOTER_GRAPH_H
#define KEYWORD_SPOTER_GRAPH_H

#include "ac.h"
#include "matrix.h"
#include "heap.h"
#include "tree.h"

typedef struct
{
    // int -- > string
    const char ** key_arr;
    uint32_t capacity;   // capacity of keywords, equals the length of key_arr
    uint32_t size;   // number of keywords
    uint32_t edges; // number of edges

    // 相关的统计信息
    double * degree;   // 每个词在图中的度
    double * closeness;   // 到其他节点最短路长度和（中心性倒数）
    double * pagerank; // 每个词的pagerank值

    // 辅助数据结构
    Tree ** neighbor_arr;    // 每个关键字对应一个neighbor tree
    List * plt;              // 用于保存匹配字符串的结果，包括出现的关键字的id和位置
    Heap * ph;
    double * dist;           // 保存节点u到其他节点的最短路

    //    ac自动机
    Automaton * pa;
    // Matrix
    Matrix * pmx;

    uint8_t  window_size;
} Graph;


/* 操作：初始化一个graph */
/* 前提条件：graph的窗口大小 wsz */
/* 后置条件：返回指向一个已初始化的graph的指针 */
Graph * init_graph(uint8_t wsz);


/* 操作：将str字符串插入到pg指定的graph中 */
/* 前提条件：pg指向已经被初始化的graph，str指向非空字符串 */
/* 后置条件：str被插入，pg记录插入的str，pg中的ac自动机中新增相应的状态节点 */
void insert_graph(Graph * pg, const char * str);

/* 操作：基于pg指向的graph，搜索str字符串，记录出现的关键字情况 */
/* 前提条件：pg指向已经被初始化的graph，str指向非空字符串 */
/* 后置条件：pg指向的graph中的matrix记录了str中关键字出现的情况 */
void search(Graph * pg, const char * str);


/* 操作：统计信息，并构造相关矩阵 */
/* 前提条件：pg指向已经被初始化的graph，且完成了关键字令居寻找的过程 */
/* 后置条件：pg指向的graph中的matrix以及其他信息统计完毕 */
void build_graph(Graph * pg);


void compute_shortest_path(Graph * pg, uint32_t u);


/* 操作：统计信息，并构造相关矩阵 */
/* 前提条件：pg指向已经被初始化的graph，且完成了关键字令居寻找的过程 */
/* 后置条件：pg指向的graph中的matrix以及其他信息统计完毕 */
void calculate(Graph * pg);


/* 操作：输出度数、closeness和PageRank最高的前number个词语以及相关信息 */
/* 前提条件：pg指向已经被初始化的graph，file为非空有效文件路极，number为一个适当大的整数 */
/* 后置条件：相关统计信息输出到了指定文件中 */
void print_result(Graph *pg, const char * file, uint8_t number);


/* 操作：销毁pg指向的graph */
/* 前提条件：pg指向已经被初始化的graph */
/* 后置条件：graph被成功销毁无内存泄露 */
void destroy_graph(Graph *pg);

#endif //KEYWORD_SPOTER_GRAPH_H
