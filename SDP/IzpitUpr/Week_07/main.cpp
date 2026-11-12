//
// Created by Lenovo on 25.1.2026 г..
//

#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template <class T>
struct BinTreeNode
{
    T value;
    BinTreeNode *left, *right;

    BinTreeNode(const T& val = T{}, BinTreeNode* left = nullptr, BinTreeNode* right = nullptr) : value(val), left(left),
        right(right)
    {
    }
};

void deallocate(BinTreeNode<int>*& root)
{
    if (!root)
        return;

    deallocate(root->left);
    deallocate(root->right);

    delete root;
    root = nullptr;
}

int get_depth(const BinTreeNode<int>* const root)
{
    if (!root)
        return 0;

    return 1 + std::max(get_depth(root->left), get_depth(root->right));
}

int count_nodes(const BinTreeNode<int>* const root)
{
    if (!root)
        return 0;

    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

void extract_helper(const BinTreeNode<int>* root, std::function<bool(const BinTreeNode<int>*)>& pred,
                    std::vector<const BinTreeNode<int>*>& result)
{
    if (!root)
        return;

    if (pred(root))
        result.push_back(root);

    extract_helper(root->left, pred, result);
    extract_helper(root->right, pred, result);
}

std::vector<const BinTreeNode<int>*> extract_nodes_by_pred(const BinTreeNode<int>* root,
                                                           std::function<bool(const BinTreeNode<int>*)>& pred)
{
    std::vector<const BinTreeNode<int>*> result;
    extract_helper(root, pred, result);

    return result;
}

void print_inorder(const BinTreeNode<int>* const root)
{
    if (!root)
        return;

    print_inorder(root->left);
    std::cout << root->value << ' ';
    print_inorder(root->right);
}

void print_preorder(const BinTreeNode<int>* const root)
{
    if (!root)
        return;

    std::cout << root->value << ' ';
    print_preorder(root->left);
    print_preorder(root->right);
}

void print_postorder(const BinTreeNode<int>* const root)
{
    if (!root)
        return;

    print_postorder(root->left);
    print_postorder(root->right);
    std::cout << root->value << ' ';
}

void print_per_level(BinTreeNode<int>* root)
{
    if (!root)
    {
        std::cout << std::endl;
        return;
    }

    std::queue<BinTreeNode<int>*> q;
    q.push(root);

    while (!q.empty())
    {
        int levelSize = q.size();

        for (int i = 0; i < levelSize; ++i)
        {
            const BinTreeNode<int>* curr = q.front();
            q.pop();
            std::cout << curr->value << ' ';

            if (curr->left) q.push(curr->left);
            if (curr->right) q.push(curr->right);
        }

        std::cout << std::endl;
    }
}

int main()
{
    BinTreeNode<int>* root =
        new BinTreeNode<int>(
            1,
            new BinTreeNode<int>(
                2,
                new BinTreeNode<int>(4),
                new BinTreeNode<int>(
                    5,
                    new BinTreeNode<int>(10),
                    new BinTreeNode<int>(11)
                )
            ),
            new BinTreeNode<int>(
                3,
                new BinTreeNode<int>(
                    6,
                    new BinTreeNode<int>(8)
                ),
                new BinTreeNode<int>(
                    7,
                    nullptr,
                    new BinTreeNode<int>(9)
                )
            )
        );

    print_per_level(root);

    deallocate(root);


    return 0;
}
