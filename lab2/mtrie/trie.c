//
// Created by 20550 on 2022/10/11.
//

#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* 局部函数声明 */
/* allocate an uncompressed node */
static TrieNode * allocate_node(TrieTree *, unsigned int);
/* destroy an uncompressed node */
static void destroy_node(TrieNode*, unsigned int);

/* allocate a compressed node */
static CTrieNode * allocate_cnode(TrieTree *, unsigned int);
/* destroy a compressed node */
static void destroy_cnode(CTrieNode*, unsigned int);
/* returns the number of bits matching the bit segment of the specified string and the specified node */
static unsigned int match_bits(CTrieNode *, const char *, unsigned int, unsigned int);
/* gets the index of the specified string from the specified length of the specified positioning index */
static unsigned int get_index(const char *, unsigned int, unsigned int, unsigned int);
/* Split String Bitwise */
static void split_string(char **, char **, unsigned int, unsigned int, bool);

/* uncompressed trie tree search */
static bool search_nonc(TrieTree *, const char *, unsigned int *);
/* uncompressed trie tree insert */
static void insert_nonc(TrieTree *, char *);

/* compressed trie tree search */
static bool search_c(TrieTree *, const char *, unsigned int *);
/* compressed trie tree insert */
static void insert_c(TrieTree *, char *);


// 分配一个前缀树节点 order是该节点孩子最大个数
static TrieNode * allocate_node(TrieTree * ptree, unsigned int order)
{
    TrieNode * node = (TrieNode *)malloc(sizeof(TrieNode));
    node->children = (TrieNode**) malloc(order * sizeof(TrieNode*));
    node->is_key = false;
    memset(node->children, 0, sizeof(TrieNode*) * order);

#if (DEBUG)
    for (int i = 0; i < order; i++)
        assert(node->children[i] == NULL);
#endif
    ++ptree->size;

    return node;
}

// 递归释放一个前缀树节点所占的内存空间 包括释放孩子指针数组
static void destroy_node(TrieNode* node, unsigned int size)
{
    if (node)
    {
        for (unsigned int i = 0; i < size; ++i)
            destroy_node(node->children[i], size);
        free(node->children);
        free(node);
    }
}


// 分配一个压缩前缀树节点 order是节点孩子最大个数
static CTrieNode * allocate_cnode(TrieTree * ptree, unsigned int order)
{
    CTrieNode * node = (CTrieNode *)malloc(sizeof(CTrieNode));
    node->children = (CTrieNode**) malloc(order * sizeof(CTrieNode*));
    node->is_key = false;
    node->data = NULL;
    node->len = 0;
    ++ptree->size;

    memset(node->children, 0, sizeof(CTrieNode*) * order);

    return node;
}

static void destroy_cnode(CTrieNode* node, unsigned int size)
{
    if (node)
    {
        for (unsigned int i = 0; i < size; ++i)
            destroy_cnode(node->children[i], size);
        free(node->children);
        free(node->data);
        free(node);
    }
}

/*
 * node：一个压缩前缀树节点
 * target：待匹配字符串
 * start：target中开始匹配的位索引
 * return：匹配的位数
 * */
static unsigned int match_bits(CTrieNode * node, const char * target, unsigned int start, unsigned int len)
{
    unsigned int res;
    unsigned int match_len = 0;
    unsigned int i, j;

    if ((res = start % 8) != 0)
    {
        unsigned int res1 = res;
        unsigned int res2 = 8 - res;
        unsigned int l = 8;
        for (i = 0, j = start / 8; i * 8 < node->len && j < len; ++i, ++j)
        {
            unsigned char x = *(node->data + i);
            if (i == node->len / 8)
            {
                unsigned int r = node->len % 8;
                x &= (1 << r) - 1;
                l = MIN(l, r);
            }
            unsigned char y = (*(target + j) >> res1) & ((1 << res2) - 1);
            if (j < len - 1)
                y |= (*(target + j + 1) & ((1 << res1) - 1)) << res2;
            else
                l = MIN(l, res2);

            if (x == y)
                match_len += l;
            else
            {
                x ^= y;
                while (l-- && (x & 1) == 0)
                {
                    match_len++;
                    x >>= 1;
                }
                break;
            }
        }
    }
    else
    {
        for (i = 0, j = start / 8; i < node->len / 8 && j < len && *(node->data + i) == *(target + j); ++i, ++j)
            match_len += 8;
        if (j < len)
        {
            //            有一个字节未完全匹配
            if (i < node->len / 8)
            {
                unsigned char x = *(node->data + i) ^ *(target + j);
                while ((x & 1) == 0)
                {
                    ++match_len;
                    x >>= 1;
                }
            }
            else if (i * 8 < node->len)
            {
                unsigned int l = node->len % 8;
                unsigned char x = (*(node->data + i) & ((1 << l) - 1)) ^ *(target + j);
                while (l-- && (x & 1) == 0)
                {
                    ++match_len;
                    x >>= 1;
                }
            }
        }
    }

    return match_len;
}


/*
 * target：字符串
 * start：target开始的位数索引，不能超过字符串大小len
 * match_len：作为返回值的位数长度 start+match_len可以大于len
 * len：target长度
 * */
static unsigned int get_index(const char * target, unsigned int start, unsigned int match_len, unsigned int len) {
    unsigned char i = 0;
    unsigned int mask = (1 << match_len) - 1;
    unsigned int res;
    if ((res = start % 8) != 0)
    {
        unsigned char x = *(target + start / 8);
        x >>= res;
        i |= x;
        if (start / 8 + 1 < len)
        {
            x = *(target + start / 8 + 1) & ((1 << res) - 1);
            i |= x << (8 - res);
        }
    }
    else
        i = *(target + start / 8);

    return (i & mask);
}


/*
 * str：原字符串
 * pr：存储分裂后的后半部分字符串
 * mid：开始分裂的位的索引
 * right：结束分裂的位的索引
 * is_const：是否可以为*str重新分配空间存储前半段字符串，以节省内存
 * */
static void split_string(char ** str, char ** pr, unsigned int mid, unsigned int right, bool is_const) {
    assert(pr);
    *pr = (char *) calloc((right - mid + 7) / 8 + 1, sizeof(char));
    char *p = *str;
    char *q = *pr;
    unsigned int i, j;
    if (mid % 8) {
        unsigned int res1 = mid % 8;
        unsigned int res2 = 8 - res1;
        for (i = mid / 8, j = 0; i < right / 8 || i * 8 < right; ++i, ++j) {
            unsigned char x = (p[i] >> res1) & ((1 << res2) - 1);
            if ((i + 1) * 8 < right) {
                unsigned char y = (p[i + 1] & ((1 << res1) - 1));
                if ((i + 1) == right / 8 && right % 8 < res1)
                    y &= (1 << (right % 8)) - 1;
                y <<= res2;
                x |= y;
            }

            q[j] = x;
        }
    } else {
        for (i = mid / 8, j = 0; i * 8 < right; ++i, ++j)
            q[j] = p[i];
    }

    if (!is_const)
    {
        unsigned int len = (mid + 7) / 8;
        char * new_s = (char *)malloc(sizeof(char) *(len + 1));
        strncpy(new_s, *str, len);
        new_s[len] = '\0';
        free(*str);
        *str = new_s;
    }
}

TrieTree * init_trie(unsigned int order, bool is_compress) {
    TrieTree *tree = (TrieTree *) malloc(sizeof(TrieTree));
    tree->is_compress = is_compress;
    if (is_compress)
        tree->root.ctroot = allocate_cnode(tree, order);
    else
        tree->root.troot = allocate_node(tree, order);
    tree->mask_len = 0;
    while ((order & 1) == 0) {
        ++tree->mask_len;
        order >>= 1;
    }

    return tree;
}


bool search(TrieTree * ptree, const char * target, unsigned int * times) {
    if (ptree->is_compress)
        return search_c(ptree, target, times);
    else
        return search_nonc(ptree, target, times);
}



void insert(TrieTree * ptree, char * target) {
    if (ptree->is_compress)
        insert_c(ptree, target);
    else
        insert_nonc(ptree, target);
}


static bool search_nonc(TrieTree * ptree, const char * target, unsigned int * times)
{
    TrieNode *cur = ptree->root.troot;
    unsigned int mask = (1 << ptree->mask_len) - 1;
    unsigned int bits = 0; // 需要右移的位数
    const char * p = target;
    unsigned int len = strlen(target);
    unsigned char data = *p;
    unsigned int idx;

    while (cur) {
        ++(*times);

        idx = (data >> bits) & mask;
        bits += ptree->mask_len;
        cur = cur->children[idx];

        if (bits == 8 * sizeof(char))
        {
            --len;
            if (!len)
                return cur && cur->is_key;
            data = *++p;
            bits = 0;
        }
    }

    return false;
}

static void insert_nonc(TrieTree * ptree, char * target)
{
    TrieNode *cur = ptree->root.troot;
    unsigned int mask = (1 << ptree->mask_len) - 1;
    unsigned int bits = 0; // 需要右移的位数
    const char *p = target;
    unsigned int len = strlen(target);
    unsigned char data = *p;
    unsigned int idx;

    while (true) {
        idx = (data >> bits) & mask;
        bits += ptree->mask_len;

        if (!cur->children[idx])
            cur->children[idx] = allocate_node(ptree, 1 << ptree->mask_len);
        cur = cur->children[idx];

        if (bits == 8 * sizeof(char))
        {
            --len;
            if (!len)
                break;
            data = *++p;
            bits = 0;
        }
    }
    cur->is_key = true;
}


static bool search_c(TrieTree *ptree, const char *target, unsigned int *times) {
    CTrieNode *cur = ptree->root.ctroot;
    unsigned int start = 0;
    unsigned int idx;
    unsigned int len = strlen(target);
    unsigned int bits_len = len * 8;
    while (start < bits_len) {
        ++(*times);

        idx = get_index(target, start, ptree->mask_len, len);
        cur = cur->children[idx];

        if (!cur || match_bits(cur, target, start, len) != cur->len)
            return false;
        else
//          计算target从现在位置+cur->len开始的ptree->mask_len长度的位对应的索引
            start += cur->len;
    }

    return cur->is_key;
}

static void insert_c(TrieTree *ptree, char *target) {
    CTrieNode *cur = ptree->root.ctroot;
    unsigned int start = 0;
    unsigned int idx;
    unsigned int len = strlen(target);
    unsigned int bits_len = len * 8;
    unsigned int match_len;

    while (start < bits_len) {
        idx = get_index(target, start, ptree->mask_len, len);
        if (!cur->children[idx]) {
            cur->children[idx] = allocate_cnode(ptree, 1 << ptree->mask_len);
            cur = cur->children[idx];
            split_string(&target, &cur->data, start, bits_len, true);
            cur->len = bits_len - start;
            cur->is_key = true;
            start = bits_len;
        } else {
            cur = cur->children[idx];
//        split node
            if ((match_len = match_bits(cur, target, start, len)) != cur->len) {
//                match_len向下取整保持可以被ptree->mask_len整除
                match_len = match_len / ptree->mask_len * ptree->mask_len;
                char *right;

                split_string(&cur->data, &right, match_len, cur->len, false);
                CTrieNode *node = allocate_cnode(ptree, 1 << ptree->mask_len);
                node->data = right;
                node->len = cur->len - match_len;
                node->is_key = cur->is_key;
                cur->is_key = false;
                cur->len = match_len;

//            swap node->children and cur->children 把cur的孩子过继给新的node node作为cur的孩子节点之一 要维持cur和cur父亲的关系
                CTrieNode **tmp = node->children;
                node->children = cur->children;
                cur->children = tmp;

//            node作为cur的一个孩子
                idx = get_index(node->data, 0, ptree->mask_len, strlen(node->data));
                cur->children[idx] = node;


//            新建一个叶子节点保存target剩余的位数
                start += match_len;
//            如果target有剩余的未匹配的位
                if (start < bits_len) {
                    node = allocate_cnode(ptree, 1 << ptree->mask_len);
                    split_string(&target, &right, start, bits_len, true);
                    node->data = right;
                    node->len = bits_len - start;
                    node->is_key = true;

                    idx = get_index(node->data, 0, ptree->mask_len, strlen(node->data));
                    cur->children[idx] = node;

                    start = bits_len;
                } else
                    cur->is_key = true;
            } else {
//          计算target从现在位置+cur->len开始的ptree->mask_len长度的位对应的索引
                start += cur->len;
                if (start == bits_len)
                    cur->is_key = true;
            }
        }
    }
}

void destroy(TrieTree * ptree) {
    if (ptree->is_compress)
        destroy_cnode(ptree->root.ctroot, 1 << ptree->mask_len);
    else
        destroy_node(ptree->root.troot, 1 << ptree->mask_len);
    free(ptree);
}
