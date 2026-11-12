//
// Created by Lenovo on 2.2.2026 г..
//

#include <iostream>
#include <vector>
#include <list>

// ZADA4A 1
template <typename T>
struct TNode
{
    T data;
    TNode<T>* left;
    TNode<T>* right;

    TNode(const T& data, TNode<T>* left = nullptr, TNode<T>* right = nullptr) : data(data), left(left), right(right)
    {
    }
};

template <typename T>
void deallocate(TNode<T>*& root)
{
    if (!root)
        return;

    deallocate(root->left);
    deallocate(root->right);

    delete root;
}

template <typename T>
void print_tree(const TNode<T>* root)
{
    if (!root)
        return;

    std::cout << root->data << ' ';
    print_tree(root->left);
    print_tree(root->right);
}

template <typename T>
bool isLeaf(const TNode<T>* node)
{
    if (!node)
        return false;

    if (!node->left && !node->right)
        return true;

    return false;
}

template <typename T>
bool isBigLeaf(const TNode<T>* node)
{
    if (!node)
        return false;

    if (!node->left && node->right && isLeaf(node->right))
        return true;

    if (!node->right && node->left && isLeaf(node->left))
        return true;

    return false;
}

template <typename T>
void removeSubtree(TNode<T>*& root, const std::string& side)
{
    if (!root)
        return;

    if (side == "left")
    {
        TNode<T>* toDelete = root->left;
        root->left = nullptr;
        deallocate(toDelete);
    }
    else if (side == "right")
    {
        TNode<T>* toDelete = root->right;
        root->right = nullptr;
        deallocate(toDelete);
    }
    else
        std::cout << "INVALID" << std::endl;
}

template <typename T>
void removeBigLeafs(TNode<T>*& root)
{
    if (!root)
        return;

    if (isBigLeaf(root))
        deallocate(root);

    if (isBigLeaf(root->left))
        removeSubtree(root, "left");

    if (isBigLeaf(root->right))
        removeSubtree(root, "right");

    removeBigLeafs(root->left);
    removeBigLeafs(root->right);

    if (isBigLeaf(root->left))
        removeSubtree(root, "left");

    if (isBigLeaf(root->right))
        removeSubtree(root, "right");
}

// ZADA4A 2
bool hasWayHelper(std::vector<std::list<int>>& g, const int start, const int to, std::vector<bool>& visited, int steps)
{
    if (start == to)
        return true;

    if (visited[start])
        return false;

    visited[start] = true;

    for (const int node : g[start])
    {
        if (node == to && steps == 0)
            continue;

        if (!visited[node])
            if (hasWayHelper(g, node, to, visited, ++steps))
                return true;
    }

    return false;
}

bool hasWay(std::vector<std::list<int>>& g, const std::pair<int, int>& firstTrans)
{
    // g[firstTrans.second].remove(firstTrans.first);
    std::vector visited(g.size(), false);
    return hasWayHelper(g, firstTrans.second, firstTrans.first, visited, 0);
}

// ZADA4A 3
template <typename T>
struct DCLE
{
    T inf;
    DCLE<T>* next;
    DCLE<T>* prev;

    DCLE(const T& data) : inf(data), next(nullptr), prev(nullptr)
    {
    }
};

template <typename T>
void printList(const DCLE<T>* pos)
{
    if (!pos)
        std::cout << std::endl;

    std::cout << pos->inf << ' ';

    DCLE<T>* copy = pos->next;
    while (copy != pos)
    {
        std::cout << copy->inf << ' ';
        copy = copy->next;
    }

    std::cout << std::endl;
}

template <typename T>
void removeElement(DCLE<T>*& pos)
{
    if (!pos)
        return;

    if (pos->next == pos && pos->prev == pos)
    {
        delete pos;
        return;
    }

    DCLE<T>* temp = pos;

    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;

    pos = pos->next;

    delete temp;
    temp = nullptr;
}

template <typename T>
DCLE<T>* findElementToRemove(DCLE<T>*& pos)
{
    if (!pos) return pos;

    if (pos->inf == pos->next->inf + pos->prev->inf)
        return pos;

    DCLE<T>* copy = pos->next;
    while (copy != pos)
    {
        if (copy->inf == copy->next->inf + copy->prev->inf)
        {
            pos = copy;
            return pos;
        }

        copy = copy->next;
    }

    return nullptr;
}

template <typename T>
void removeAdjSum(DCLE<T>*& pos)
{
    if (!pos)
        return;

    while (findElementToRemove(pos) != nullptr)
    {
        removeElement(pos);
    }
}

int main()
{
    // ZAD 1
    TNode<int>* leaf2 = new TNode<int>(2);
    TNode<int>* leaf5 = new TNode<int>(5);
    TNode<int>* leaf11 = new TNode<int>(11);
    TNode<int>* leaf5_2 = new TNode<int>(5);

    // Тест с екстра връх
    //TNode<int>* test = new TNode<int>(100);
    //TNode<int>* leaf5_2 = new TNode<int>(5, test);

    TNode<int>* node6 = new TNode<int>(6, leaf5, leaf11);
    TNode<int>* node7 = new TNode<int>(7, leaf2, node6);
    TNode<int>* node9 = new TNode<int>(9, leaf5_2);
    TNode<int>* node9_2 = new TNode<int>(9, nullptr, node9);

    TNode<int>* root = new TNode<int>(1, node7, node9_2);

    print_tree<int>(root);

    removeBigLeafs<int>(root);

    std::cout << std::endl;
    print_tree<int>(root);

    deallocate(root);

    std::cout << std::endl;

    // ZAD 3
    DCLE<int>* el5 = new DCLE<int>(5);
    DCLE<int>* el3 = new DCLE<int>(3);
    DCLE<int>* el1 = new DCLE<int>(1);
    DCLE<int>* el7 = new DCLE<int>(7);
    DCLE<int>* el4 = new DCLE<int>(4);
    DCLE<int>* el2 = new DCLE<int>(2);

    el5->prev = el2;
    el5->next = el3;

    el3->prev = el5;
    el3->next = el1;

    el1->prev = el3;
    el1->next = el7;

    el7->prev = el1;
    el7->next = el4;

    el4->prev = el7;
    el4->next = el2;

    el2->prev = el4;
    el2->next = el5;

    printList(el5);

    removeAdjSum(el5);

    printList(el5);

    return 0;
}
