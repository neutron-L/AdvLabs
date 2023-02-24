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
        hash = (hash << 4) + (*str);//hash����4λ���ѵ�ǰ�ַ�ASCII����hash����λ��
        if ((x = hash & 0xF0000000L) != 0) {
//�����ߵ���λ��Ϊ0����˵���ַ�����7�����������ڴ��7���ַ�������������ټ���һ���ַ�ʱ����һ���ַ��ᱻ�Ƴ������Ҫ�����´���
//�ô���������λΪ0���ͻ����Ӱ��5-8λ�������Ӱ��5-31λ����ΪC����ʹ�õ�������λ
//��Ϊ1-4λ�ոմ洢���¼��뵽�ַ������Բ���>>28
            hash ^= (x >> 24);
//�������д��벢�����X��Ӱ�죬����X��hash�ĸ�4λ��ͬ���������д���&~����28-31(��4λ)λ���㡣
            hash &= ~x;
        }
    }
//����һ������λΪ0���������������λ�����⺯�������Ӱ�졣(���ǿ��Կ��ǣ����ֻ���ַ�������λ������Ϊ��)
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

