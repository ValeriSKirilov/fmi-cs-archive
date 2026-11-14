//
// Created by Lenovo on 8.1.2026 г..
//

#include <iostream>
#include <cassert>
#include <functional>
#include <vector>
#include <queue>

template <class T>
struct BinTreeNode
{
    T value;
    BinTreeNode *left, *right;

    BinTreeNode(const T& val = T{}, BinTreeNode* left = nullptr, BinTreeNode* right = nullptr);
};

template <typename T>
void deallocate(BinTreeNode<T>*& root)
{
    if (root == nullptr)
        return;

    deallocate<T>(root->left);
    deallocate<T>(root->right);

    delete root;
    root = nullptr;
}

template <typename T>
int get_depth(const BinTreeNode<T>* const root)
{
    if (root == nullptr)
        return 0;

    return 1 + std::max(get_depth<T>(root->left), get_depth<T>(root->right));
}

template <typename T>
int count_nodes(const BinTreeNode<T>* const root)
{
    if (root == nullptr)
        return 0;

    return 1 + count_nodes<T>(root->left) + count_nodes<T>(root->right);
}

template <typename T>
void extract_helper(const BinTreeNode<T>* root,
                    std::function<bool(const BinTreeNode<T>*)>& pred,
                    std::vector<const BinTreeNode<T>*>& result)
{
    if (root == nullptr)
        return;

    if (pred(root))
        result.push_back(root);

    extract_helper(root->left, pred, result);
    extract_helper(root->right, pred, result);
}

template <typename T>
std::vector<const BinTreeNode<T>*> extract_nodes_by_pred(const BinTreeNode<T>* root,
                                                         std::function<bool(const BinTreeNode<T>*)>& pred)
{
    std::vector<const BinTreeNode<T>*> result;
    etract_helper(root, pred, result);

    return result;
}

template <typename T>
void print_inorder(const BinTreeNode<T>* const root)
{
    if (root == nullptr)
        return;

    print_inorder(root->left);
    std::cout << root->value << ' ';
    print_inorder(root->right);
}

template <typename T>
void print_preorder(const BinTreeNode<T>* const root)
{
    if (root == nullptr)
        return;

    std::cout << root->value << ' ';
    print_preorder(root->left);
    print_preorder(root->right);
}

template <typename T>
void print_postorder(const BinTreeNode<T>* const root)
{
    if (root == nullptr)
        return;

    print_postorder(root->left);
    print_postorder(root->right);
    std::cout << root->value << ' ';
}

template <typename T>
void print_per_levels(BinTreeNode<T>* root)
{
    if (root == nullptr)
    {
        std::cout << std::endl;
        return;
    }

    std::queue<BinTreeNode<T>*> q;
    q.push(root);

    while (!q.empty())
    {
        BinTreeNode<T>* curr = q.front();
        q.pop();

        std::cout << curr->value << ' ';

        if (curr->left)
            q.push(curr->left);

        if (curr->right)
            q.push(curr->right);
    }
}

int main()
{
    auto* root =
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

    std::cout << get_depth(root) << std::endl;
    std::cout << count_nodes(root) << std::endl;

    // std::vector<const BinTreeNode<int>*> leaves = extract_nodes_by_pred<int>(
    //     root,
    //     [](const BinTreeNode<int>* node) { return node->left == nullptr && node->right == nullptr; }
    // );

    // std::vector<const BinTreeNode<int>*> evens = extract_nodes_by_pred<int>(
    //     root,
    //     [](const BinTreeNode<int>* node) { return node->value % 2 == 0; }
    // );

    // for (const BinTreeNode<int>* leaf : leaves)
    //     std::cout << leaf->value << ' ';
    // std::cout << std::endl;

    // for (const BinTreeNode<int>* even : evens)
    //     std::cout << even->value << ' ';
    // std::cout << std::endl;


    print_inorder<int>(root);
    std::cout << std::endl;
    print_preorder<int>(root);
    std::cout << std::endl;
    print_postorder<int>(root);
    std::cout << std::endl;
    print_per_levels<int>(root);

    deallocate<int>(root);
    assert(root == nullptr);

    return 0;
}
