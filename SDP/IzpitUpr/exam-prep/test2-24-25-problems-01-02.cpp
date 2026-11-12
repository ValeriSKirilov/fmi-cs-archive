//
// Created by Lenovo on 30.1.2026 г..
//

#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

// Zad 1
struct Node
{
    std::string word;
    Node *left, *right;

    Node(const std::string& word, Node* left = nullptr, Node* right = nullptr) : word(word), left(left), right(right)
    {
    }
};


Node* buildTree(const int i, const size_t n, std::string s[], int l[], int r[])
{
    if (i >= n)
        return nullptr;
    if (i == -1)
        return nullptr;

    Node* left = buildTree(l[i], n, s, l, r);
    Node* right = buildTree(r[i], n, s, l, r);

    return new Node(s[i], left, right);
}

Node* buildTree(const size_t n, std::string s[], int l[], int r[])
{
    return buildTree(0, n, s, l, r);
}

bool isWhitespace(const char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

std::vector<std::string> words(const std::string& s)
{
    std::vector<std::string> result;
    std::string acc;

    for (const char c : s)
    {
        if (isWhitespace(c))
        {
            if (!acc.empty())
            {
                result.push_back(acc);
                acc.clear();
            }
        }
        else
            acc += c;
    }

    if (!acc.empty())
        result.push_back(acc);

    return result;
}

bool horizontalSentence(const Node* root, const std::string& sentence)
{
    const std::vector<std::string> ws = words(sentence);

    if (!root)
        return ws.empty();

    std::queue<const Node*> q;
    q.push(root);

    while (!q.empty())
    {
        const size_t qsize = q.size();
        bool hasSentence = ws.size() == qsize;

        for (size_t i = 0; i < qsize; ++i)
        {
            const Node* front = q.front();
            q.pop();

            hasSentence = hasSentence && front->word == ws[i];

            if (front->left)
                q.push(front->left);

            if (front->right)
                q.push(front->right);
        }

        if (hasSentence)
            return true;
    }

    return false;
}

void free(Node const* tree)
{
    if (!tree) return;

    free(tree->left);
    free(tree->right);

    delete tree;
}

// Zad 2
using hist = std::unordered_map<std::string, unsigned>;

hist mkHist(const std::vector<std::string>& ws)
{
    hist h;

    for (const std::string& word : ws)
    {
        if (h.find(word) == h.end())
            h.insert({word, 1});
        else
            h[word]++;
    }

    return h;
}

bool areAnagrams(const hist& h1, const hist& h2)
{
    if (h1.size() != h2.size())
        return false;

    for (auto const& [key, value] : h1)
    {
        if (h2.find(key) == h2.end())
            return false;

        if (h2.at(key) != value)
            return false;
    }

    return true;
}

bool permuteOrCommon(const std::string& s1, const std::string& s2, std::ostream& os)
{
    hist h1 = mkHist(words(s1)), h2 = mkHist(words(s2));

    if (areAnagrams(h1, h2))
        return true;

    for (auto const& [key, _] : h1)
    {
        if (h2.find(key) != h2.end())
            os << key << ' ';
    }

    return false;
}
