//
// Created by Lenovo on 11.1.2026 г..
//

#include <iostream>
#include <vector>
#include <string>

struct Node
{
    char symbol;
    Node* left;
    Node* middle;
    Node* right;

    Node(const char data, Node* left = nullptr, Node* middle = nullptr, Node* right = nullptr) : symbol(data), left(left),
        middle(middle), right(right)
    {
    }
};

void deallocate(const Node* root)
{
    if (!root)
        return;

    deallocate(root->left);
    deallocate(root->right);
    delete root;
}

void getAllWords(const Node* root, std::vector<std::string>& words, std::string& currentWord)
{
    if (!root->left && !root->middle && !root->right)
        words.push_back(currentWord + root->symbol);

    currentWord.push_back(root->symbol);

    if (root->left)
        getAllWords(root->left, words, currentWord);

    if (root->middle)
        getAllWords(root->middle, words, currentWord);

    if (root->right)
        getAllWords(root->right, words, currentWord);

    currentWord.pop_back();
}

bool isDuplicated(const std::vector<std::string>& arr, const std::string& word)
{
    for (std::size_t i = 0; i < arr.size(); ++i)
    {
        if (arr[i] == word)
            return true;
    }

    return false;
}

std::vector<std::string> filterUnique(const std::vector<std::string>& words)
{
    std::vector<std::string> result;
    for (std::size_t i = 0; i < words.size(); ++i)
    {
        if (!isDuplicated(result, words[i]))
            result.push_back(words[i]);
    }

    return result;
}

std::vector<std::string> getUniqueWords(const Node* root)
{
    std::vector<std::string> words;
    std::string currentWord;
    getAllWords(root, words, currentWord);
    filterUnique(words);
    return words;
}

int main()
{
    Node* root = new Node('A',
                          new Node('B',
                                   new Node('D',
                                            nullptr,
                                            nullptr,
                                            nullptr
                                   ),
                                   nullptr,
                                   new Node('E',
                                            nullptr,
                                            nullptr,
                                            nullptr
                                   )
                          ),
                          new Node('C',
                                   nullptr,
                                   nullptr,
                                   nullptr
                          ),
                          new Node('B',
                                   nullptr,
                                   new Node('D',
                                            nullptr,
                                            nullptr
                                   ),
                                   nullptr
                          )
    );

    const std::vector<std::string> result = getUniqueWords(root);

    std::cout << "[";
    for (const auto& i : result)
        std::cout << "\"" << i << "\"" << ",";
    std::cout << "]";

    deallocate(root);

    return 0;
}
