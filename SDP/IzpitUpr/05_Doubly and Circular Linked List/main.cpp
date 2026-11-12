//
// Created by Lenovo on 29.1.2026 г..
//

#include <vector>
#include <bits/streambuf_iterator.h>

struct Node
{
    int data;
    Node* next;

    Node(const int data, Node* next = nullptr)
        : data(data), next(next)
    {
    }
};

bool hasCycle(Node* head)
{
    if (!head) return false;

    Node* slow = head;
    Node* fast = head->next;

    while (fast && fast->next)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
            return true;
    }

    return false;
}

Node* find_last(Node* l)
{
    Node* iter = l;

    while (iter && iter->next != l)
    {
        iter = iter->next;
    }

    return iter;
}

void push_front(const int el, Node*& front)
{
    if (!front)
    {
        front = new Node(el);
        front->next = front;
        return;
    }

    Node* last = find_last(front);

    front = new Node(el, front);
    last->next = front;
}

void remove_next(Node*& l)
{
    if (!l || !l->next) return;
    if (l == l->next)
    {
        delete l;
        l = nullptr;
        return;
    }

    Node* save = l->next->next;
    delete l->next;
    l->next = save;
}

int josephus(int n, const int k)
{
    if (k <= 0 || n <= 0)
        return -1;

    Node* list = nullptr;

    while (n > 0)
        push_front(n--, list);

    Node* prev = find_last(list);

    while (prev != prev->next)
    {
        for (int i = 0; i < k - 1; ++i)
            prev = prev->next;

        remove_next(prev);
    }

    const int winner = prev->data;
    delete prev;

    return winner;
}
