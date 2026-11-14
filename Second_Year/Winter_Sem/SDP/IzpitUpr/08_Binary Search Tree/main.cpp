//
// Created by Lenovo on 29.1.2026 г..
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits> // За INT_MIN, INT_MAX

template <typename T>
struct Node
{
    T data;
    Node* left;
    Node* right;

    Node(const T& val, Node* left = nullptr, Node* right = nullptr) : data(val), left(left), right(right)
    {
    }
};

// 2
template <typename T>
void sortedHelper(Node<T>* root, std::vector<T>& res)
{
    if (!root) return;

    sortedHelper(root->left, res);
    res.push_back(root->data);
    sortedHelper(root->right, res);
}

template <typename T>
std::vector<T> getSortedElements(Node<T>* root)
{
    std::vector<T> result;
    sortedHelper(root, result);
    return result;
}

// 3
template <typename T>
T findSuccessor(Node<T>* root, const T& x)
{
    Node<T>* current = root;
    Node<T>* successor = nullptr;

    while (current)
    {
        if (current->data > x)
        {
            successor = current;
            current = current->left;
        }
        else
            current = current->right;
    }

    if (!successor) throw std::runtime_error("No successor found (x is max or not in tree)");

    return successor->data;
}

// 4
template <typename T>
Node<T>* insert(Node<T>* root, const T& val)
{
    if (!root) return new Node<T>(val);

    if (val < root->data)
        root->left = insert(root->left, val);
    else if (val > root->data)
        root->right = insert(root->right, val);

    return root;
}

template <typename T>
Node<T>* buildBST(const std::vector<T>& elements)
{
    Node<T>* root = nullptr;
    for (const T& el : elements)
    {
        insert(root, el);
    }
    return root;
}

// 5
template <typename T>
bool hasTwoSums(Node<T>* root, const int k)
{
    std::vector<T> sorted = getSortedElements(root);

    std::size_t left = 0, right = sorted.size() - 1;
    while (left < right)
    {
        int sum = sorted[left] + sorted[right];
        if (sum == k)
            return true;
        else if (sum < k)
            ++left;
        else
            --right;
    }
    return false;
}

// 6
template <typename T>
bool isBSTHelper(Node<T>* root, long long minVal, long long maxVal)
{
    if (!root) return true;

    if (root->data <= minVal || root->data >= maxVal) return false;

    return isBSTHelper(root->left, minVal, root->data) &&
        isBSTHelper(root->right, root->data, maxVal);
}

template <typename T>
bool isBST(Node<T>* root)
{
    return isBSTHelper(root, LLONG_MIN, LLONG_MAX);
}

// 7
void findKth(Node<int>* root, const int k, int& count, int& kthValue, bool& found)
{
    if (!root || found) return;

    findKth(root->left, k, count, kthValue, found);

    if (found) return;

    count++;
    if (count == k)
    {
        kthValue = root->data;
        found = true;
        return;
    }

    findKth(root->right, k, count, kthValue, found);
}

int sumLessOrEqual(Node<int>* root, int threshold)
{
    if (!root) return 0;

    if (root->data <= threshold)
        return root->data + sumLessOrEqual(root->left, threshold) + sumLessOrEqual(root->right, threshold);

    return sumLessOrEqual(root->left, threshold);
}

int sumUpToKth(Node<int>* root, const int k)
{
    int count = 0;
    int kthValue = 0;
    bool found = false;

    findKth(root, k, count, kthValue, found);

    if (!found) return 0;

    return sumLessOrEqual(root, kthValue);
}
