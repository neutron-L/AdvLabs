//
// Created by 20550 on 2022/9/20.
//

#ifndef AVLTREE_H
#define AVLTREE_H

#include "item.h"

template <typename T>
struct AvlNode
{
    T data;
    AvlNode<T> * left;
    AvlNode<T> * right;
    int height{};
    AvlNode() = default;
    explicit AvlNode(T & t) : data(t), left(nullptr), right(nullptr) {}
};


template <typename T>
class AvlTree : public Item<T> {
private:
    AvlNode<T> *root;

    void singleRotateLeft(AvlNode<T> *&);

    void doubleRotateLeft(AvlNode<T> *&);

    void singleRotateRight(AvlNode<T> *&);

    void doubleRotateRight(AvlNode<T> *&);

    int getHeight(AvlNode<T> *node) { return node ? node->height : -1; }

    void insert(AvlNode<T> *&, T &);

    void freeTree(AvlNode<T> *);

public:
    AvlTree() : root(nullptr) {}

    bool search(const T &, unsigned int &) override;

    void insert(T &&) override;

    void print() const override;

    ~AvlTree();
};


template<typename T>
void AvlTree<T>::singleRotateLeft(AvlNode<T> *&node) {
    auto lc = node->left;
    node->left = lc->right;
    lc->right = node;
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    lc->height = std::max(getHeight(lc->left), getHeight(lc->right)) + 1;
    node = lc;
}

template<typename T>
void AvlTree<T>::doubleRotateLeft(AvlNode<T> *&node) {
    singleRotateRight(node->left);
    singleRotateLeft(node);
}


template<typename T>
void AvlTree<T>::singleRotateRight(AvlNode<T> *&node) {
    auto rc = node->right;
    node->right = rc->left;
    rc->left = node;
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    rc->height = std::max(getHeight(rc->left), getHeight(rc->right)) + 1;
    node = rc;
}

template<typename T>
void AvlTree<T>::doubleRotateRight(AvlNode<T> *&node) {
    singleRotateLeft(node->right);
    singleRotateRight(node);
}


template<typename T>
bool AvlTree<T>::search(const T &target, unsigned int & times) {
    times = 1;
    auto node = root;
    while (node && node->data != target)
    {
        if (node->data < target)
            node = node->right;
        else
            node = node->left;
        times++;
    }
    return node != nullptr;
}


template<typename T>
void AvlTree<T>::insert(T &&target) {
    insert(root, target);
}


template<typename T>
void AvlTree<T>::print() const {
}


template<typename T>
AvlTree<T>::~AvlTree() {
    freeTree(root);
}

template<typename T>
void AvlTree<T>::insert(AvlNode<T> *&node, T &target) {
    if (node == nullptr)
        node = new AvlNode<T>(target);
    else if (target < node->data)
    {
        insert(node->left, target);
        if (getHeight(node->left) - getHeight(node->right) == 2)
        {
            if (target < node->left->data)
                singleRotateLeft(node);
            else
                doubleRotateLeft(node);
        }
    }
    else
    {
        insert(node->right, target);
        if (getHeight(node->right) - getHeight(node->left) == 2)
        {
            if (target > node->right->data)
                singleRotateRight(node);
            else
                doubleRotateRight(node);
        }
    }
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
}


template<typename T>
void AvlTree<T>::freeTree(AvlNode<T> * node) {
    if (node)
    {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
}




#endif //AVLTREE_H
