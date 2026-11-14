//
// Created by Lenovo on 31.12.2025 г..
//

#ifndef KONT3UPR_BINARYTREEUTIL_H
#define KONT3UPR_BINARYTREEUTIL_H

#include <queue>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <iostream>

template <typename T, template<typename> class BinaryTreeType>
class BinaryTreeUtil
{
    using position = typename BinaryTreeType<T>::position;

public:
    static long long height(BinaryTreeType<T> const& t)
    {
        return height(t.root());
    }

    static long long diameter(BinaryTreeType<T> const& t)
    {
        return diameter(t.root());
    }

    static void serialise(BinaryTreeType<T> const& t, std::ostream& os = std::cout)
    {
        serialise(t.root(), "", false, os);
    }

    std::vector<T> inorder(BinaryTreeType<T> const& tree)
    {
        std::vector<T> result;
        inorder(tree.root(), result);
        return result;
    }

    std::vector<T> preorder(BinaryTreeType<T> const& tree)
    {
        std::vector<T> result;
        preorder(tree.root(), result);
        return result;
    }

    template <typename U>
    static BinaryTreeType<U> map(U (*f)(T const&), BinaryTreeType<T> const& t)
    {
        return map(f, t.root());
    }

    static std::vector<std::vector<T>> levels(BinaryTreeType<T> const& tree)
    {
        std::vector<std::vector<T>> result;

        if (tree.empty()) return result;

        std::queue<position> q;
        q.push(tree.root());

        while (!q.empty())
        {
            const size_t levelSize = q.size();

            std::vector<T> level;
            for (size_t i = 0; i < levelSize; ++i)
            {
                auto currPos = q.front();
                q.pop();

                if (!currPos.valid()) continue;

                level.push_back(*currPos);

                q.push(currPos.left());
                q.push(currPos.right());
            }

            result.push_back(level);
        }

        return result;
    }

    static std::vector<std::vector<T>> paths(BinaryTreeType<T> const& t)
    {
        std::vector<std::vector<T>> result;
        std::vector<T> acc;
        paths(t.root(), acc, result);
        return result;
    }

private:
    static long long height(position pos)
    {
        if (!pos.valid()) return -1;
        if (!pos.left().valid() && !pos.right().valid()) return 0;

        return 1 + std::max(height(pos.left()), height(pos.right()));
    }

    static long long diameter(position pos)
    {
        if (!pos.valid()) return -1;

        long long diam = height(pos.left()) + height(pos.right());

        return 2 + std::max({diam, height(pos.left()), height(pos.right())});
    }

    static void serialise(position pos, std::string const& prefix, bool isLeft, std::ostream& os)
    {
        if (!pos.valid()) return;

        os << prefix;

        os << (isLeft ? "|---" : "+---");

        os << *pos << std::endl;

        serialise(pos.left(), prefix + (isLeft ? "|  " : "   "), true, os);
        serialise(pos.right(), prefix + (isLeft ? "|  " : "   "), false, os);
    }

    void inorder(position pos, std::vector<T>& result)
    {
        if (!pos.valid()) return;

        inorder(pos.left(), result);
        result.push_back(*pos);
        inorder(pos.right(), result);
    }

    void preorder(position pos, std::vector<T>& result)
    {
        if (!pos.valid()) return;

        result.push_back(*pos);
        preorder(pos.left(), result);
        preorder(pos.right(), result);
    }

    template <typename U>
    static BinaryTreeType<U> map(U (*f)(T const&), position pos)
    {
        if (!pos.valid()) return BinaryTreeType<U>{};

        return BinaryTreeType<U>(f(*pos),
                                 map(f, pos.left()),
                                 map(f, pos.right()));
    }

    static void paths(position pos, std::vector<T>& acc, std::vector<std::vector<T>>& result)
    {
        if (!pos.valid())
        {
            return;
        }

        acc.push_back(*pos);

        if (!pos.left().valid() && !pos.right().valid())
        {
            result.push_back(acc);
        }
        else
        {
            paths(pos.left(), acc, result);
            paths(pos.right(), acc, result);
        }

        acc.pop_back();
    }
};

#endif //KONT3UPR_BINARYTREEUTIL_H
