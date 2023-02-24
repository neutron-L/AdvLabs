//
// Created by 20550 on 2022/10/11.
//

#ifndef BPLUS_BPLUSTREE_H
#define BPLUS_BPLUSTREE_H

#define DEBUG 0
#include <stdbool.h>


#define KEY char *
#define DATATYPE KEY


typedef struct BplusNode
{
    int keynum;
    KEY *keys;
    bool is_leaf;
//    DATATYPE * data;
    struct BplusNode ** child;
    struct BplusNode* parent;
    struct BplusNode * next;
} BplusNode;

typedef struct
{
    unsigned int order;
    unsigned int size;
    unsigned int layers;
    BplusNode * root;
    BplusNode * sqt;
    int (*cmp)(const DATATYPE, const DATATYPE);
}BplusTree;

/* initialize b+ tree */
BplusTree * init_bplus_tree(unsigned int, int (*)(const DATATYPE, const DATATYPE));
/* search the specified string in b+tree, and save the search times */
bool search(BplusTree *, DATATYPE *, unsigned int *);
/* insert the specified string in b+ tree */
void insert(BplusTree *, DATATYPE *);
/* destroy the b+tree */
void destroy(BplusTree *);


#endif //BPLUS_BPLUSTREE_H
