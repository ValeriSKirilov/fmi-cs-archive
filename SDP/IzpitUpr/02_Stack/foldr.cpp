//
// Created by Lenovo on 28.1.2026 г..
//

#include <iostream>
#include <stack>
#include <vector>

template <typename T, typename U>
U foldr(U (*op)(T, U), U nv, const std::vector<T>& v, std::size_t i)
{
    if (i == v.size()) return nv;
    return op(v[i], foldr(op, nv, v, i + 1));
}

template <typename T, typename U>
U foldr(U (*op)(T, U), U nv, const std::vector<T>& v)
{
    return foldr(op, nv, v, 0);
}

template <typename T, typename U>
U foldrStack(U (*op)(T, U), U nv, const std::vector<T>& v)
{
    std::stack<T> stack;

    for (T el : v)
    {
        stack.push(el);
    }

    while (!stack.empty())
    {
        nv = op(stack.top(), nv);
        stack.pop();
    }

    return nv;
}
