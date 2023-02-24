//
// Created by 20550 on 2022/9/20.
//

#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "item.h"

#define ListNodePos(T) ListNode<T> *

template <typename T>
struct ListNode
{
    T data;
    ListNodePos(T) next = nullptr;
    ListNode<T>() = default;
    explicit ListNode<T>(T & s) : data(s) {}
};


template <typename T>
class List : public Item<T> {
private:
    ListNodePos(T) header;
public:
    List() = default;
    bool search(const T & target, unsigned int &) override;

    void insert(T && str) override;

    void print() const override;

    ~List();
};


template<typename T>
bool List<T>::search(const T &target, unsigned int & times) {
    times = 1;
    ListNodePos(T) tmp = header;
    while (tmp && tmp->data != target) {
        times++;
        tmp = tmp->next;
    }
    return tmp != nullptr;
}

template<typename T>
void List<T>::insert(T &&str)
{
    if (header == nullptr)
        header = new ListNode<T>(str);
    else
    {
        auto node = new ListNode<T>(str);
        node->next = header->next;
        header->next = node;
    }
}

template<typename T>
void List<T>::print() const
{
    auto tmp = header;
    while (tmp) {
        std::cout << tmp->data << " -> ";
        tmp = tmp->next;
    }
    std::cout << "null";
}

template<typename T>
List<T>::~List() {
    auto p = header;
    decltype(p) tmp;
    while (p)
    {
        tmp = p->next;
        delete p;
        p = tmp;
    }
}




#endif //LIST_H
