//
// Created by Lenovo on 27.1.2026 г..
//
#include <iostream>
#include <unordered_map>
#include <list>

template <typename T>
struct Node
{
    T data;
    Node* left;
    Node* right;

    bool leaf() const
    {
        return left == nullptr && right == nullptr;
    }


    Node(const T& data, Node* left = nullptr, Node* right = nullptr) : data(data), left(left), right(right)
    {
    };
};

struct Graph
{
    using AdjList = std::unordered_map<char, std::list<char>>;
    AdjList adj;


    void addEdge(char from, char to)
    {
        adj[from].push_back(to);
    }
};

bool findPathHelper(const Graph& g, const Node<char>* root, std::string& word)
{
    if (!root)
        return false;

    Graph::AdjList::const_iterator it = g.adj.find(root->data);

    if (it == g.adj.end() && !root->leaf())
        return false;

    word.push_back(root->data);

    if (!root->left && !root->right)
    {
        std::cout << word;

        return true;
    }

    const std::list<char>& neighbours = it->second;

    for (char neighbour : neighbours)
    {
        if (root->left && neighbour == root->left->data && findPathHelper(g, root->left, word))
        {
            return true;
        }

        if (root->right && neighbour == root->right->data && findPathHelper(g, root->right, word))
        {
            return true;
        }
    }

    word.pop_back();

    return false;
}

void findPath(const Graph& g, const Node<char>* root)
{
    std::string word = "";
    findPathHelper(g, root, word);
}
