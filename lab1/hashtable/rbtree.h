//
// Created by 20550 on 2022/9/20.
//

#ifndef RBTREE_H
#define RBTREE_H

#include "item.h"
#define RED 1
#define BLACK 0

template<typename T>
struct RBNode {
    T data{};
    RBNode<T> *left{};
    RBNode<T> *right{};
    RBNode<T> *parent{};
    char color{};

    RBNode() = default;

    explicit RBNode(T &d) : data(d), left(nullptr), right(nullptr), parent(nullptr) {}
};

template<typename T>
class RBTree : public Item<T> {
private:
    RBNode<T> *root{};
    RBNode<T> *nill{};

    void leftRotate(RBNode<T> *);

    void rightRotate(RBNode<T> *);

    void insertFixup(RBNode<T> *);

    void freeTree(RBNode<T> *);

public:
    RBTree() {
        nill = new RBNode<T>();
        nill->color = BLACK;
        root = nill;
    }

    bool search(const T &, unsigned int &);

    void insert(T &&target);

    void print() const;

    ~RBTree();
};

template<typename T>
bool RBTree<T>::search(const T &target, unsigned int & times) {
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
void RBTree<T>::insert(T && target) {
    RBNode<T> * y = nill;
    RBNode<T> * x = root;
    auto * z = new RBNode<T>(target);
    while (x != nill)
    {
        y = x;
        if (target < x->data)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == nill)
        root = z;
    else if (target < y->data)
        y->left = z;
    else
        y->right = z;
    z->left = nill;
    z->right = nill;
    z->color = RED;
    insertFixup(z);
}

template<typename T>
void RBTree<T>::print() const {

}

template<typename T>
void RBTree<T>::leftRotate(RBNode<T> * x) {
    auto y = x->right;
    x->right = y->left;
    if (y->left != nill)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nill)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

template<typename T>
void RBTree<T>::rightRotate(RBNode<T> * x) {
    auto y = x->left;
    x->left = y->right;
    if (y->right != nill)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nill)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

template<typename T>
void RBTree<T>::insertFixup(RBNode<T> * z) {
    while (z != root && z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            auto y = z->parent->parent->right;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        else
        {
            auto y = z->parent->parent->left;
            if (y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

template<typename T>
RBTree<T>::~RBTree() {
    freeTree(root);
    delete nill;
}

template<typename T>
void RBTree<T>::freeTree(RBNode<T> * node) {
    if (node != nill)
    {
        freeTree(node->left);
        freeTree(node->right);
        delete node;
    }
}


#endif //RBTREE_H
