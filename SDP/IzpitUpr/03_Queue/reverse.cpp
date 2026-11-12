//
// Created by Lenovo on 28.1.2026 г..
//

#include <queue>
#include <stack>
#include <iostream>

template <typename T>
void reverseRec(std::queue<T>& q)
{
    if (q.empty()) return;

    T tmp = q.front();
    q.pop();
    reverseRec(q);

    q.push(std::move(tmp));
}

template <typename T>
void reverseStack(std::queue<T>& q)
{
    std::stack<T> s;

    while (!q.empty())
    {
        s.push(q.front());
        q.pop();
    }

    while (!s.empty())
    {
        q.push(s.top());
        s.pop();
    }
}
