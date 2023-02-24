//
// Created by 20550 on 2022/9/20.
//

#ifndef ITEM_H
#define ITEM_H

#include <string>


template <typename T>
class Item {
private:
public:
    virtual bool search(const T &, unsigned int &) = 0;
    virtual void insert(T &&) = 0;
    virtual void print() const = 0;
    virtual ~Item() = default;
};


#endif //ITEM_H
