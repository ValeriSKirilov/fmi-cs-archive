//
// Created by Lenovo on 30.1.2026 г..
//

#include <iostream>
#include <algorithm>
#include <optional>
#include <set>
#include <queue>
#include <unordered_map>
#include <vector>
#include <utility>

// Zad 1
struct Node
{
    char symbol;
    Node *left, *middle, *right;

    Node(char symbol, Node* left = nullptr, Node* middle = nullptr, Node* right = nullptr)
        : symbol(symbol), left(left), middle(middle), right(right)
    {
    }
};

bool isLeaf(Node const* tree)
{
    if (!tree) return false;
    return !tree->left && !tree->middle && !tree->right;
}

void words(const Node* root, std::string& acc, std::set<std::string>& result)
{
    if (!root)
        return;

    acc += root->symbol;

    if (isLeaf(root))
        result.insert(acc);

    words(root->left, acc, result);
    words(root->middle, acc, result);
    words(root->right, acc, result);

    acc.pop_back();
}

std::set<std::string> words(const Node* root)
{
    std::set<std::string> result;
    std::string acc;
    words(root, acc, result);
    return result;
}

void free(const Node* root)
{
    if (!root) return;

    free(root->left);
    free(root->middle);
    free(root->right);

    delete root;
}

// Zad 3
template <typename T>
struct btNode
{
    T data;
    btNode *left, *right;

    btNode(const T& data, btNode* left = nullptr, btNode* right = nullptr) : data(data), left(left), right(right)
    {
    }
};

template <typename T>
std::optional<std::pair<unsigned, unsigned>> levelsWithEqSums(const btNode<T>* root)
{
    if (!root)
        return std::nullopt;

    std::queue<btNode<T> const*> q;
    q.push(root);

    std::vector<std::pair<unsigned, T>> sums;

    unsigned level = 0;
    while (!q.empty())
    {
        std::size_t qsize = q.size();

        T sum = T{};

        for (std::size_t i = 0; i < qsize; ++i)
        {
            btNode<T> const* front = q.front();
            q.pop();

            sum += front->data;

            if (front->left)
                q.push(front->left);

            if (front->right)
                q.push(front->right);
        }

        auto iter = std::find_if(sums.begin(), sums.end(),
                                 [&sum](std::pair<unsigned, T> const& currPair)
                                 {
                                     return currPair.second == sum;
                                 }
        );

        if (iter != sums.end())
        {
            return std::optional{std::pair<unsigned, unsigned>{level, (*iter).first}};
        }

        sums.push_back({level, sum});

        level++;
    }

    return std::nullopt;
}

template <typename T>
void free(const btNode<T>* root)
{
    if (!root) return;

    free(root->left);
    free(root->right);

    delete root;
}

// Zad 2
template <typename T>
using graph = std::unordered_map<T, std::vector<T>>;

template <typename T>
int indexOf(std::vector<T> const& v, T const& el)
{
    for (int i = 0; i < v.size(); ++i)
    {
        if (v[i] == el)
            return i;
    }
    return -1;
}

template <typename T>
bool hasCycleOfLength(T const& vertex, unsigned k, graph<T> const& g, std::vector<T>& path)
{
    int index = indexOf(path, vertex);

    if (index >= 0)
        return path.size() - index == k;

    path.push_back(vertex);

    bool cyclic = false;
    for (const T& child : g.at[vertex])
    {
        cyclic = cyclic || hasCycleOfLength(child, k, g, path);
    }

    path.pop_back();

    return cyclic;
}

template <typename T>
bool hasCycleOfLengthK(T const& vertex, unsigned k, graph<T> const& g)
{
    std::vector<T> path;
    return hasCycleOfLength(vertex, k, g, path);
}
