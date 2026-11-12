//
// Created by Lenovo on 31.12.2025 г..
//

#include "BinaryTree.h"
#include "BinaryTreeUtil.h"

int main()
{
    BinaryTree<int> tree1 = BinaryTree<int>{
        10,
        BinaryTree<int>{
            5,
            BinaryTree<int>{1},
            BinaryTree<int>{7}
        },
        BinaryTree<int>{
            20,
            BinaryTree<int>{15},
            BinaryTree<int>{30}
        }
    };

    // const std::vector<int> inOrder = inorder(tree1);
    //
    // for (const int i : inOrder)
    // {
    //     std::cout << i << ' ';
    // }
    //
    // std::cout << std::endl;
    //
    // const std::vector<int> preOrder = preorder(tree1);
    //
    // for (const int i : preOrder)
    // {
    //     std::cout << i << ' ';
    // }
    //
    // std::cout << diameter(tree1);

    const std::vector<std::vector<int>> result = BinaryTreeUtil<int, BinaryTree>::paths(tree1);

    std::cout << "[ ";
    for (const auto& row : result)
    {
        std::cout << "[";
        for (std::size_t j = 0; j < row.size(); ++j)
        {
            std::cout << row[j];

            if (j < row.size() - 1)
            {
                std::cout << ",";
            }
        }
        std::cout << "] ";
    }
    std::cout << "]";

    return 0;
}
