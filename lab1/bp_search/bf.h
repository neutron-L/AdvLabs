//
// Created by 20550 on 2022/9/28.
//

#ifndef BP_SEARCH_BF_H
#define BP_SEARCH_BF_H

#include <stdbool.h>

typedef unsigned int uint32_t;


struct BloomFilter
{
    long m;
    long size;
    uint32_t k;
    char * bits;
    uint32_t * seeds;
};
/*
 * params:
 * long: the number of strings which will be inserted
 * uint32_t: the number of hash function
 *
 * return: a pointer to a BloomFilter
 */
struct BloomFilter * init_bloom_filter(long, uint32_t);
void insert(struct BloomFilter *, const char *);
bool search(struct BloomFilter *, const char *);
void release(struct BloomFilter *);

#endif //BP_SEARCH_BF_H
