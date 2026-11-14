//
// Created by Lenovo on 9.1.2026 г..
//

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <list>
#include <stack>
#include <unordered_map>

template <class T>
struct BinTreeNode
{
    T value;
    BinTreeNode *left, *right;

    BinTreeNode(const T& val = T{}, BinTreeNode* left = nullptr, BinTreeNode* right = nullptr);

    bool leaf() const
    {
        return left == nullptr && right == nullptr;
    }
};

// 1.1
template <typename T>
bool isMirror(BinTreeNode<T>* t1, BinTreeNode<T>* t2)
{
    if (!t1 && !t2) return true;

    if (!t1 || !t2) return false;

    return (t1->value == t2->value) && isMirror(t1->left, t2->right) && isMirror(t1->right, t2->left);
}

template <typename T>
bool isSymmetric(BinTreeNode<T>* root)
{
    if (!root)
        return true;

    return isMirror(root->left, root->right);
}

// 1.2
template <typename T>
std::size_t heightAndDiam(BinTreeNode<T>* root, std::size_t& maxDiam)
{
    if (!root)
        return 0;

    std::size_t leftH = heightAndDiam(root->left, maxDiam);
    std::size_t rightH = heightAndDiam(root->right, maxDiam);

    maxDiam = std::max(maxDiam, leftH + rightH);

    return 1 + std::max(leftH, rightH);
}

template <typename T>
std::size_t diameter(BinTreeNode<T>* root)
{
    std::size_t maxDiam = 0;
    heightAndDiam(root, maxDiam);
    return maxDiam;
}

// 1.3
//DFS
template <typename T>
bool checkUtil(BinTreeNode<T>* root, const int level, int& leafLevel)
{
    if (!root) return true;

    if (!root->left && !root->right)
    {
        if (leafLevel == 0)
        {
            leafLevel = level;
            return true;
        }

        return level == leafLevel;
    }

    return checkUtil(root->left, level + 1, leafLevel)
        && checkUtil(root->right, level + 1, leafLevel);
}

template <typename T>
bool checkLeavesLevel(BinTreeNode<T>* root)
{
    int leafLevel = 0;
    return checkUtil(root, 1, leafLevel);
}

// BFS
// template <typename T>
// bool checkLeavesLevel(BinTreeNode<T>* root)
// {
//     if (!root)
//         return true;
//
//     std::queue<std::pair<BinTreeNode<T>*, int>> q;
//     q.push({root, 0});
//
//     int firstLeafLevel = -1;
//
//     while (!q.empty())
//     {
//         std::pair<BinTreeNode<T>*, int> current = q.front();
//         q.pop();
//
//         BinTreeNode<T>* node = current.first;
//         int level = current.second;
//
//         if (!node->left && !node->right)
//         {
//             if (firstLeafLevel == -1)
//             {
//                 firstLeafLevel = level;
//             }
//             else if (level != firstLeafLevel)
//             {
//                 return false;
//             }
//         }
//         else
//         {
//             if (node->left)
//             {
//                 q.push({node->left, level + 1});
//             }
//             if (node->right)
//             {
//                 q.push({node->right, level + 1});
//             }
//         }
//     }
//
//     return true;
// }

// 2.1
template <typename T>
BinTreeNode<T>* findLCA(BinTreeNode<T>* root, T v1, T v2)
{
    if (!root)
        return root;

    if (v1 < root->value && v2 < root->value)
        return findLCA(root->left, v1, v2);

    if (v1 > root->value && v2 > root->value)
        return findLCA(root->right, v1, v2);

    return root;
}

// 2.2
std::size_t rangeSumBST(const BinTreeNode<int>* const root, const int low, const int high)
{
    if (!root)
        return 0;

    if (root->value > high)
        return rangeSumBST(root->left, low, high);

    if (root->value < low)
        return rangeSumBST(root->right, low, high);

    return root->value + rangeSumBST(root->left, low, high) + rangeSumBST(root->right, low, high);
}

// 2.3
template <typename T>
bool validate(BinTreeNode<T>* root, const std::size_t minVal, const std::size_t maxVal)
{
    if (!root)
        return true;

    if (root->value <= minVal || root->value >= maxVal)
        return false;

    return validate(root->left, minVal, static_cast<std::size_t>(root->value)) && validate(
        root->right, static_cast<std::size_t>(root->value), maxVal);
}

template <typename T>
bool isValidBst(BinTreeNode<T>* root)
{
    if (!root)
        return true;

    return validate(root, LLONG_MAX, LLONG_MIN);
}

template <typename T>
struct Node
{
    T value;
    std::vector<Node*> children;

    Node(const T& val = T{}, const std::vector<Node*>& children = std::vector<Node*>())
        : value(val), children(children)
    {
    }
};

// 3.1
template <typename T>
void zigZagTraversal(Node<T>* root)
{
    if (!root)
        return;

    std::queue<Node<T>*> q;
    q.push(root);

    bool leftToRight = true;

    while (!q.empty())
    {
        int levelSize = q.size();
        std::vector<T> currLevelValues;

        for (int i = 0; i < levelSize; ++i)
        {
            Node<T>* curr = q.front();
            q.pop();

            currLevelValues.push_back(curr->value);

            for (Node<T>* child : curr->children)
            {
                q.push(child);
            }
        }

        if (!leftToRight)
        {
            std::reverse(currLevelValues.begin(), currLevelValues.end());
        }

        for (const T& val : currLevelValues)
        {
            std::cout << val << ' ';
        }
        std::cout << std::endl;

        leftToRight = !leftToRight;
    }
}

// 3.2
template <typename T>
BinTreeNode<T>* encodeNaryToBinary(Node<T>* root)
{
    if (!root)
        return nullptr;

    BinTreeNode<T>* newRoot = new BinTreeNode<T>(root->value);

    if (!root->children.empty())
    {
        newRoot->left = new BinTreeNode<T>(root->children[0]);

        BinTreeNode<T>* current = newRoot->left;

        for (std::size_t i = 1; i < root->children.size(); ++i)
        {
            current->right = encodeNaryToBinary(root->children[i]);

            current = current->right;
        }
    }

    return newRoot;
}

// 4.1
int findJudge(const int N, const std::vector<std::vector<int>>& trust)
{
    std::vector<int> inDegree(N + 1, 0);
    std::vector<int> outDegree(N + 1, 0);

    for (const auto& edge : trust)
    {
        const int u = edge[0];
        const int v = edge[1];

        outDegree[u]++;
        inDegree[v]++;
    }

    for (int i = 1; i <= N; ++i)
    {
        if (inDegree[i] == N - 1 && outDegree[i] == 0)
            return i;
    }

    return -1;
}

// 4.2
bool hasPath(std::vector<std::vector<int>>& edges, const int start, const int end, std::vector<bool>& visited)
{
    if (start == end)
        return true;

    visited[start] = true;

    for (const int node : edges[start])
    {
        if (!visited[node])
        {
            if (hasPath(edges, node, end, visited))
                return true;
        }
    }

    return false;
}

bool validPath(std::vector<std::vector<int>>& edges, const int start, const int end)
{
    std::vector visited(edges.size(), false);
    return hasPath(edges, start, end, visited);
}

// ANOTHER

// 1
template <typename T>
std::size_t maxDepthNaryTree(const Node<T>* const root)
{
    if (!root)
        return 0;

    std::size_t depth = 0;

    for (const Node<T>* node : root->children)
    {
        depth = std::max(depth, maxDepthNaryTree(node));
    }

    return 1 + depth;
}

// 2
template <typename T>
bool isMirror2(BinTreeNode<T>* r1, BinTreeNode<T>* r2)
{
    if (!r1 && !r2)
        return true;

    if (!r1 || !r2)
        return false;

    return r1 == r2 && isMirror2(r1->left, r2->right) && isMirror2(r1->right, r2->left);
}

template <typename T>
bool isSymmetric2(BinTreeNode<T>* root)
{
    if (!root)
        return true;

    return isMirror2(root->left, root->right);
}

// 3
std::size_t findJudge2(const std::vector<std::vector<int>>& trust)
{
    std::vector<int> inDegree(trust.size(), 0);
    std::vector<int> outDegree(trust.size(), 0);

    for (const std::vector<int>& edge : trust)
    {
        const int person = edge[0];

        outDegree[person]++;

        for (std::size_t i = 1; i < edge.size(); ++i)
        {
            inDegree[edge[1]]++;
        }
    }

    for (std::size_t i = 1; i <= trust.size(); ++i)
    {
        if (inDegree[i] == trust.size() - 1 && outDegree[i] == 0)
            return i;
    }

    return -1;
}

struct Graph
{
    using AdjList = std::unordered_map<char, std::list<char>>;
    AdjList adj;

    void addEdge(const char from, const char to)
    {
        adj[from].push_back(to);
    }
};

bool findPathHelper(const Graph& g, const BinTreeNode<char>* root, std::string& word)
{
    if (root == nullptr)
        return false;

    const auto it = g.adj.find(root->value);

    if (it == g.adj.end() && !root->leaf())
        return false;

    word.push_back(root->value);

    if (!root->left && !root->right)
    {
        std::cout << word;

        return true;
    }

    const std::list<char>& neighbours = it->second;

    for (char neighbour : neighbours)
    {
        if (root->left && neighbour == root->left->value && findPathHelper(g, root->left, word))
            return true;

        if (root->right && neighbour == root->right->value && findPathHelper(g, root->right, word))
            return true;
    }

    word.pop_back();

    return false;
}

void findPath(const Graph g, const BinTreeNode<char>* root)
{
    std::string word;
    findPathHelper(g, root, word);
}

// TOPOLOGICAL SORT
void topoSortHelper(const std::vector<std::vector<int>>& g, const int curr, std::vector<bool>& visited,
                    std::stack<int>& stack)
{
    if (visited[curr])
        return;

    visited[curr] = true;

    for (const int node : g[curr])
    {
        topoSortHelper(g, node, visited, stack);
    }

    stack.push(curr);
}

std::vector<int> topoSort(const std::vector<std::vector<int>>& g)
{
    std::vector visited(g.size(), false);
    std::stack<int> stack;

    for (std::size_t i = 0; i < g.size(); ++i)
    {
        topoSortHelper(g, i, visited, stack);
    }

    std::vector<int> result;

    while (!stack.empty())
    {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}

// CHECK FOR A CYCLE
bool hasCycleHelp(const std::vector<std::vector<int>>& g, const int curr, std::vector<bool>& visited,
                  std::vector<bool>& stack)
{
    if (visited[curr])
        return false;

    visited[curr] = true;
    stack[curr] = true;

    for (const int node : g[curr])
    {
        if (stack[node])
            return true;

        if (!visited[node] && hasCycleHelp(g, node, visited, stack))
            return true;
    }

    stack[curr] = false;
    return false;
}

bool hasCycle(const std::vector<std::vector<int>>& g)
{
    std::vector visited(g.size(), false);
    std::vector stack(g.size(), false);

    for (std::size_t i = 0; i < g.size(); i++)
    {
        if (hasCycleHelp(g, i, visited, stack))
            return true;
    }

    return false;
}
