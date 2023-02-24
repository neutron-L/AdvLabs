//
// Created by 20550 on 2022/12/11.
//

#include <stdlib.h>
#include "tree.h"

static TNode * alloc_tnode();
static void traverse_delegate(TNode * pn,  Tuple *** pt);
static void destroy_tnode(TNode * pn);

Tree * init_tree()
{
    Tree * pt = (Tree *)malloc(sizeof(Tree));
    pt->root = NULL;
    pt->size = 0;

    return pt;
}

void insert_tree(Tree* pt, uint32_t u)
{
    TNode *prev = NULL;
    TNode *cur = pt->root;
    uint32_t v;
    while (cur)
    {
        prev = cur;
        v = cur->item->first;
        if (v < u)
            cur = cur->right;
        else if (v > u)
            cur = cur->left;
        else
        {
            ++cur->item->second;
            return;
        }
    }

    // root node
    if (!prev)
    {
        pt->root = alloc_tnode();
        cur = pt->root;
    } else
    {
        if (v < u)
        {
            prev->right = alloc_tnode();
            cur = prev->right;
        } else
        {
            prev->left = alloc_tnode();
            cur = prev->left;
        }
    }
    cur->item->first = u;
    cur->item->second = 1;
    ++pt->size;
}

void traverse(Tree * pt, Tuple *** ptr)
{
    traverse_delegate(pt->root, ptr);
}


void destroy_tree(Tree * pt)
{
    destroy_tnode(pt->root); // 递归销毁根结点
    free(pt);            // 释放pt指向的空间
}

static TNode * alloc_tnode()
{
    TNode * pt = (TNode *) malloc(sizeof(TNode));
    pt->left = pt->right = NULL;
    pt->item = init_tuple(0, 0);    // 默认item的值均为0

    return pt;
}

/* 先序遍历 */
static void traverse_delegate(TNode * pn,  Tuple *** pt)
{
    if (pn)
    {
        traverse_delegate(pn->left, pt);
        **pt = pn->item;
        ++*(pt);
        traverse_delegate(pn->right, pt);
    }
}


static void destroy_tnode(TNode * pn)
{
    if (pn)
    {
        destroy_tnode(pn->left);
        destroy_tnode(pn->right);
        // item由matrix释放
        free(pn);
    }
}
