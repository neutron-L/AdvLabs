//
// Created by 20550 on 2022/10/11.
//

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "bplustree.h"


static BplusNode * allocate_node(unsigned int, bool);
static void destroy_node(BplusNode*);
static unsigned int get_index_insert(BplusTree *, BplusNode *, const char *);

/*
 * allocate and initialize a b+ node, if is_leaf, the node should be a leaf
 * the size of child array is b+tree->order + 1
 * */
static BplusNode * allocate_node(unsigned int order, bool is_leaf) {
    BplusNode *bplusNode = (BplusNode *) malloc(sizeof(BplusNode));
    bplusNode->keynum = 0;
    bplusNode->is_leaf = is_leaf;
    bplusNode->keys = (KEY*) malloc((order + 1) * sizeof(KEY));
    bplusNode->child = (BplusNode **) malloc((order + 1) * sizeof(BplusNode *));
    bplusNode->parent = NULL;
    bplusNode->next = NULL;

    if (is_leaf)
        memset(bplusNode->child, 0, (order + 1) * sizeof(BplusNode *));

    return bplusNode;
}

/*
 * Recursively release the memory occupied by the node and its children
 * The internal node and the leaf node point to the same memory,
 * and the leaf node is responsible for releasing
 * */
static void destroy_node(BplusNode* node) {
    if (node) {
        for (int i = 0; i < node->keynum; i++) {
//            if node is leaf, its child is null, so it does nothing
            destroy_node(node->child[i]);

        }
        if (node->is_leaf) {
            for (int i = 0; i < node->keynum; i++)
                free(node->keys[i]);
        }
        free(node->child);
        free(node->keys);
        free(node);
    }
}


/*
 * ptree：b+树
 * node：当前节点
 * target：需要比较的字符串
 * return：node中第一个不小于target的key的索引，如果没有返回最后一个key的下一个位置的索引，即keynum
 * */
static unsigned int get_index_insert(BplusTree * ptree, BplusNode * node, const char *target)
{
    unsigned int l = 0, r = node->keynum;
    unsigned int mid;

    while (l < r) {
        mid = (l + r) >> 1;
        if (ptree->cmp(node->keys[mid], target) < 0)
            l = mid+1;
        else
            r=mid;
    }
    return l;
}


/*
 * m: order
 * return a pointer to a b+tree of order m
 * */
BplusTree * init_bplus_tree(unsigned int m, int (*cmp)(const DATATYPE, const DATATYPE))
{
    BplusTree  *bplusTree = (BplusTree*)malloc(sizeof(BplusTree));
    bplusTree->root = allocate_node(m, true);
    bplusTree->order = m;
    bplusTree->size = 1;
    bplusTree->layers = 1;
    bplusTree->sqt = bplusTree->root;
    bplusTree->cmp = cmp;

    return bplusTree;
}



/* data: the data to be searched
 * times: pointer to the variable which stores the search times
 * return to true if the search is successful
 * */
bool search(BplusTree * ptree, DATATYPE * data, unsigned int * ptimes) {
    BplusNode *cur = ptree->root;
    unsigned int i;
    while (cur) {
        (*ptimes)++;
//        i = 0;
//        while (i < cur->keynum && ptree->cmp(*data, cur->keys[i]) > 0)
//            ++i;
        i = get_index_insert(ptree, cur, *data);
        if (i < cur->keynum && !ptree->cmp(*data, cur->keys[i]))
            return true;
        else if (i == cur->keynum)
            return false;
        else
            cur = cur->child[i];
    }

    return false;
}

/*
 * data: a pointer to the data of DATATYPE
 * */
void insert(BplusTree * ptree, DATATYPE * data) {
//    insert
    BplusNode *cur = ptree->root;
    unsigned int i;
    while (true) {
//        i = 0;
//        while (i < cur->keynum && ptree->cmp(*data, cur->keys[i]) > 0)
//            ++i;
        i = get_index_insert(ptree, cur, *data);
        if (!cur->is_leaf) {
            cur = cur->child[(i < cur->keynum) ? i : cur->keynum - 1];
        } else {
            for (int j = cur->keynum; j > i; --j)
                cur->keys[j] = cur->keys[j - 1];
            cur->keys[i] = *data;
            ++cur->keynum;
            break;
        }
    }


//    split node
    while (cur->keynum > ptree->order) {
//        for debug, check the order of node keys
#if (DEBUG)
        for (i = 1; i < cur->keynum; ++i)
            assert(ptree->cmp(cur->keys[i], cur->keys[i-1]) >= 0);
#endif
        int mid = cur->keynum / 2;
//        allocate a new node
        BplusNode *node = allocate_node(ptree->order, cur->is_leaf);
        for (i = mid; i < cur->keynum; ++i) {
            node->keys[i - mid] = cur->keys[i];
            if (!cur->is_leaf)
            {
                node->child[i - mid] = cur->child[i];
                cur->child[i]->parent = node;
            }
        }
        node->keynum = cur->keynum - mid;
        cur->keynum = mid;

        node->parent = cur->parent;
//        if cur is leaf, insert the node to the list
        if (cur->is_leaf) {
            node->next = cur->next;
            cur->next = node;
        }
//
        if (ptree->root == cur) {
//            allocate a new root
            BplusNode *new_root = allocate_node(ptree->order, false);
            new_root->keynum = 2;
            new_root->child[0] = cur;
            new_root->child[1] = node;
            new_root->keys[0] = cur->keys[cur->keynum - 1];
            new_root->keys[1] = node->keys[node->keynum - 1];

            cur->parent = new_root;
            node->parent = new_root;

            ptree->root = new_root;
            ++ptree->layers;
        } else {
            BplusNode *parent = cur->parent;
            // get the index of cur in parent->child
            i = 0;
            while (i < parent->keynum && parent->child[i] != cur)
                ++i;
            assert(i != parent->keynum);
//            Reinsert cur node
            parent->keys[i] = node->keys[node->keynum - 1];
            parent->child[i] = node;

//            insert node
//            i = parent->keynum;
            KEY target = cur->keys[cur->keynum - 1];
//            while (i > 0 && ptree->cmp(parent->keys[i - 1], target) > 0) {
//                parent->keys[i] = parent->keys[i - 1];
//                parent->child[i] = parent->child[i - 1];
//                --i;
//            }
            for (int j = parent->keynum; j > i; --j)
            {
                parent->keys[j] = parent->keys[j - 1];
                parent->child[j] = parent->child[j - 1];
            }
            parent->keys[i] = target;
            parent->child[i] = cur;
            ++parent->keynum;
        }
        cur = cur->parent;
        ++ptree->size;
    }

//    update biggest val
    while (cur->parent) {
        // get the index of cur in parent->child
        i = 0;
        while (i < cur->parent->keynum && cur->parent->child[i] != cur)
            ++i;
        assert(i != cur->parent->keynum);

        if (ptree->cmp(cur->parent->keys[i], cur->keys[cur->keynum - 1]) < 0) {
            cur->parent->keys[i] = cur->keys[cur->keynum - 1];
            cur = cur->parent;
        } else
            break;
    }
}


/*
 * release the memory of b+tree pointed by ptree
 * */
void destroy(BplusTree * ptree)
{
    destroy_node(ptree->root);
    free(ptree);
}

