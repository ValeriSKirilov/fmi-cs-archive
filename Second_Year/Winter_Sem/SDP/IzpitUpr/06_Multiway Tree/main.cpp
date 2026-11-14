//
// Created by Lenovo on 29.1.2026 г..
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

template <typename T>
struct Tree
{
    T value;
    std::vector<Tree*> children;

    Tree(const T& val = T{}, std::vector<Tree*> children = std::vector<Tree*>())
        : value(val), children(children)
    {
    }
};

int countLeaves(const Tree<int>* root)
{
    if (!root)
        return 0;

    if (root->children.empty())
        return 1;

    int leaves = 0;
    for (Tree<int>* child : root->children)
    {
        leaves += countLeaves(child);
    }

    return leaves;
}

void getLevelHelper(Tree<int>* root, const int targetLevel, const int currentLevel, std::vector<int>& result)
{
    if (!root)
        return;

    if (currentLevel == targetLevel)
    {
        result.push_back(root->value);
        return;
    }

    for (Tree<int>* child : root->children)
        getLevelHelper(child, targetLevel, currentLevel + 1, result);
}

std::vector<int> getLevel(Tree<int>* root, int targetLevel)
{
    std::vector<int> result;
    getLevelHelper(root, targetLevel, 0, result);
    return result;
}

bool match(Tree<int>* t1, Tree<int>* t2)
{
    if (t1->value != t2->value)
        return false;

    for (Tree<int>* child1 : t1->children)
    {
        bool found = false;
        for (Tree<int>* child2 : t2->children)
        {
            if (match(child1, child2))
            {
                found = true;
                break;
            }
        }
        if (!found)
            return false;
    }

    return true;
}

bool isSubtreeOf(Tree<int>* t1, Tree<int>* t2)
{
    if (!t1) return true;
    if (!t2) return false;

    if (match(t1, t2)) return true;

    for (Tree<int>* child : t2->children)
    {
        if (isSubtreeOf(t1, child)) return true;
    }

    return false;
}

struct DpResult
{
    int incl, excl;
};

DpResult solveMaxSum(Tree<int>* root)
{
    if (!root) return {0, 0};

    int sumWithRoot = root->value;
    int sumWithoutRoot = 0;

    for (Tree<int>* child : root->children)
    {
        DpResult childRes = solveMaxSum(child);

        sumWithRoot += childRes.excl;

        sumWithoutRoot += std::max(childRes.incl, childRes.excl);
    }

    return {sumWithRoot, sumWithoutRoot};
}

int maxSumOfAdjacent(Tree<int>* root)
{
    DpResult res = solveMaxSum(root);
    return std::max(res.incl, res.excl);
}
