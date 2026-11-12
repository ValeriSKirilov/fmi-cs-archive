//
// Created by Lenovo on 11.1.2026 г..
//

#include <iostream>
#include <queue>

template <typename T>
struct BinTreeNode
{
    T data;
    BinTreeNode *left, *right;

    BinTreeNode(T data, BinTreeNode* left = nullptr, BinTreeNode* right = nullptr) : data(data), left(left),
        right(right)
    {
    }
};

// Нивата си работят правилно, но се дублира сумата и нямам идея защо (затова е оставен std::cout-а). Също 1 5 8 си излизат като ред ама не се сумират, само то си знае защо
template <typename T>
std::vector<T> findSumsPerLayer(BinTreeNode<T>* root)
{
    std::vector<T> result;
    std::queue<BinTreeNode<T>*> q;

    q.push(root);

    result.push_back(root->data);

    while (!q.empty())
    {
        int sum = 0;

        std::queue<BinTreeNode<T>*> copy = q, newChildren;

        while (!copy.empty())
        {
            std::cout << copy.front()->data << " ";

            if (copy.front()->left)
            {
                newChildren.push(copy.front()->left);
            }

            if (copy.front()->right)
            {
                newChildren.push(copy.front()->right);
            }

            sum += copy.front()->data;
            copy.pop();
        }
        std::cout << std::endl;

        if (!newChildren.empty())
            q = newChildren;
        else
            break;

        result.emplace_back(sum);
    }

    return result;
}

template <typename T>
bool hasEqualSums(const std::vector<T>& sums)
{
    for (std::size_t i = 0; i < sums.size(); i++)
    {
        for (std::size_t j = i + 1; j < sums.size(); ++j)
        {
            if (sums[i] == sums[j])
            {
                std::cout << "Levels with equal sums: " << i << " and " << j << std::endl;
            }
        }
    }

    return false;
}

int main()
{
    auto* root = new BinTreeNode<int>(7,
                                      new BinTreeNode<int>(4,
                                                           new BinTreeNode<int>(1, nullptr, nullptr),
                                                           new BinTreeNode<int>(5, nullptr, nullptr)),
                                      new BinTreeNode<int>(10,
                                                           new BinTreeNode<int>(8, nullptr, nullptr),
                                                           nullptr));

    const std::vector<int> allSums = findSumsPerLayer(root);

    for (int i = 0; i < allSums.size(); i++)
        std::cout << allSums[i] << std::endl;

    std::cout << hasEqualSums(allSums);
}
