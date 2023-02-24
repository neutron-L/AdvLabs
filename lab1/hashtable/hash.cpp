//
// Created by 20550 on 2022/9/20.
//

#include "hash.h"

unsigned int RSHash(char* str, unsigned int len)
{
    unsigned int b    = 378551;
    unsigned int a    = 63689;
    unsigned int hash = 0;

    for(int i = 0; i < len; str++, i++)
    {
        hash = hash * a + (*str);
        a    = a * b;
    }
    return hash;
}

unsigned int JSHash (char* str, unsigned int len)
{
    unsigned int hash=1315423911;
    for(int i=0;i<len;str++, i++){
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }
    return hash;
}

unsigned int PJWHash(char *str, unsigned int len)
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt
                                                           - OneEighth);
    unsigned int hash    = 0;
    unsigned int test    = 0;

    for (int i = 0; i < len; str++, i++)
    {
        hash = (hash << OneEighth) + (*str);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

unsigned int ELFHash(char *str, unsigned int len) {
    unsigned int hash = 0;
    unsigned int x = 0;
    for (int i = 0; i < len; str++, i++) {
        hash = (hash << 4) + (*str);//hash左移4位，把当前字符ASCII存入hash低四位。
        if ((x = hash & 0xF0000000L) != 0) {
//如果最高的四位不为0，则说明字符多余7个，现在正在存第7个字符，如果不处理，再加下一个字符时，第一个字符会被移出，因此要有如下处理。
//该处理，如果最高位为0，就会仅仅影响5-8位，否则会影响5-31位，因为C语言使用的算数移位
//因为1-4位刚刚存储了新加入到字符，所以不能>>28
            hash ^= (x >> 24);
//上面这行代码并不会对X有影响，本身X和hash的高4位相同，下面这行代码&~即对28-31(高4位)位清零。
            hash &= ~x;
        }
    }
//返回一个符号位为0的数，即丢弃最高位，以免函数外产生影响。(我们可以考虑，如果只有字符，符号位不可能为负)
    return hash;
}

unsigned int BKDRHash(char* str, unsigned int len)
{
    unsigned int seed = 131;
    unsigned hash = 0;
    for (int i = 0; i < len; str++, i++)
        hash = hash * seed + (*str);
    return hash;
}

unsigned int SDBMHash(char* str, unsigned int len) {
    unsigned int hash = 0;
    for (int i = 0; i < len; str++, i++) {
        hash = hash * 65599 + (*str);
    }
    return hash;
}

unsigned int DJBHash (char* str, unsigned int len) {
    unsigned int hash = 5381;
    for (int i = 0; i < len; str++, i++) {
        hash = ((hash << 5) + hash) + (*str);
    }
    return hash;
}

unsigned int DEKHash(char* str, unsigned int len) {
    unsigned int hash = len;
    for (int i = 0; i < len; i++, str++) {
        hash = (hash << 5) ^ (hash >> 27) ^ (*str);
    }
    return hash;
}

unsigned int BPHash (char* str, unsigned int len) {
    unsigned int hash = len;

    for (int i = 0; i < len; str++, i++) {
        hash = (hash << 7) ^ ((int)(*str));
    }

    return hash;
}

unsigned int FNVHash (char* str, unsigned int len)
{
    unsigned int fnvprime = 0x811C9DC5;
    unsigned int hash = 0;

    for (int i = 0; i < len; str++, i++) {
        hash *= fnvprime;
        hash ^= (int)*str;
    }

    return hash;
}

unsigned int APHash (char* str, unsigned int len)
{
    unsigned int hash = 0;

    for (int i = 0; i < len; str++, i++) {
        if ((i & 1) == 0) {
            hash ^= (hash << 7) ^ (*str) ^ (hash >> 3);
        } else {
            hash ^= ~((hash << 11) ^ (*str) ^ (hash >> 5));
        }
    }

    return hash;
}

