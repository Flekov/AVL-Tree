#pragma once

#include <iostream>
#include <string>
#include <vector>

template <typename T>
class AVLTree
{
public:
    struct node
    {
        T value;
        node *left, *right, *parent;
        int bf;
    };

    AVLTree();
    AVLTree(std::vector<T> values);
    AVLTree(const AVLTree &t);
    AVLTree &operator=(const AVLTree &t);

    void print();
    void insert(T _value);
    void remove(T _value);

    // два search-а с различни с резултати, защото не се рабира
    // от условието кой трябва да е
    const node *search(T _value) const;
    bool member(T _value) const;

    ~AVLTree();

private:
    void print(const std::string &prefix, const node *curr, bool isLeft);

    // две помощни функции за insert, заради родителя
    //(който най-вероятно не е нужен)
    node *insert(T _value, node *curr);
    node *insertWithParent(T _value, node *curr, node *parent);

    node *remove(T _value, node *&curr);
    int leftMost(node *curr);

    node *rebalance(node *parent);
    node *rotateLeft(node *parent, node *right);
    node *rotateRight(node *parent, node *left);
    int height(const node *curr);
    int balanceFactor(const node *curr);

    const node *search(T _value, const node *curr) const;
    bool member(T _value, const node *curr) const;

    void clear(node *&curr);

    node *root;

    // за copy-конструктор и оператор =
    std::vector<T> values;
};