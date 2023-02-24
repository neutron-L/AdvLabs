#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "bf.h"
#define DICT_SIZE 1034189
//#define STRING_NUM 123417
#define BUFFER_SIZE 256
#define ANS 49425
typedef unsigned int uint32_t;



int main() {
    double e = exp(1.0);
    char buffer[BUFFER_SIZE];
    double fp;
    long m;
    uint32_t k;
    printf("input the false rate fp:");
    scanf("%s", buffer);
    fp = strtod(buffer, NULL);
    m = DICT_SIZE * log2(e) * log2(1.0 / fp);
    k = (uint32_t) (log(2.0) * m / DICT_SIZE);
    printf("m: %ld k: %d\n", m, k);

//    init bloom filter
    struct BloomFilter *bloomFilter = init_bloom_filter(m, k);

//    read dict.txt
    FILE *fr, *fw;
    fr = fopen("dict.txt", "r");

    while (fgets(buffer, BUFFER_SIZE, fr)) {
        assert(buffer[strlen(buffer) - 1] == '\n');
        buffer[strlen(buffer) - 1] = '\0';
//        puts(buffer);
        insert(bloomFilter, buffer);
    }
    fclose(fr);

//    read string.txt
    fr = fopen("string.txt", "r");
    fw = fopen("result.txt", "w");

    unsigned int hits = 0;
    while (fgets(buffer, BUFFER_SIZE, fr)) {
        assert(buffer[strlen(buffer) - 1] == '\n');
        buffer[strlen(buffer) - 1] = '\0';
        if (search(bloomFilter, buffer))
        {
            fputs(buffer, fw);
            fputs("\n", fw);
            hits++;
        }
//        puts(buffer);
    }

    fclose(fr);
    fclose(fw);
    release(bloomFilter);
    printf("Expected hits: %d\n", ANS);
    printf("Actual hits: %d\n", hits);
    printf("False rate: %f\n", 1.0 * abs((int)hits - ANS) / ANS);

    return 0;
}
