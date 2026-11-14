//
// Created by Lenovo on 28.1.2026 г..
//

#include <stack>
#include <iostream>
#include <vector>

bool subsetSum(const std::vector<int>& v, int target)
{
    std::stack<std::pair<int, int>> stack;

    stack.push({target, 0});
    while (!stack.empty())
    {
        auto [res, idx] = stack.top();
        stack.pop();

        if (res == 0) return true;
        if (idx >= v.size()) continue;

        stack.push({res - v[idx], idx + 1});
        stack.push({res, idx + 1});
    }

    return false;
}
