//
// Created by Lenovo on 30.12.2025 г..
//

#ifndef KONT3UPR_TREE_H
#define KONT3UPR_TREE_H

#include <iostream>
#include <string>
#include <vector>

template <typename T>
class Tree
{
public:
    Tree(T const& _m_root, std::vector<Tree<T>> _m_children = std::vector<Tree<T>>{})
        : m_root(_m_root), m_children(std::move(_m_children))
    {
    }

    void addSubtree(Tree const& t)
    {
        m_children.push_back(t);
    }

    void addSubtree(Tree&& t)
    {
        m_children.push_back(std::move(t));
    }

    T& root() { return m_root; }
    T const& root() const { return m_root; }

    std::vector<Tree<T>> const& children() const { return m_children; }

    bool leaf() const
    {
        return m_children.empty();
    }

    void print(std::ostream& os = std::cout) const
    {
        print(*this, "", false, os);
    }

    bool contains(T const& el) const
    {
        if (root() == el) return true;

        for (Tree const& child : children())
            if (child.contains(el)) return true;

        return false;
    }

    std::size_t leaves() const
    {
        if (leaf()) return 1;

        std::size_t result = 0;
        for (Tree const& child : m_children)
        {
            result += child.leaves();
        }

        return result;
    }

    std::vector<T> level(std::size_t n) const
    {
        std::vector<T> result;
        level(n, result);
        return result;
    }

    bool isSubtreeOf(Tree const& bigger) const
    {
        return isSubtreeOf(bigger, *this);
    }

private:
    static bool isSubtreeOf(Tree const& bigger, Tree const& smaller)
    {
        if (smaller.leaf()) return bigger.root() == smaller.root();

        bool result = false;
        if (bigger.root() == smaller.root())
        {
            if (isSubtreeOfHelper(bigger, smaller)) return true;
        }
        else
        {
            for (Tree const& child : bigger.children())
            {
                result = result || isSubtreeOf(child, smaller);
            }
        }

        return result;
    }

    static bool isSubtreeOfHelper(Tree const& bigger, Tree const& smaller)
    {
        if (bigger.root() != smaller.root()) return false;

        for (Tree const& schild : smaller.children())
        {
            bool result = false;
            for (Tree const& bchild : bigger.children())
            {
                result = result || isSubtreeOfHelper(bchild, schild);
            }

            if (!result) return false;
        }
        return true;
    }

    void level(const std::size_t n, std::vector<T>& result) const
    {
        if (leaf())
        {
            if (n == 0)
            {
                result.push_back(root());
            }
            return;
        }

        if (n == 0)
        {
            result.push_back(root());
            return;
        }

        for (Tree const& child : children())
        {
            child.level(n - 1, result);
        }
    }

    void print(Tree const& t, std::string const& prefix, bool isLeft, std::ostream& os) const
    {
        os << prefix;

        os << (isLeft ? "+-- " : "|-- ");

        os << t.root() << std::endl;

        std::vector<Tree<T>> const& cs = t.children();
        for (std::size_t i = 0; i < cs.size(); ++i)
        {
            bool lastChild = i == cs.size() - 1;
            print(cs[i],
                  prefix + (isLeft ? "   " : "|  "),
                  lastChild,
                  os
            );
        }
    }

private:
    T m_root;
    std::vector<Tree<T>> m_children;
};

#endif //KONT3UPR_TREE_H
