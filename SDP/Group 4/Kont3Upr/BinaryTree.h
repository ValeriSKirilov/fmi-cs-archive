//
// Created by Lenovo on 31.12.2025 г..
//

#ifndef KONT3UPR_BINARYTREE_H
#define KONT3UPR_BINARYTREE_H

#include <stdexcept>
#include <utility>

template <typename T>
class BinaryTree
{
    class Position;

public:
    using position = Position;

    bool empty() const
    {
        return !m_root;
    }

    operator bool() const
    {
        return !empty();
    }

    position root() { return position(m_root); }

    position root() const { return position(m_root); }

public:
    BinaryTree() : m_root(nullptr)
    {
    }

    BinaryTree(T const& root)
        : m_root(new Node(root))
    {
    }

    BinaryTree(T const& root,
               BinaryTree const& left,
               BinaryTree const& right)
        : m_root(new Node(root))
    {
        m_root->left = copy(left.m_root);
        m_root->right = copy(right.m_root);
    }

    BinaryTree(T const& root,
               BinaryTree&& left,
               BinaryTree&& right)
        : m_root(new Node(root, left.m_root, right.m_root))
    {
        left.m_root = nullptr;
        right.m_root = nullptr;
    }

    BinaryTree& operator=(BinaryTree const& other)
    {
        if (this != &other)
        {
            free(m_root);
            m_root = copy(other.m_root);
        }

        return *this;
    }

    BinaryTree& operator=(BinaryTree&& other) noexcept
    {
        if (this != &other)
        {
            free(m_root);
            m_root = std::exchange(other.m_root, nullptr);
        }

        return *this;
    }

    ~BinaryTree() { free(m_root); }

private:
    struct Node
    {
        T data;
        Node *left, *right;

        Node(T const& data, Node* left = nullptr, Node* right = nullptr)
            : data(data), left(left), right(right)
        {
        }
    };

    class Position
    {
        friend class BinaryTree<T>;

    public:
        Position(Node* pos) : pos(pos)
        {
        }

        bool valid() const
        {
            return pos;
        }

        operator bool() const
        {
            return valid();
        }

        bool operator!=(Position const& other) const
        {
            return pos != other.pos;
        }

        bool operator==(Position const& other) const
        {
            return pos == other.pos;
        }

        T& operator*()
        {
            if (!valid())
                throw std::runtime_error("Invalid position");

            return pos->data;
        }

        T const& operator*() const
        {
            if (!valid())
                throw std::runtime_error("Invalid position");

            return pos->data;
        }

        Position left() const
        {
            if (!valid())
                throw std::runtime_error("Invalid position");

            return Position(pos->left);
        }

        Position right() const
        {
            if (!valid())
                throw std::runtime_error("Invalid position");

            return Position(pos->right);
        }

        Position operator-() const
        {
            return left();
        }

        Position operator+() const
        {
            return right();
        }

        Position& operator--()
        {
            return *this = left();
        }

        Position& operator++()
        {
            return *this = right();
        }

        Position operator--(int)
        {
            Position saved = *this;
            --(*this);
            return saved;
        }

        Position operator++(int)
        {
            Position saved = *this;
            ++(*this);
            return saved;
        }

    private:
        Node* pos;
    };

private:
    Node* copy(Node* pos)
    {
        if (!pos) return pos;

        Node* root = new Node(pos->data);
        root->left = copy(pos->left);
        root->right = copy(pos->right);

        return root;
    }

    void free(Node* pos)
    {
        if (!pos) return;

        free(pos->left);
        free(pos->right);

        delete pos;
        pos = nullptr;
    }

protected:
    Node* m_root;

    using BTNode = Node;
};

#endif //KONT3UPR_BINARYTREE_H
