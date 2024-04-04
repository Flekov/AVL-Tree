#include "avltree.h"

#include <iostream>
#include <string>
#include <vector>

// функция за приниране от упражнения
template <typename T>
void AVLTree<T>::print(const std::string &prefix, const AVLTree<T>::node *curr, bool isLeft)
{
    if (curr != nullptr)
    {
        std::cout << prefix;
        std::cout << (isLeft ? "├──" : "└──");
        std::cout << curr->value << std::endl;

        print(prefix + (isLeft ? "│   " : "    "), curr->left, true);
        print(prefix + (isLeft ? "│   " : "    "), curr->right, false);
    }
}

template <typename T>
void AVLTree<T>::print()
{
    print("", root, false);
}

template <typename T>
AVLTree<T>::AVLTree() : root(nullptr) {}

template <typename T>
AVLTree<T>::AVLTree(std::vector<T> values) : root(nullptr)
{
    for (T v : values)
    {
        insert(v);
    }
}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree<T> &t) : AVLTree(t.values) {}

template <typename T>
AVLTree<T> &AVLTree<T>::operator=(const AVLTree<T> &t)
{
    for (T v : t.values)
    {
        insert(v);
    }
    return *this;
}

// стандартен insert за двоични дървета с добавена самобалансираща функция
template <typename T>
void AVLTree<T>::insert(T _value)
{
    root = insert(_value, root);
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::insert(T _value, AVLTree<T>::node *curr)
{
    if (curr == nullptr)
    {
        curr = new typename AVLTree<T>::node{_value, nullptr, nullptr, nullptr, 0};
        values.push_back(_value);
        return curr;
    }

    if (_value < curr->value)
    {
        curr->left = insertWithParent(_value, curr->left, curr);
    }
    else
    {
        curr->right = insertWithParent(_value, curr->right, curr);
    }

    curr->bf = balanceFactor(curr);
    curr = rebalance(curr);
    return curr;
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::insertWithParent(T _value, AVLTree<T>::node *curr, AVLTree<T>::node *parent)
{
    if (curr == nullptr)
    {
        curr = new typename AVLTree<T>::node{_value, nullptr, nullptr, parent, 0};
        values.push_back(_value);
        return curr;
    }

    if (_value < curr->value)
    {
        curr->left = insertWithParent(_value, curr->left, curr);
    }
    else
    {
        curr->right = insertWithParent(_value, curr->right, curr);
    }

    curr->bf = balanceFactor(curr);
    curr = rebalance(curr);
    return curr;
}

// отново стандадртен remove за двоични дървета с добавена самобалансираща функция
template <typename T>
void AVLTree<T>::remove(T _value)
{
    root = remove(_value, root);
}

template <typename T>
typename AVLTree<T>::node *AVLTree<T>::remove(T _value, AVLTree<T>::node *&curr)
{
    if (curr == nullptr)
    {
        return curr;
    }

    if (curr->value == _value)
    {
        if (curr->right == nullptr)
        {
            node *left = curr->left;
            if (curr->left != nullptr)
            {
                curr->left->parent = curr->parent;
            }
            delete curr;
            curr = left;
            return curr;
        }
        else
        {
            T replacement = leftMost(curr->right);
            curr->value = replacement;
            remove(replacement, curr->right);
        }
    }

    if (_value < curr->value)
    {
        remove(_value, curr->left);
    }
    else
    {
        remove(_value, curr->right);
    }

    curr->bf = balanceFactor(curr);
    curr = rebalance(curr);
    return curr;
}

template <typename T>
int AVLTree<T>::leftMost(AVLTree<T>::node *curr)
{
    while (curr->left != nullptr)
    {
        curr = curr->left;
    }

    return curr->value;
}

// балансираща функция, ако има нужда
// целият алгоритъм е от линка за AVL tree в домашното
template <typename T>
typename AVLTree<T>::node *AVLTree<T>::rebalance(AVLTree<T>::node *curr)
{
    if (curr->bf > 1)
    {
        if (curr->right->bf >= 0)
        {
            // right right случай
            curr = rotateLeft(curr, curr->right);
            return curr;
        }
        else
        {
            // right left случай
            curr->right = rotateRight(curr->right, curr->right->left);
            curr = rotateLeft(curr, curr->right);
            return curr;
        }
    }

    else if (curr->bf < -1)
    {
        if (curr->left->bf <= 0)
        {
            // left left случай
            curr = rotateRight(curr, curr->left);
            return curr;
        }
        else
        {
            // left right случай
            curr->left = rotateLeft(curr->left, curr->left->right);
            curr = rotateRight(curr, curr->left);
            return curr;
        }
    }

    else
    {
        return curr;
    }
}

// ляво завъртане
template <typename T>
typename AVLTree<T>::node *AVLTree<T>::rotateLeft(node *parent, node *right)
{
    node *t23 = right->left;
    parent->right = t23;

    if (t23 != nullptr)
    {
        t23->parent = parent;
    }

    right->left = parent;
    parent->parent = right;

    if (right->bf == 0)
    {
        parent->bf = 1;
        right->bf = -1;
    }
    else
    {
        parent->bf = 0;
        right->bf = 0;
    }

    return right;
}

// дясно завърнате
template <typename T>
typename AVLTree<T>::node *AVLTree<T>::rotateRight(node *parent, node *left)
{
    node *t23 = left->right;
    parent->left = t23;

    if (t23 != nullptr)
    {
        t23->parent = parent;
    }

    left->right = parent;
    parent->parent = left;

    if (left->bf == 0)
    {
        parent->bf = 1;
        left->bf = -1;
    }
    else
    {
        parent->bf = 0;
        left->bf = 0;
    }

    return left;
}

int max(int a, int b)
{
    if (a >= b)
        return a;
    else
        return b;
}

// височина на дърво
template <typename T>
int AVLTree<T>::height(const AVLTree<T>::node *curr)
{
    if (curr == nullptr)
    {
        return 0;
    }

    return 1 + max(height(curr->left), height(curr->right));
}

// балансиращия фактор нужен за AVL дърво
template <typename T>
int AVLTree<T>::balanceFactor(const AVLTree<T>::node *curr)
{
    return height(curr->right) - height(curr->left);
}

// search, който връща самия връх, ако има такъв
template <typename T>
const typename AVLTree<T>::node *AVLTree<T>::search(T _value) const
{
    return search(_value, root);
}

template <typename T>
const typename AVLTree<T>::node *AVLTree<T>::search(T _value, const AVLTree<T>::node *curr) const
{
    if (curr == nullptr || curr->value == _value)
    {
        return curr;
    }

    if (_value < curr->value)
    {
        return search(_value, curr->left);
    }
    else
    {
        return search(_value, curr->right);
    }
}

// search, който проверява дали се съдържа дадена стойност
template <typename T>
bool AVLTree<T>::member(T _value) const
{
    return member(_value, root);
}

template <typename T>
bool AVLTree<T>::member(T _value, const AVLTree<T>::node *curr) const
{
    if (curr == nullptr)
    {
        return false;
    }

    if (curr->value == _value)
    {
        return true;
    }

    if (_value < curr->value)
    {
        return search(_value, curr->left);
    }
    else
    {
        return search(_value, curr->right);
    }
}

// clear за деструктор
template <typename T>
void AVLTree<T>::clear(AVLTree<T>::node *&curr)
{
    if (curr != nullptr)
    {
        clear(curr->left);
        clear(curr->right);
        delete curr;
    }
}

template <typename T>
AVLTree<T>::~AVLTree()
{
    clear(root);
}