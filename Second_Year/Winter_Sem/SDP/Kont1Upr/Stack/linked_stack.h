//
// Created by Lenovo on 14.11.2025 г..
//

#ifndef CUSTOMPROJECT_LINKED_STACK_H
#define CUSTOMPROJECT_LINKED_STACK_H
#include "stdexcept"

template <typename T>
struct StackElement
{
    T data;
    StackElement<T>* next;
};

template <typename T>
class LinkedStack
{
    StackElement<T>* topPtr;

    void copyStack(LinkedStack const& other);
    void erase();

public:
    LinkedStack();
    LinkedStack(LinkedStack const& other);
    LinkedStack& operator=(LinkedStack const& other);
    ~LinkedStack();

    LinkedStack(LinkedStack&& other) noexcept;
    LinkedStack& operator=(LinkedStack&& other);

    bool empty() const;

    void push(T const& x);

    void pop();

    T const& top() const;

    T& top();
};

template <typename T>
void LinkedStack<T>::copyStack(LinkedStack const& other)
{
    if (!other.topPtr)
    {
        topPtr = nullptr;
        return;
    }

    topPtr = new StackElement<T>{other.topPtr->data, nullptr};
    StackElement<T>* nextToCopy = other.topPtr->next;
    StackElement<T>* lastAdded = topPtr;
    while (nextToCopy)
    {
        lastAdded = lastAdded->next = new StackElement<T>{nextToCopy->data, nullptr};
        nextToCopy = nextToCopy->next;
    }
}

template <typename T>
void LinkedStack<T>::erase()
{
    while (!empty())
    {
        pop();
    }
}

template <typename T>
LinkedStack<T>::LinkedStack()
{
    topPtr = nullptr;
}

template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack const& other)
{
    copyStack(other);
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack const& other)
{
    if (this != other)
    {
        erase();
        copyStack(other);
    }

    return *this;
}

template <typename T>
LinkedStack<T>::~LinkedStack()
{
    erase();
}

template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack<T>&& other) noexcept
{
    topPtr = other.topPtr;
    other.topPtr = nullptr;
}

template <typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack&& other)
{
    if (this != &other)
    {
        erase();
        topPtr = other.topPtr;
        other.topPtr = nullptr;
    }

    return *this;
}

template <typename T>
bool LinkedStack<T>::empty() const
{
    return topPtr == nullptr;
}

template <typename T>
void LinkedStack<T>::push(T const& x)
{
    topPtr = new StackElement<T>{x, topPtr};
}

template <typename T>
void LinkedStack<T>::pop()
{
    if (empty())
    {
        throw std::runtime_error("Cannot delete the top element of an empty stack");
    }

    StackElement<T>* toDelete = topPtr;
    topPtr = topPtr->next;
    delete toDelete;
}

template <typename T>
T const& LinkedStack<T>::top() const
{
    if (empty())
    {
        throw std::runtime_error("Cannot get the top element of an empty stack");
    }

    return topPtr->data;
}

template <typename T>
T& LinkedStack<T>::top()
{
    if (empty())
    {
        throw std::runtime_error("Cannot get the top element of an empty stack");
    }

    return topPtr->data;
}


#endif //CUSTOMPROJECT_LINKED_STACK_H
