//
// Created by 20550 on 2022/11/5.
//
#include <stdlib.h>
#include <string.h>
#include "ac.h"
#include "queue.h"

static AcNode * allocate_acnode(Automaton * pa, uint32_t size, uint32_t level);
static void deallocate_acnode(Automaton * pa,AcNode * pn, uint32_t size);
/*
 * content：A0 61 A0 A0 燼牋
 * keyword：61 A0 A0  a牋
 *
 * content：A0 61 A0 A0 61  燼牋a
 * keyword：61 A0 A0 61 a牋a
 *
 * content：A0 B0 A0 C0  牥犂
 * keyword：B0 A0        盃
 * */
static void set_dp(unsigned char ** dp, const char * str, unsigned int len);


Automaton * init_automaton(uint32_t order) {
    Automaton *pa = (Automaton *) malloc(sizeof(Automaton));
    pa->order = order;
    pa->mask_len = 0;
    pa->size = 0;
    pa->root = allocate_acnode(pa, order, 0);
    pa->len = 0;
    pa->dp = NULL;
    while ((order & 1) == 0) {
        ++pa->mask_len;
        order >>= 1;
    }

    return pa;
}

void insert_atm(Automaton * pa, const char * str, uint32_t id) {
    uint32_t start = 0;
    uint32_t bit_len = 8 * strlen(str);
    uint32_t index;
    AcNode *cur = pa->root;

    while (start < bit_len)
    {
        //        从start开始的order相应的位获取idx
        index = (str[start / 8] >> (start % 8)) & (pa->order - 1);
        if (cur->children[index] == NULL)
            cur->children[index] = allocate_acnode(pa, pa->order, cur->level + 1);
        cur=cur->children[index];
        start += pa->mask_len;
    }

    cur->id = id;
    cur->contain_key = true;
}


void set_fail_pointer(Automaton * pa)
{
    pa->root->fail = NULL;
    Queue * q = init_queue();
    push(q, pa->root);

    while (!empty_queue(q)) {
        AcNode *cur = (AcNode *) pop(q);
        for (int i = 0; i < pa->order; ++i)
        {
            if (!cur->children[i])
                continue;

            if (cur == pa->root)
                cur->children[i]->fail = cur;
            else
            {
                AcNode *p = cur->fail;
                while (p && !p->children[i])
                    p = p->fail;
                cur->children[i]->fail = p ? p->children[i] : pa->root;
                if (cur->children[i]->id == -1)
                    cur->children[i]->contain_key = cur->children[i]->fail->contain_key;
            }
            push(q, cur->children[i]);
        }
    }
}

uint32_t match(Automaton * pa, const char * str, List * plt)
{
    uint32_t start = 0;
    uint32_t index;
    uint32_t len = strlen(str);
    uint32_t bit_len = 8 * len;
    uint32_t times = 0;
    //    分词
    if (pa->len < len + 1)
    {
        pa->len = len + 1;
        free(pa->dp);
        pa->dp = (unsigned char *) malloc(pa->len * sizeof(unsigned char));
    }
    memset(pa->dp, 0, sizeof(unsigned char) * pa->len);
    set_dp(&pa->dp, str, len);

    AcNode *cur = pa->root;
    while (cur && start < bit_len)
    {
//        从start开始的order相应的位获取idx
        index = (str[start / 8] >> (start % 8)) & (pa->order - 1);
        //         不存在则通过fail指针回溯
        while (cur->children[index] == NULL && cur->fail)
            cur = cur->fail;
        start += pa->mask_len;

//         存在则说明匹配了这部分位
        if (cur->children[index])
        {
            cur = cur->children[index];

            if ((start % 8) == 0 && pa->dp[start / 8])
            {
                AcNode *tmp = cur;
                while (tmp && tmp->contain_key)
                {
                    if (tmp->id >= 0 && tmp->level * pa->mask_len % 8 == 0
                        && pa->dp[(start - tmp->level * pa->mask_len) / 8])
                    {
                        insert_lst(plt, tmp->id, (start - tmp->level * pa->mask_len) / 8);
                        ++times;
                    }
                    tmp = tmp->fail;
                }
            }
        } else
        {
            while (start <= bit_len && (start % 8 || !pa->dp[start / 8]))
                start += pa->mask_len;
        }
    }
    return times;
}

void destroy_automaton(Automaton *pa)
{
    deallocate_acnode(pa, pa->root, pa->order);
}


static void set_dp(unsigned char ** dp, const char * str, unsigned int len) {
    unsigned int i = 0;
    while (i <= len) {
        (*dp)[i] = 1;
        if ((unsigned char)str[i] > 127)
            i += 2;
        else
            ++i;
    }
}

static AcNode * allocate_acnode(Automaton * pa, uint32_t size, uint32_t level)
{
    AcNode * pn = (AcNode*) malloc(sizeof(AcNode));
    pn->fail = NULL;
    pn->children = (AcNode**) malloc(sizeof(AcNode*) * size);
    memset(pn->children, 0, sizeof(AcNode*) * size);
    pn->id = -1;
    pn->contain_key = false;
    pn->level = level;
    ++pa->size;

    return pn;
}

static void deallocate_acnode(Automaton * pa, AcNode * pn, uint32_t size)
{
    if (pn && pn->children)
    {
        --pa->size;
        for (int i = 0; i < size; ++i)
            deallocate_acnode(pa, pn->children[i], size);
        free(pn->children);
    }
    free(pn);
}
