//
// Created by Lenovo on 13.11.2025 г..
//

#ifndef FMI_RESIZABLE_STACK_HPP
#define FMI_RESIZABLE_STACK_HPP

#include <iostream>

template <typename T>
class RStack
{
private:
    T* elements;
    int topIndex;
    int capacity;

    bool full() const;
    void resize();
    void eraseStack();
    void copyElements(T const*);
    void copyStack(RStack const&);

public:
    RStack();
    RStack(RStack const&);
    RStack& operator=(RStack const&);
    ~RStack();

    RStack(RStack&&);
    RStack& operator=(RStack&&);

    bool empty() const;
    void push(T const& x);
    void pop();
    T top() const;
};

const unsigned INITIAL_CAPACITY = 16;

template <typename T>
bool RStack<T>::full() const
{
    return topIndex == capacity - 1;
}

template <typename T>
void RStack<T>::eraseStack()
{
    delete[] elements;
}

template <typename T>
void RStack<T>::copyElements(T const* stackElements)
{
    elements = new T[capacity];

    for (unsigned i = 0; i < capacity; i++)
    {
        elements[i] = stackElements[i];
    }
}

template <typename T>
void RStack<T>::copyStack(RStack const& stack)
{
    topIndex = stack.topIndex;
    capacity = stack.capacity;
    copyElements(stack.elements);
}


template <typename T>
void RStack<T>::resize()
{
    T* oldStackPtr = elements;
    elements = new T[2 * capacity];

    for (unsigned i = 0; i < capacity; i++)
    {
        elements[i] = oldStackPtr[i];
    }

    capacity *= 2;
    delete[] oldStackPtr;
}

template <typename T>
RStack<T>::RStack() : topIndex(-1), capacity(INITIAL_CAPACITY)
{
    elements = new T[capacity];
}

template <typename T>
RStack<T>::RStack(RStack<T> const& other)
{
    copyStack(other);
}

template <typename T>
RStack<T>& RStack<T>::operator=(RStack<T> const& other)
{
    if (this != &other)
    {
        eraseStack();
        copyStack(other);
    }

    return *this;
}

template <typename T>
RStack<T>::~RStack()
{
    eraseStack();
}

template <typename T>
RStack<T>::RStack(RStack&& other)
{
    topIndex = other.topIndex;
    capacity = other.capacity;

    elements = other.elements;
    other.elements = nullptr;
}

template <typename T>
RStack<T>& RStack<T>::operator=(RStack&& other)
{
    if (this != &other)
    {
        topIndex = other.topIndex;
        capacity = other.capacity;

        eraseStack();
        elements = other.elements;
        other.elements = nullptr;
    }

    return *this;
}

template <typename T>
bool RStack<T>::empty() const
{
    return topIndex == -1;
}

template <typename T>
void RStack<T>::push(T const& x)
{
    if (full()) resize();
    elements[++topIndex] = x;
}

template <typename T>
void RStack<T>::pop()
{
    if (empty())
    {
        throw std::runtime_error("Cannot delete the top element of an empty stack");
    }

    topIndex--;
}

template <typename T>
T RStack<T>::top() const
{
    if (empty())
    {
        throw std::runtime_error("Cannot get the top element of an empty stack");
    }

    return elements[topIndex];
}

#endif //FMI_RESIZABLE_STACK_HPP
