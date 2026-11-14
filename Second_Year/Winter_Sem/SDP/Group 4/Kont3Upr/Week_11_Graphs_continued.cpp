//
// Created by Lenovo on 9.1.2026 г..
//

#include <iostream>
#include <list>
#include <unordered_map>

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
    }
};

struct Graph
{
    using AdjList = std::unordered_map<char, std::list<char>>;
    AdjList adj;

    void addEdge(const char from, const char to)
    {
        adj[from].push_back(to);
    }
};

// TASK 2
bool findPathHelper(const Graph& g, const Node<char>* root, std::string& word)
{
    if (!root)
        return false;

    const auto it = g.adj.find(root->data);

    if (it == g.adj.end() && !root->leaf())
        return false;

    word.push_back(root->data);

    if (!root->left && !root->right)
    {
        std::cout << word;
        return true;
    }

    const std::list<char>& neighbours = it->second;

    for (const char neighbour : neighbours)
    {
        if (root->left != nullptr
            && neighbour == root->left->data
            && findPathHelper(g, root->left, word))
        {
            return true;
        }

        if (root->right != nullptr
            && neighbour == root->right->data
            && findPathHelper(g, root->right, word))
        {
            return true;
        }
    }

    word.pop_back();

    return false;
}

void findPath(const Graph& g, const Node<char>* root)
{
    std::string word;
    findPathHelper(g, root, word);
}

int main()
{
    auto* root = new Node('a');
    auto* n1 = new Node('e');
    auto* n2 = new Node('f');
    auto* n3 = new Node('d');
    auto* n4 = new Node('g');
    auto* n5 = new Node('c');
    auto* n6 = new Node('b');
    auto* n7 = new Node('h');
    root->left = n1;
    root->right = n2;
    n1->left = n3;
    n1->right = n4;
    n2->right = n5;
    n3->left = n6;
    n5->left = n7;

    Graph g;
    g.addEdge('a', 'f');
    g.addEdge('a', 'd');
    g.addEdge('b', 'h');
    g.addEdge('c', 'h');
    g.addEdge('d', 'e');
    g.addEdge('e', 'h');
    g.addEdge('f', 'h');
    g.addEdge('f', 'c');
    g.addEdge('g', 'b');
    g.addEdge('g', 'e');

    findPath(g, root);

    delete root;
    delete n1;
    delete n2;
    delete n3;
    delete n4;
    delete n5;
    delete n6;
    delete n7;

    return 0;
}
