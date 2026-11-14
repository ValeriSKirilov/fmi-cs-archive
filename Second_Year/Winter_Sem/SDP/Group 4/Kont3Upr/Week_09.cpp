//
// Created by Lenovo on 29.12.2025 г..
//

#import <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>

template <typename T>
struct Node
{
    T value;
    std::vector<Node*> children;

    Node(T val = T{}, std::vector<Node*> children = std::vector<Node*>())
        : value(std::move(val)), children(std::move(children))
    {
    }
};

template <typename T>
void deallocate(Node<T>*& root)
{
    if (root == nullptr)
        return;

    for (int i = 0; i < root->children.size(); ++i)
        deallocate(root->children[i]);

    delete root;
    root = nullptr;
}

template <typename T>
std::vector<std::vector<T>> get_elements_per_level(Node<T>* root)
{
    if (root == nullptr)
        return std::vector<std::vector<T>>();

    std::queue<Node<T>*> queue;
    std::vector<std::vector<T>> result;
    std::vector<T> current_level;
    queue.push(root);
    int curr_cnt = 1, next_cnt = 0;

    while (!queue.empty())
    {
        Node<T>* temp = queue.front();
        queue.pop();

        --curr_cnt;
        current_level.push_back(temp->value);

        for (Node<T>* child : temp->children)
        {
            ++next_cnt;
            queue.push(child);
        }

        if (curr_cnt == 0)
        {
            result.push_back(current_level);
            current_level.clear();

            curr_cnt = next_cnt;
            next_cnt = 0;
        }
    }

    return result;
}

// template <typename T>
Node<std::string>* build_from_file(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file.is_open())
        return nullptr;

    std::string line;
    std::getline(file, line);
    auto* root = new Node(line);

    std::getline(file, line);

    std::stringstream firstSS(line);
    std::string firstWord;

    std::vector<Node<std::string>*> currChildren;

    while (firstSS >> firstWord)
    {
        auto* fstWord = new Node(firstWord);
        currChildren.push_back(fstWord);
    }

    root->children = currChildren;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string word;

        std::vector<Node<std::string>*> nextLevelParents;
        std::vector<Node<std::string>*> currChildChildren;

        int currChild = 0;

        while (ss >> word)
        {
            if (word == "*")
            {
                currChildren[currChild]->children = currChildChildren;
                currChildChildren.clear();
                ++currChild;
            }
            else
            {
                auto* newNode = new Node(word);
                currChildChildren.push_back(newNode);
                nextLevelParents.push_back(newNode);
            }
        }

        if (!currChildChildren.empty())
            currChildren[currChild]->children = currChildChildren;

        currChildren = nextLevelParents;
    }

    file.close();

    return root;
}

// template <typename T>
void print_tree(const Node<std::string>* node, const int depth = 0)
{
    if (!node)
        return;

    for (int i = 0; i < depth; ++i)
        std::cout << " ";

    std::cout << node->value << std::endl;

    for (Node<std::string>* child : node->children)
        print_tree(child, depth + 1);
}

template <typename T>
bool compare_with_order(Node<T>* root1, Node<T>* root2)
{
    if (root1 == nullptr && root2 == nullptr)
        return true;

    if (root1 == nullptr || root2 == nullptr)
        return false;

    if (root1->value != root2->value)
        return false;

    if (root1->children.size() != root2->children.size())
        return false;

    for (std::size_t i = 0; i < root1->children.size(); ++i)
    {
        if (!compare_with_order(root1->children[i], root2->children[i]))
            return false;
    }

    return true;
}

template <typename T>
bool compare_without_order(Node<T>* root1, Node<T>* root2)
{
    if (root1 == nullptr && root2 == nullptr)
        return true;

    if (root1 == nullptr || root2 == nullptr)
        return false;

    if (root1->value != root2->value)
        return false;

    if (root1->children.size() != root2->children.size())
        return false;

    std::vector<bool> matched(root2->children.size(), false);

    for (Node<T>* child1 : root1->children)
    {
        bool found = false;

        for (std::size_t i = 0; i < root2->children.size(); ++i)
        {
            if (!matched[i] && compare_without_order(child1, root2->children[i]))
            {
                matched[i] = true;
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

int main()
{
    Node<std::string>* root = build_from_file("tree.txt");

    if (!root)
    {
        std::cout << "Failed to build tree!" << std::endl;
        return 1;
    }


    std::cout << "Tree structure:" << std::endl;
    print_tree(root);

    deallocate(root);

    return 0;
}
