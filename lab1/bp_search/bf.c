//
// Created by 20550 on 2022/9/28.
//

#include "bf.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

static uint32_t MurmurHash2 ( const void * key, unsigned int len, uint32_t seed )
{
    /* 'm' and 'r' are mixing constants generated offline.
       They're not really 'magic', they just happen to work well.  */

    const uint32_t m = 0x5bd1e995;
    const int r = 24;

    /* Initialize the hash to a 'random' value */

    uint32_t h = seed ^ len;

    /* Mix 4 bytes at a time into the hash */

    const unsigned char * data = (const unsigned char *)key;

    while(len >= 4)
    {
        uint32_t k = *(uint32_t*)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    /* Handle the last few bytes of the input array  */

    switch(len) {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    }

    /* Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.  */

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

struct BloomFilter * init_bloom_filter(long m, uint32_t k)
{
    struct BloomFilter * bloomFilter = (struct BloomFilter*)malloc(sizeof(struct BloomFilter));
    bloomFilter->m = m;
    bloomFilter->size = (m + 7) / 8;
    bloomFilter->k = k;
    bloomFilter->bits =  (char *) calloc(bloomFilter->size, sizeof(char));
    bloomFilter->seeds = (uint32_t*) malloc(sizeof(int) * bloomFilter->k);

    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < bloomFilter->k; i++)
        bloomFilter->seeds[i] = rand();

    return bloomFilter;
}


void insert(struct BloomFilter * bloomFilter, const char * str)
{
    uint32_t k = bloomFilter->k;
    size_t len = strlen(str);
    for (int i = 0; i < k; i++) {
        uint32_t idx = MurmurHash2(str, len, bloomFilter->seeds[i]) % bloomFilter->m;
        bloomFilter->bits[idx / 8] |= (1 << (idx % 8));
    }
}


bool search(struct BloomFilter * bloomFilter, const char * str)
{
    uint32_t k = bloomFilter->k;
    bool ans = true;
    size_t len = strlen(str);
    for (int i = 0; i < k && ans; i++) {
        uint32_t idx = MurmurHash2(str, len, bloomFilter->seeds[i]) % bloomFilter->m;
        unsigned char item = bloomFilter->bits[idx / 8];
        uint32_t offset = idx % 8;
        ans &= (item & (1<<offset)) >> offset;
    }
    return ans;
}

void release(struct BloomFilter * bloomFilter)
{
    free(bloomFilter->bits);
    free(bloomFilter->seeds);
    free(bloomFilter);
}