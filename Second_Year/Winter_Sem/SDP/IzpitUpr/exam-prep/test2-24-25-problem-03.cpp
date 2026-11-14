//
// Created by Lenovo on 30.1.2026 г..
//

#include <iostream>
#include <vector>

struct Node
{
    int code;
    Node *prev, *next;

    Node(int code, Node* prev = nullptr, Node* next = nullptr) : code(code), prev(prev), next(next)
    {
    }
};

Node* fromVector(const std::vector<int>& v)
{
    if (v.empty())
        return nullptr;

    Node dummy(0);
    Node* dptr = &dummy;

    for (int el : v)
    {
        dptr->next = new Node(el, dptr);
        dptr = dptr->next;
    }

    dummy.next->prev = nullptr;
    return dummy.next;
}

Node* moveLeft(Node* start, int n)
{
    while (n > 0 && start)
    {
        start = start->prev;
        n--;
    }
    return start;
}

Node* moveRight(Node* start, int n)
{
    while (n > 0 && start)
    {
        start = start->next;
        n--;
    }
    return start;
}

void removeNode(Node* list)
{
    if (!list) return;

    Node *previous = list->prev, *next = list->next;

    delete list;

    if (previous)
        previous->next = next;

    if (next)
        next->prev = previous;
}

int steps(Node* start, int n)
{
    int k = 0;
    int cnt = 0;

    while (start && start->code != 0)
    {
        k++;
        cnt++;

        if (k == n)
        {
            k = 0;

            Node* save = start;
            if (start->code > 0)
                start = start->next;
            else
                start = start->prev;

            removeNode(save);

            continue;
        }

        if (start->code > 0)
            start = moveRight(start, start->code);
        else
            start = moveLeft(start, start->code);
    }

    if (!start)
        return -1;

    return cnt + 1;
}

void free(const Node* list)
{
    while (list)
    {
        const Node* tmp = list->next;
        delete list;
        list = tmp;
    }
}
