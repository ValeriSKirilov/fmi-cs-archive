//
// Created by Lenovo on 29.1.2026 г..
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional> // за std::function

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

template <typename T>
void deallocate(Node<T>*& root)
{
    if (!root)
        return;

    deallocate(root->left);
    deallocate(root->right);

    delete root;
    root = nullptr;
}

template <typename T>
void printVector(const std::vector<T>& vec)
{
    std::cout << "[ ";
    for (const auto& val : vec) std::cout << val << " ";
    std::cout << "]";
}

template <typename T>
std::size_t height(Node<T>* root)
{
    if (!root) return -1;
    if (!root->left && !root->right) return 0;

    return 1 + std::max(height(root->left), height(root->right));
}

template <typename T>
std::size_t diameter(Node<T>* root)
{
    if (!root) return -1;
    std::size_t diam = height(root->left) + height(root->right);

    return 2 + std::max({diam, diameter(root->left), diameter(root->right)});
}

template <typename T>
void inorderHelper(Node<T>* root, std::vector<T>& res)
{
    if (!root)
        return;

    inorderHelper(root->left, res);
    res.push_back(root->data);
    inorderHelper(root->right, res);
}

template <typename T>
std::vector<T> inorder(Node<T>* root)
{
    std::vector<T> res;
    inorderHelper(root, res);
    return res;
}

template <typename T>
void preorderHelper(Node<T>* root, std::vector<T>& res)
{
    if (!res)
        return;

    res.push_back(root->data);
    preorderHelper(root->left, res);
    preorderHelper(root->right, res);
}

template <typename T>
std::vector<T> preorder(Node<T>* root)
{
    std::vector<T> res;
    preorderHelper(root, res);
    return res;
}

template <typename T>
void bloom(Node<T>* root, const T& x)
{
    if (!root) return;

    if (!root->left && !root->right)
    {
        root->left = new Node<T>(x);
        root->right = new Node<T>(x);
        return;
    }

    bloom(root->left, x);
    bloom(root->right, x);
}

template <typename T, typename U>
Node<U>* mapTree(const std::function<U(const T&)>& f, Node<T>* root)
{
    if (!root) return nullptr;

    Node<U>* newNode = new Node<U>(f(root->data));

    newNode->left = mapTree(f, root->left);
    newNode->right = mapTree(f, root->right);

    return newNode;
}

template <typename T>
std::vector<std::vector<T>> levels(Node<T>* root)
{
    std::vector<std::vector<T>> result;
    if (!root) return result;

    std::queue<Node<T>*> q;
    q.push(root);

    while (!q.empty())
    {
        const int levelSize = q.size();
        std::vector<T> currentLevel;

        for (int i = 0; i < levelSize; ++i)
        {
            Node<T>* curr = q.front();
            q.pop();
            currentLevel.push_back(curr->data);

            if (curr->left)
                q.push(curr->left);

            if (curr->right)
                q.push(curr->right);
        }

        result.push_back(currentLevel);
    }

    return result;
}

template <typename T>
void pathsHelper(Node<T>* root, std::vector<T>& currentPath, std::vector<std::vector<T>>& result)
{
    if (!root) return;

    currentPath.push_back(root->data);

    if (!root->left && !root->right)
        result.push_back(currentPath);
    else
    {
        pathsHelper(root->left, currentPath, result);
        pathsHelper(root->right, currentPath, result);
    }

    currentPath.pop_back();
}

template <typename T>
std::vector<std::vector<T>> paths(Node<T>* root)
{
    std::vector<std::vector<T>> result;
    std::vector<T> currentPath;
    pathsHelper(root, currentPath, result);

    return result;
}

bool checkPermutationHelper(Node<int>* root, const int n, std::vector<bool>& visited, int& count)
{
    if (!root) return true;

    const int val = root->data;

    if (val < 0 || val > n) return false;

    if (visited[val]) return false;

    visited[val] = true;
    count++;

    return checkPermutationHelper(root->left, n, visited, count) &&
        checkPermutationHelper(root->right, n, visited, count);
}

bool checkPermutation(const int n, Node<int>* root)
{
    std::vector visited(n + 1, false);
    int count = 0;

    const bool validValues = checkPermutationHelper(root, n, visited, count);

    return validValues && (count == n + 1);
}

int main()
{
    // --- 1. СЪЗДАВАНЕ НА ДЪРВОТО ---
    /*
           10
          /  \
         5    20
        / \   / \
       1   7 15 30
    */

    // Листа
    Node<int>* n1 = new Node<int>(1);
    Node<int>* n7 = new Node<int>(7);
    Node<int>* n15 = new Node<int>(15);
    Node<int>* n30 = new Node<int>(30);

    // Средно ниво
    Node<int>* n5 = new Node<int>(5);
    n5->left = n1;
    n5->right = n7;

    Node<int>* n20 = new Node<int>(20);
    n20->left = n15;
    n20->right = n30;

    // Корен
    Node<int>* root = new Node<int>(10);
    root->left = n5;
    root->right = n20;

    std::cout << "Tree created successfully.\n\n";

    // --- 2. ТЕСТОВЕ ---

    // Задача 02: Диаметър
    // Очаква се 5 (пътят 1-5-10-20-30 или 7-5-10-20-15 - броим възлите)
    // Забележка: Увери се, че ползваш версията на функцията за <int>
    std::cout << "Task 02 (Diameter): " << diameter(root) << std::endl;

    // Задача 03: Inorder
    std::cout << "Task 03 (Inorder): ";
    printVector(inorder(root)); // Очаквано: 1 5 7 10 15 20 30
    std::cout << std::endl;

    // Задача 06: Levels
    std::cout << "Task 06 (Levels): \n";
    auto lvl = levels(root);
    for (auto& row : lvl)
    {
        std::cout << "  ";
        printVector(row);
        std::cout << "\n";
    }

    // Задача 07: Paths
    std::cout << "Task 07 (Paths): \n";
    auto p = paths(root);
    for (auto& row : p)
    {
        std::cout << "  ";
        printVector(row);
        std::cout << "\n";
    }

    // Задача 08: Permutation Check
    // В момента дървото има числата {1,5,7,10,15,20,30}. Това НЕ е пермутация на 0..6.
    std::cout << "Task 08 (Is Permutation 0..6?): "
        << (checkPermutation(6, root) ? "Yes" : "No") << std::endl;

    // Задача 05: Map (x % 3)
    // Създаваме НОВО дърво, което е трансформирано
    std::cout << "Task 05 (Map x % 3): \n";
    Node<int>* mappedRoot = mapTree<int, int>([](const int& x) { return x % 3; }, root);

    // Принтираме нивата на новото дърво за проверка
    auto mappedLevels = levels(mappedRoot);
    for (auto& row : mappedLevels)
    {
        std::cout << "  ";
        printVector(row);
        std::cout << "\n";
    }
    // ВАЖНО: Трябва да изтрием и новото дърво!
    deallocate(mappedRoot);

    // Задача 04: Bloom (Добавяне на листа)
    // ВНИМАНИЕ: Това променя оригиналното дърво! Правим го последно.
    std::cout << "\nTask 04 (Bloom with 99)... modifying tree.\n";
    bloom(root, 99);

    std::cout << "New Levels after Bloom:\n";
    auto bloomedLevels = levels(root);
    for (auto& row : bloomedLevels)
    {
        std::cout << "  ";
        printVector(row);
        std::cout << "\n";
    }

    // --- 3. ПОЧИСТВАНЕ ---
    std::cout << "\nDeallocating memory...\n";
    deallocate(root);

    if (root == nullptr)
    {
        std::cout << "Tree successfully deleted and pointer set to nullptr.\n";
    }

    return 0;
}
