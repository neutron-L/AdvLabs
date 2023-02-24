//
// Created by 20550 on 2022/9/22.
//

#ifndef DSL_H
#define DSL_H
#include <iostream>

#include <random>
#include "item.h"

#define SKIPLIST_P 0.5
#define MAX_LEVEL 5

template <typename T>
struct SkipNode
{
    T data{};
    unsigned int num{};
    SkipNode<T> * right{};
    SkipNode<T> * down{};
    SkipNode() = default;
    explicit SkipNode(T& e, SkipNode<T>* r = nullptr, SkipNode<T> * d = nullptr)
    : data(e), right(r), down(d) {}
};

template <typename T>
class SkipList : public Item<T> {
private:
    SkipNode<T> * header;
    int layers{};
    std::mt19937 gen{std::random_device{}()};
    std::uniform_real_distribution<double> dis;

    int randomLevel()
    {
        int lv = 1;
        /* 随机生成 lv */
        while (dis(gen) < SKIPLIST_P && lv < MAX_LEVEL) {
            lv++;
        }
//        std::cout << lv << std::endl;
        return lv;
    }
public:
    SkipList() : dis(0, 1){ header = new SkipNode<T>(); }
    bool search(const T & target, unsigned int &) override;
    void insert(T &&s);
    void print() const;
};

template<typename T>
bool SkipList<T>::search(const T & target, unsigned int & times) {
    SkipNode<T> * cur = header;
    times = 1;
    while (cur != nullptr) {
        if (cur->data == target)
            break;
        else {
            if (cur->right && cur->right->data <= target)
                cur = cur->right;
            else
                cur = cur->down;
        }
        times++;
    }
    return cur != nullptr;
}

template<typename T>
void SkipList<T>::insert(T &&s) {
    if (layers > 5)
        std::cout << layers << std::endl;
    SkipNode<T> *cur = header;
    int level = randomLevel();
    int now = layers;
    SkipNode<T> *up = nullptr;
    SkipNode<T> *topNewNode = nullptr; // 最上层的新节点，可能新节点的level大于跳表的layers
    while (cur != nullptr) {
        if (cur->data == s) {
            cur->num++;
            cur = cur->down;
        } else {
            if (cur->right && cur->right->data <= s)
                cur = cur->right;
            else if (now <= level) {
                SkipNode<T> *nex = cur->right;
                auto *node = new SkipNode<T>(s);
                node->right = nex;
                cur->right = node;
                if (up)
                    up->down = node;
                else if (!topNewNode)
                    topNewNode = node;
                up = node;
                cur = cur->down;
            }
        }
        now--;
    }

//        如果level大于layers
    int gap = level - layers;
    while (gap-- > 0) {
        auto *node = new SkipNode<T>(s, nullptr, topNewNode);
        auto *newHeader = new SkipNode<T>();
        newHeader->right = node;
        newHeader->down = header;
        header = newHeader;
        topNewNode = node;
    }
    layers = level;
}

template<typename T>
void SkipList<T>::print() const {

}



#endif //DSL_H
