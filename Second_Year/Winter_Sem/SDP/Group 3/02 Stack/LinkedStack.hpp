#ifndef __LINKED_STACK_HPP
#define __LINKED_STACK_HPP

#include <stdexcept>

template <class T>
class LinkedStack
{
public:
    bool empty() const
    {
        return !m_top;
    }

    void push(T const& data)
    {
        m_top = new Node(data, m_top);
    }

    T pop()
    {
        if (empty()) throw std::underflow_error("Stack is empty");

        T res = m_top->data;
        Node* save = m_top->next;
        delete m_top;
        m_top = save;
        return res;
    }

    T const& peek() const
    {
        if (empty()) throw std::underflow_error("Stack is empty");
        return m_top->data;
    }

    LinkedStack() : m_top(nullptr)
    {
    }

    LinkedStack(LinkedStack const& other)
    {
        copy(other);
    }

    LinkedStack(LinkedStack&& other) noexcept
    {
        move(std::move(other));
    }

    LinkedStack& operator=(LinkedStack const& other)
    {
        if (this != &other)
        {
            free();
            copy(other);
        }
        return *this;
    }

    LinkedStack& operator=(LinkedStack&& other) noexcept
    {
        if (this != &other)
        {
            free();
            move(std::move(other));
        }
        return *this;
    }

    ~LinkedStack()
    {
        free();
    }

    void copy(LinkedStack const& other)
    {
        Node dummy;
        Node* dptr = &dummy;
        Node* optr = other.m_top;

        while (optr)
        {
            dptr->next = new Node(*optr->next);
            optr = optr->next;
            dptr = dptr->next;
        }
        m_top = dummy.next;
    }

    void move(LinkedStack const&& other)
    {
        m_top = other.m_top;
        other.m_top = nullptr;
    }

    void free()
    {
        while (m_top)
        {
            pop();
        }
    }

    size_t size() const
    {
        Node* ptr = m_top;
        size_t s = 0;
        while (ptr)
        {
            ++s;
            ptr = ptr->next;
        }
        return s;
    }

    void clear()
    {
        free();
    }

    operator bool() const
    {
        return !empty();
    }

private:
    struct Node
    {
        T data;
        Node* next;

        Node(T const& data = T(), Node* next = nullptr) : data(data), next(next)
        {
        }
    };

private:
    Node* m_top;
};

#endif
