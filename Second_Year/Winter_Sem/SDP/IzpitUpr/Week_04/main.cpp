//
// Created by Lenovo on 24.1.2026 г..
//

#include <iostream>

template <typename T>
struct Node
{
    T value;
    Node* next;

    Node(
        const T& val = T{},
        Node* next = nullptr
    ) : value(val), next(next)
    {
    }
};

template <typename T>
struct DoubleNode
{
    T value;
    DoubleNode *prev, *next;

    DoubleNode(
        const T& val = T{},
        DoubleNode* prev = nullptr,
        DoubleNode* next = nullptr
    ) :
        value(val),
        prev(prev),
        next(next)
    {
    }
};

template <typename T>
struct TripleNode
{
    T value;
    TripleNode *prev, *next;
    TripleNode* child;

    TripleNode(
        const T& val = T{},
        TripleNode* prev = nullptr,
        TripleNode* next = nullptr,
        TripleNode* child = nullptr
    ) :
        value(val),
        prev(prev),
        next(next),
        child(child)
    {
    }
};

template <typename T>
void print(DoubleNode<T>* head)
{
    while (head != nullptr)
    {
        std::cout << head->value << " <-> ";
        head = head->next;
    }
    std::cout << "nullptr\n";
}

template <typename T>
void deallocate(DoubleNode<T>*& head)
{
    while (head != nullptr)
    {
        const DoubleNode<T>* to_delete = head;
        head = head->next;
        delete to_delete;
    }
}

// P1
template <typename T>
bool hasCycle(Node<T>* head)
{
    if (!head || !head->next)
        return false;

    Node<T>* slow = head;
    Node<T>* fast = head;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return true;
    }

    return false;
}

// P2
template <typename T>
void remove_consecutives(DoubleNode<T>*& head)
{
    if (!head || (!head->prev && !head->next))
        return;

    DoubleNode<T> *curr = head, *next = head->next;
    while (next)
    {
        if (curr->value != next->value)
        {
            curr = next;
            next = next->next;
        }
        else
        {
            DoubleNode<T>* nextnext = next->next;
            curr->next = nextnext;
            if (nextnext)
                nextnext->prev = curr;

            delete next;
            next = nextnext;
        }
    }
}

template <typename T>
void helper(TripleNode<T>* _head, DoubleNode<T>*& head, DoubleNode<T>*& tail)
{
    if (_head == nullptr)
        return;

    auto* curr = new DoubleNode<T>(_head->value);
    if (!head)
        head = tail = curr;
    else
    {
        tail->next = curr;
        curr->prev = tail;
        tail = curr;
    }

    if (_head->child)
        helper(_head->child, head, tail);

    helper(_head->next, head, tail);
}

template <typename T>
DoubleNode<T>* flatten(const TripleNode<T>* head)
{
    DoubleNode<T> *new_head = nullptr, *new_tail = nullptr;
    helper(head, new_head, new_tail);

    return new_head;
}

int main()
{
    DoubleNode<int>* el1 = new DoubleNode<int>(1);
    DoubleNode<int>* el2 = new DoubleNode<int>(1);
    DoubleNode<int>* el3 = new DoubleNode<int>(1);
    DoubleNode<int>* el4 = new DoubleNode<int>(2);
    DoubleNode<int>* el5 = new DoubleNode<int>(2);
    DoubleNode<int>* el6 = new DoubleNode<int>(3);
    DoubleNode<int>* el7 = new DoubleNode<int>(3);

    el1->next = el2;
    el2->prev = el1;
    el2->next = el3;
    el3->prev = el2;
    el3->next = el4;
    el4->prev = el3;
    el4->next = el5;
    el5->prev = el4;
    el5->next = el6;
    el6->prev = el5;
    el6->next = el7;
    el7->prev = el6;

    print<int>(el1);
    remove_consecutives<int>(el1);
    print<int>(el1);

    deallocate<int>(el1);

    return 0;
}
