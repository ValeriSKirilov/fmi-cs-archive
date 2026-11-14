//
// Created by Lenovo on 26.1.2026 г..
//

#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <algorithm>

template <typename T>
struct Node
{
    T value;
    std::vector<Node*> children;

    Node(const T& val = T{}, const std::vector<Node*>& children = std::vector<Node*>()) : value(val), children(children)
    {
    }
};

template <typename T>
void deallocate(Node<T>*& root)
{
    if (!root)
        return;

    for (Node<T>*& child : root->children)
    {
        deallocate(child);
    }

    delete root;
    root = nullptr;
}

template <typename T>
std::vector<std::vector<T>> get_elements_per_level(Node<T>* root)
{
    if (!root)
        return std::vector<std::vector<T>>();

    std::queue<Node<T>*> q;
    std::vector<std::vector<T>> result;
    std::vector<T> curr_level;
    q.push(root);

    while (!q.empty())
    {
        int level_size = q.size();

        for (int i = 0; i < level_size; ++i)
        {
            const Node<T>* curr = q.front();
            q.pop();

            for (Node<T>* child : curr->children)
                q.push(child);

            curr_level.push_back(curr->value);
        }

        result.push_back(curr_level);
        curr_level.clear();
    }

    return result;
}

template <typename T>
bool are_isomorphic_ordered(const Node<T>* r1, const Node<T>* r2)
{
    if (!r1 && !r2) return true;
    if (!r1 || !r2) return false;

    if (r1->value != r2->value) return false;
    if (r1->children.size() != r2->children.size()) return false;

    for (std::size_t i = 0; i < r1->children.size(); ++i)
    {
        if (!are_isomorphic_ordered(r1->children[i], r2->children[i]))
            return false;
    }

    return true;
}

template <typename T>
bool are_isomorphic_unordered(const Node<T>* r1, const Node<T>* r2)
{
    if (!r1 && !r2) return true;
    if (!r1 || !r2) return false;

    if (r1->value != r2->value) return false;
    if (r1->children.size() != r2->children.size()) return false;

    std::vector<Node<T>*> c1 = r1->children;
    std::vector<Node<T>*> c2 = r2->children;

    auto comp = [](const Node<T>* a, const Node<T>* b)
    {
        return a->value < b->value;
    };

    std::sort(c1.begin(), c1.end, comp);
    std::sort(c2.begin(), c2.end, comp);

    for (std::size_t i = 0; i < c1.size(); ++i)
    {
        if (!are_isomorphic_unordered(c1[i], c2[i]))
            return false;
    }

    return true;
}

template <typename T>
Node<T>* build_from_parent_array(const std::vector<T>& parents)
{
    if (parents.empty()) return nullptr;

    int n = parents.size();

    std::vector<Node<T>*> nodes;
    nodes.reserve(n);

    for (int i = 0; i < n; ++i)
    {
        nodes.push_back(new Node<T>(i));
    }

    Node<T>* root = nullptr;

    for (int i = 0; i < n; ++i)
    {
        int p_index = parents[i];

        if (p_index == -1)
            root = nodes[i];
        else
        {
            if (p_index >= 0 && p_index < n)
            {
                nodes[p_index]->children.push_back(nodes[i]);
            }
        }
    }

    return root;
}

int main()
{
    Node<int>* el01 = new Node(1);
    Node<int>* el02 = new Node(2);
    Node<int>* el03 = new Node(3);
    Node<int>* el04 = new Node(4);
    Node<int>* el05 = new Node(5);
    Node<int>* el06 = new Node(6);
    Node<int>* el07 = new Node(7);
    Node<int>* el08 = new Node(8);
    Node<int>* el09 = new Node(9);
    Node<int>* el10 = new Node(10);
    Node<int>* el11 = new Node(11);

    el01->children.push_back(el02);
    el01->children.push_back(el03);
    el01->children.push_back(el04);

    el02->children.push_back(el05);
    el02->children.push_back(el06);

    el03->children.push_back(el07);

    el07->children.push_back(el08);
    el07->children.push_back(el09);
    el07->children.push_back(el10);
    el07->children.push_back(el11);

    std::vector<std::vector<int>> result = get_elements_per_level<int>(el01);

    for (auto& i : result)
    {
        for (int j : i)
            std::cout << j << ' ';
        std::cout << std::endl;
    }

    delete el01;
    delete el02;
    delete el03;
    delete el04;
    delete el05;
    delete el06;
    delete el07;
    delete el08;
    delete el09;
    delete el10;
    delete el11;
}
