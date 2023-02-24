//
// Created by 20550 on 2022/9/20.
//

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include "item.h"
#include "list.h"
#include "avltree.h"
#include "rbtree.h"
#include "dsl.h"
#include "hash.h"



template <typename T>
class HashTable {
private:
    enum TYPE {LIST, AVL, RBT, SKL};
    enum ALGORITHM {RS, JS, PJW, ELF, BKDR, SDBM, DJB, DEK, BP, FNV, AP};


    Item<T> ** table;
    unsigned int size;
    unsigned int hash_func_idx{};
    TYPE item_type{};
    using hash_func = unsigned int (*)(char*, unsigned int);
    hash_func hashes[11] = {RSHash, JSHash, PJWHash, ELFHash, BKDRHash,
                          SDBMHash, DJBHash, DEKHash, BPHash, FNVHash, APHash };
    unsigned int hash(const std::string & s)
    {
        return hashes[hash_func_idx]((char *)s.c_str(), s.length());
    }
public:
    // ָ����ϣ���С����ѡ��Ĺ�ϣ�����������ַ��ͻ�����ݽṹ
    explicit HashTable(unsigned int sz, unsigned int idx = 0, unsigned int type = 0) :
    size(sz), hash_func_idx(idx), item_type(static_cast<TYPE>(type))
    {
        table = new Item<T>*[size]();
    }
    // ����Ԫ�ز������Ҵ�������times�������ҵ�����true���򷵻�false
    bool search(const T &, unsigned int &);
    // ����Ԫ��
    void insert(T &);
    // ��ӡ��ϣ������debug��
    void printTable() const;

    ~HashTable();
};



template<typename T>
void HashTable<T>::printTable() const {
    for (int i = 0; i < size; i++) {
        if (table[i])
            table[i]->print();
        else
            std::cout << "null";
        std::cout << std::endl;
    }
}

template<typename T>
void HashTable<T>::insert(T &target) {
    int idx = hash(target) % size;
    if (table[idx] == nullptr) {
        switch (item_type) {
            case 0:
                table[idx] = new List<T>();
                break;
            case 1:
                table[idx] = new AvlTree<T>();
                break;
            case 2:
                table[idx] = new RBTree<T>();
                break;
            case 3:
                table[idx] = new SkipList<T>();
                break;
            default:
                table[idx] = new SkipList<T>();
                break;
        }
    }
    table[idx]->insert(std::move(target));
}

template<typename T>
bool HashTable<T>::search(const T &target, unsigned int & times) {
    unsigned int idx = hash(target) % size;
    if (table[idx] == nullptr)
        return false;
    else
        return table[idx]->search(target, times);
}

template<typename T>
HashTable<T>::~HashTable() {
    for (int i = 0; i < size; i++)
        if (table[i])
            delete table[i];
    delete [] table;
}





#endif //HASHTABLE_H
