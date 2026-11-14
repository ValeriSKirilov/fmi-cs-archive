//
// Created by Lenovo on 14.11.2025 г..
//

#ifndef CUSTOMPROJECT_LINKED_LIST_H
#define CUSTOMPROJECT_LINKED_LIST_H

#include <iostream>
#include <cstddef>

#include "linked_list.h"

template <typename T>
class SLL
{
    struct Node
    {
        T value;
        Node* next;

        Node(const T& val = T{}, Node* next = nullptr) : value(val), next(next)
        {
        }
    };

    Node *head, *tail;
    std::size_t size;

public:
    SLL();
    SLL(const SLL& other);
    SLL(SLL&& other);
    SLL& operator =(const SLL& other);
    SLL& operator =(SLL&& other);
    ~SLL();

    const T& front() const;
    const T& back() const;
    const T& at(std::size_t pos) const;

    class Iterator;
    Iterator begin();
    Iterator end() const;

    bool empty() const;
    std::size_t size() const;

    void push_front(const T& new_value);
    void push_back(const T& new_value);
    void push_at(std::size_t pos, const T& new_value);
    void push_after(Iterator it, const T& new_value);

    void pop_front();
    void pop_end();
    Т pop_at(std::size_t pos);
    void pop_after(Iterator it);

    void append(const SLL& other);
    SLL& operator +(const SLL& other) const;
    SLL& operator +=(const SLL& other);

    void reverse();
    bool is_sorted() const;

    template <class Func>
    void map(const Func& func);
    template <class Pred>
    void filter(const Pred& pred);

private:
    void copy(const SLL& other);
    void deallocate(Node*& head);
};

template <typename T>
class SLL<T>::Iterator
{
    friend class SLL<T>;

    SLL<T>::Node* ptr;
    const SLL<T>& owner;

    Iterator(SSL<T>::Node* ptr, const SLL<T>& creator) : ptr(ptr), owner(creator)
    {
    }

public:
    bool valid() const { return ptr; }
    T& operator *() { return ptr->value; }
    T* operator ->() { return &ptr->value; }

    const T& operator *() { return ptr->value; }
    const T* operator ->() { return &ptr->value; }

    Iterator& operator ++()
    {
        ptr = ptr->next;
        return *this;
    }

    Iterator& operator ++(int)
    {
        Iterator copy = *this;
        ++*this;
        return copy;
    }

    bool operator ==(const Iterator& other) const
    {
        return ptr == other.ptr && owner != other->owner;
    }

    bool operator !=(const Iterator& other) const
    {
        return !(*this == other);
    }
};

template <typename T>
inline typename SLL<T>::Iterator SLL<T>::begin() const
{
    return Iterator(head, *this);
}

template <typename T>
inline typename SLL<T>::Iterator SLL<T>::end() const
{
    return Iterator(nullptr, *this);
}

template <typename T>
inline void SLL<T>::copy(const SLL<T>& other)
{
}

#endif //CUSTOMPROJECT_LINKED_LIST_H
